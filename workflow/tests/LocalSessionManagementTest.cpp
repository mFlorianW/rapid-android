// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QSignalSpy>
#include <QTest>
#include <QtConcurrentRun>
#include <TestHelper/Session.hpp>
#include <TestHelper/SessionJsonDeserializerMock.hpp>
#include <TestHelper/SessionJsonSerializerMock.hpp>
#include <TestHelper/SessionStorageMock.hpp>
#include <Workflow/LocalSessionManagement.hpp>

namespace RapidAndroid::Workflow::Tests
{

class LocalSessionManagementTest : public QObject
{
    Q_OBJECT
    using Lsm = LocalSessionManagement<TestHelper::SessionStorageMock>;

public:
    Q_DISABLE_COPY_MOVE(LocalSessionManagementTest)
    LocalSessionManagementTest() = default;
    ~LocalSessionManagementTest() override = default;

    void refreshSessionInfoList(ILocalSessionManagement& lsm)
    {
        auto spy = QSignalSpy{&lsm, &ILocalSessionManagement::sessionInfoListRefreshed};
        lsm.refreshSessionInfos();

        QTRY_COMPARE_GE_WITH_TIMEOUT(spy.size(), 1, std::chrono::seconds(1));
        QCOMPARE(lsm.getSessionInfoListModel()->rowCount(), 1);
    }

private Q_SLOTS:

    void testModelIsNotNull()
    {
        auto storage = TestHelper::SessionStorageMock{};
        auto lsm = Lsm{&storage};

        QCOMPARE_NE(lsm.getSessionInfoListModel(), nullptr);
        QCOMPARE_NE(lsm.property("model").value<QAbstractListModel*>(), nullptr);
    }

    void testLoadSessionInfos()
    {
        auto storage = TestHelper::SessionStorageMock{};
        auto lsm = Lsm{&storage};
        auto spy = QSignalSpy{&lsm, &ILocalSessionManagement::sessionInfoListRefreshed};

        EXPECT_CALL(storage, getSessionInfos()).Times(1).WillRepeatedly(testing::Return(QtConcurrent::run([]() {
            QVector<Common::SessionInfo> infos{TestHelper::getOscherslebenSessionInfo()};
            return infos;
        })));

        QCOMPARE(lsm.getSessionInfoListModel()->rowCount(), 0);
        refreshSessionInfoList(lsm);
        auto index = lsm.getSessionInfoListModel()->index(0, 0);
        auto sessionInfo =
            lsm.getSessionInfoListModel()->data(index, SessionListModel::SessionInfo).value<Common::SessionInfo>();
        QCOMPARE(sessionInfo, TestHelper::getOscherslebenSessionInfo());
        QVERIFY(testing::Mock::VerifyAndClearExpectations(&storage));
    }

    void testLoadSessionInfosMultipleTimes()
    {
        auto storage = TestHelper::SessionStorageMock{};
        auto lsm = Lsm{&storage};
        auto spy = QSignalSpy{&lsm, &ILocalSessionManagement::sessionInfoListRefreshed};

        EXPECT_CALL(storage, getSessionInfos()).WillRepeatedly(testing::Return(QtConcurrent::run([]() {
            QVector<Common::SessionInfo> infos{TestHelper::getOscherslebenSessionInfo()};
            return infos;
        })));

        QCOMPARE(lsm.getSessionInfoListModel()->rowCount(), 0);
        refreshSessionInfoList(lsm);
        refreshSessionInfoList(lsm);
        refreshSessionInfoList(lsm);
        QTRY_COMPARE_WITH_TIMEOUT(spy.size(), 3, std::chrono::seconds(1));
        QCOMPARE(lsm.getSessionInfoListModel()->rowCount(), 1);
        QVERIFY(testing::Mock::VerifyAndClearExpectations(&storage));
    }

    void testRemoveSession()
    {
        auto storage = TestHelper::SessionStorageMock{};
        auto lsm = Lsm{&storage};
        auto spy = QSignalSpy{&lsm, &ILocalSessionManagement::sessionRemoved};

        EXPECT_CALL(storage, remove(TestHelper::getOscherslebenSessionInfo()))
            .Times(1)
            .WillOnce(testing::Return(QtConcurrent::run([]() {
                return true;
            })));

        EXPECT_CALL(storage, getSessionInfos()).WillRepeatedly(testing::Return(QtConcurrent::run([]() {
            QVector<Common::SessionInfo> infos{TestHelper::getOscherslebenSessionInfo()};
            return infos;
        })));

        refreshSessionInfoList(lsm);
        lsm.remove(TestHelper::getOscherslebenSessionInfo());
        QTRY_COMPARE_WITH_TIMEOUT(spy.size(), 1, std::chrono::seconds(1));
        QCOMPARE(lsm.getSessionInfoListModel()->rowCount(), 0);
        QVERIFY(testing::Mock::VerifyAndClearExpectations(&storage));
    }

    void testLoadSession()
    {
        auto storage = TestHelper::SessionStorageMock{};
        auto lsm = Lsm{&storage};
        auto spy = QSignalSpy{&lsm, &ILocalSessionManagement::sessionLoaded};

        EXPECT_CALL(storage, load(TestHelper::getOscherslebenSessionInfo()))
            .Times(1)
            .WillOnce(testing::Return(QtConcurrent::run([]() -> std::optional<Common::Session> {
                return TestHelper::getOscherslebenSession();
            })));

        lsm.load(TestHelper::getOscherslebenSessionInfo());
        QTRY_COMPARE_WITH_TIMEOUT(spy.size(), 1, std::chrono::seconds(1));
        QCOMPARE(spy.at(0).at(0).toBool(), true);
        QCOMPARE(spy.at(0).at(1).value<Common::SessionInfo>(), TestHelper::getOscherslebenSessionInfo());
        QCOMPARE(spy.at(0).at(2).value<Common::Session>(), TestHelper::getOscherslebenSession());
        QVERIFY(testing::Mock::VerifyAndClearExpectations(&storage));
    }
};

} // namespace RapidAndroid::Workflow::Tests

QTEST_MAIN(RapidAndroid::Workflow::Tests::LocalSessionManagementTest)
#include "LocalSessionManagementTest.moc"
