// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <Common/SessionInfo.hpp>
#include <QSignalSpy>
#include <QTest>
#include <TestHelper/FakeLaptimer.hpp>
#include <TestHelper/SessionJsonDeserializerMock.hpp>
#include <TestHelper/SessionStorageMock.hpp>
#include <Workflow/HttpDeviceSessionManagement.hpp>

namespace RapidAndroid::Workflow::Tests
{
using RestDeviceSessionManagement =
    RapidAndroid::Workflow::HttpDeviceSessionManagement<TestHelper::SessionJsonDeserializerMock,
                                                        TestHelper::SessionStorageMock>;

MATCHER_P(SessionMatcher, sessionPtr, "")
{
    return *sessionPtr == *arg;
};

class HttpDeviceSessionManagementTest : public QObject
{
    Q_OBJECT
public:
    Q_DISABLE_COPY_MOVE(HttpDeviceSessionManagementTest)
    HttpDeviceSessionManagementTest() = default;
    ~HttpDeviceSessionManagementTest() override = default;

private:
    void fetchSessionInfos(RestDeviceSessionManagement& rdsm, Common::DeviceSettings const& settings) noexcept
    {
        auto finishedSpy = QSignalSpy(&rdsm, &IDeviceSessionManagement::sessionInfosSynchronized);
        rdsm.synchronizeSessionInfos(settings);
        QTRY_COMPARE_WITH_TIMEOUT(finishedSpy.count(), 1, std::chrono::seconds{5});
        QCOMPARE(rdsm.getDeviceSessionInfoListModel()->rowCount(), 2);
    }

private Q_SLOTS:
    void testModelIsEmpty()
    {
        auto fakeLaptimer = TestHelper::FakeLaptimer{};
        auto deserializer = TestHelper::SessionJsonDeserializerMock{};
        auto storage = TestHelper::SessionStorageMock{};
        auto rdsm = RestDeviceSessionManagement{&deserializer, &storage};
        auto* const model = rdsm.getDeviceSessionInfoListModel();
        QCOMPARE_NE(model, nullptr);
        QCOMPARE(model->rowCount(), 0);
    }

    void testSynchonisationOfSessionInfos()
    {
        auto fakeLaptimer = TestHelper::FakeLaptimer{};
        auto deserializer = TestHelper::SessionJsonDeserializerMock{};
        auto storage = TestHelper::SessionStorageMock{};
        auto rdsm = RestDeviceSessionManagement{&deserializer, &storage};
        auto finishedSpy = QSignalSpy(&rdsm, &IDeviceSessionManagement::sessionInfosSynchronized);
        auto settings = fakeLaptimer.deviceSettings();

        rdsm.synchronizeSessionInfos(fakeLaptimer.deviceSettings());
        QTRY_COMPARE_WITH_TIMEOUT(finishedSpy.count(), 1, std::chrono::seconds{5});
    }

    void testModelSynchronization()
    {
        auto fakeLaptimer = TestHelper::FakeLaptimer{};
        auto deserializer = TestHelper::SessionJsonDeserializerMock{};
        auto storage = TestHelper::SessionStorageMock{};
        auto rdsm = RestDeviceSessionManagement{&deserializer, &storage};
        auto finishedSpy = QSignalSpy(&rdsm, &IDeviceSessionManagement::sessionInfosSynchronized);
        auto settings = fakeLaptimer.deviceSettings();

        fetchSessionInfos(rdsm, settings);

        auto* const model = rdsm.getDeviceSessionInfoListModel();
        QCOMPARE_NE(model, nullptr);

        QCOMPARE(model->rowCount(), 2);
        auto index = model->index(0, 0);
        auto role = 257; // SessionListModel::Laptimer
        // clang-format off
        QCOMPARE(model->data(index, role).value<Common::SessionInfo>().id, fakeLaptimer.getSessionInfo2().id);
        QCOMPARE(model->data(index, role).value<Common::SessionInfo>().trackName,fakeLaptimer.getSessionInfo2().trackName);
        QCOMPARE(model->data(index, role).value<Common::SessionInfo>().date, fakeLaptimer.getSessionInfo2().date);
        QCOMPARE(model->data(index, role).value<Common::SessionInfo>().laps, fakeLaptimer.getSessionInfo2().laps);
        index = model->index(1, 0);
        QCOMPARE(model->data(index, role).value<Common::SessionInfo>().id, fakeLaptimer.getSessionInfo1().id);
        QCOMPARE(model->data(index, role).value<Common::SessionInfo>().trackName,fakeLaptimer.getSessionInfo1().trackName);
        QCOMPARE(model->data(index, role).value<Common::SessionInfo>().date, fakeLaptimer.getSessionInfo1().date);
        QCOMPARE(model->data(index, role).value<Common::SessionInfo>().laps, fakeLaptimer.getSessionInfo1().laps);
        // clang-format on
    }

    void testMultipleSynchronisations()
    {
        auto fakeLaptimer = TestHelper::FakeLaptimer{};
        auto deserializer = TestHelper::SessionJsonDeserializerMock{};
        auto storage = TestHelper::SessionStorageMock{};
        auto rdsm = RestDeviceSessionManagement{&deserializer, &storage};
        auto settings = fakeLaptimer.deviceSettings();

        fetchSessionInfos(rdsm, settings);
        fetchSessionInfos(rdsm, settings);
        fetchSessionInfos(rdsm, settings);

        auto* const model = rdsm.getDeviceSessionInfoListModel();
        QCOMPARE_NE(model, nullptr);
        QCOMPARE(model->rowCount(), 2);
    }

    void testDeleteSession()
    {
        auto fakeLaptimer = TestHelper::FakeLaptimer{};
        auto deserializer = TestHelper::SessionJsonDeserializerMock{};
        auto storage = TestHelper::SessionStorageMock{};
        auto rdsm = RestDeviceSessionManagement{&deserializer, &storage};
        auto deleteSpy = QSignalSpy(&rdsm, &IDeviceSessionManagement::sessionDeleted);
        auto settings = fakeLaptimer.deviceSettings();

        fetchSessionInfos(rdsm, settings);

        rdsm.deleteSession(settings, "sess-123");
        QTRY_COMPARE_WITH_TIMEOUT(deleteSpy.count(), 1, std::chrono::seconds{5});
        // clang-format off
        QCOMPARE(deleteSpy.at(0).at(0).toString(), "sess-123");
        QCOMPARE(deleteSpy.at(0).at(1).value<IDeviceSessionManagement::Result>(), IDeviceSessionManagement::Result::Success);
        QCOMPARE(rdsm.getDeviceSessionInfoListModel()->rowCount(), 1);
        // clang-format on
    }

    void testDownloadSession()
    {
        auto fakeLaptimer = TestHelper::FakeLaptimer{};
        auto deserializer = TestHelper::SessionJsonDeserializerMock{};
        auto storage = TestHelper::SessionStorageMock{};
        auto rdsm = RestDeviceSessionManagement{&deserializer, &storage};
        auto downloadSpy = QSignalSpy(&rdsm, &IDeviceSessionManagement::sessionDownloaded);
        auto settings = fakeLaptimer.deviceSettings();

        auto sessionMatcher = testing::Truly([&fakeLaptimer](std::unique_ptr<Common::Session> const& sessionPtr) {
            return *sessionPtr == *fakeLaptimer.getSession1();
        });

        EXPECT_CALL(storage, store(sessionMatcher))
            .Times(1)
            .WillOnce(testing::Return(QtConcurrent::run([&fakeLaptimer]() -> Workflow::StoreResult {
                return Workflow::StoreResult{.session = fakeLaptimer.getSession1(), .success = true};
            })));
        EXPECT_CALL(deserializer, deserialize(testing::_))
            .Times(1)
            .WillOnce(
                testing::Return(QtConcurrent::run([&fakeLaptimer]() -> std::optional<std::unique_ptr<Common::Session>> {
                    return fakeLaptimer.getSession1();
                })));

        rdsm.downloadSession(settings, "sess-123");
        QTRY_COMPARE_WITH_TIMEOUT(downloadSpy.count(), 1, std::chrono::seconds{5});
        // clang-format off
        QCOMPARE(downloadSpy.at(0).at(0).toString(), "sess-123");
        QCOMPARE(downloadSpy.at(0).at(1).value<IDeviceSessionManagement::Result>(), IDeviceSessionManagement::Result::Success);
        // clang-format on
        QVERIFY(testing::Mock::VerifyAndClearExpectations(&storage));
    }

    void testSessionInfosAreSorted()
    {
        auto fakeLaptimer = TestHelper::FakeLaptimer{};
        auto deserializer = TestHelper::SessionJsonDeserializerMock{};
        auto storage = TestHelper::SessionStorageMock{};
        auto rdsm = RestDeviceSessionManagement{&deserializer, &storage};
        auto settings = fakeLaptimer.deviceSettings();

        fetchSessionInfos(rdsm, settings);

        auto* const model = rdsm.getDeviceSessionInfoListModel();

        QCOMPARE(model->rowCount(), 2);
        auto index = model->index(0, 0);
        auto role = 257; // SessionListModel::Laptimer
        QCOMPARE(model->data(index, role).value<Common::SessionInfo>().id, fakeLaptimer.getSessionInfo2().id);
        index = model->index(1, 0);
        QCOMPARE(model->data(index, role).value<Common::SessionInfo>().id, fakeLaptimer.getSessionInfo1().id);
    }
};

} // namespace RapidAndroid::Workflow::Tests

QTEST_MAIN(RapidAndroid::Workflow::Tests::HttpDeviceSessionManagementTest)
#include "HttpDeviceSessionManagementTest.moc"
