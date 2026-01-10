// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <Common/Session.hpp>
#include <QObject>
#include <QTemporaryDir>
#include <QTest>
#include <RapidSession/FilesystemStorage.hpp>
#include <TestHelper/Session.hpp>
#include <TestHelper/SessionJsonDeserializerMock.hpp>
#include <TestHelper/SessionJsonSerializerMock.hpp>

namespace RapidAndroid::Session::Test
{

using FilesystemJsonStorage = RapidAndroid::Session::FilesystemStorage<TestHelper::SessionJsonSerializerMock,
                                                                       TestHelper::SessionJsonDeserializerMock>;

class FilesystemStorageTest : public QObject
{
    Q_OBJECT
public:
    Q_DISABLE_COPY_MOVE(FilesystemStorageTest)

    FilesystemStorageTest() = default;

    ~FilesystemStorageTest() override = default;

    void createTestFiles(std::filesystem::path const& filePath)
    {
        QString const sessionInfoFileName = "oschersleben_01_01_1970_13_00_00_000.info";
        auto sessionInfoFile = QString::fromStdString(filePath / sessionInfoFileName.toUtf8().constData());
        auto infoFileContent = QFile{sessionInfoFile};
        QVERIFY(infoFileContent.open(QIODevice::WriteOnly | QIODevice::Text));
        if (infoFileContent.write(TestHelper::getJsonOscherslebenSessionInfo().toJson()) < sessionInfoFile.size()) {
            QFAIL("Failed to write test session info file");
        }

        QString const sessionFileName = "oschersleben_01_01_1970_13_00_00_000.session";
        auto sessionFile = QString::fromStdString(filePath / sessionFileName.toUtf8().constData());
        auto fileContent = QFile{sessionFile};
        QVERIFY(fileContent.open(QIODevice::WriteOnly | QIODevice::Text));
        if (fileContent.write(TestHelper::getJsonOscherslebenSession().toJson()) < sessionFile.size()) {
            QFAIL("Failed to write test session file");
        }
    }

private Q_SLOTS:
    void testStoreSession()
    {
        QString const sessionFileName = "oschersleben_01_01_1970_13_00_00_000.session";
        QString const sessionInfoFileName = "oschersleben_01_01_1970_13_00_00_000.info";
        auto testDir = QTemporaryDir{};
        auto filePath = std::filesystem::path{testDir.path().toUtf8().constData()};
        auto serializer = TestHelper::SessionJsonSerializerMock{};
        auto deserializer = TestHelper::SessionJsonDeserializerMock{};
        auto storage = FilesystemJsonStorage{filePath, &serializer, &deserializer};
        auto session = std::make_unique<Common::Session>(TestHelper::getOscherslebenSession());

        EXPECT_CALL(serializer, serialize(testing::_))
            .Times(1)
            .WillOnce(testing::Return(QtConcurrent::run([]() -> Workflow::SerializeResult {
                return Workflow::SerializeResult{
                    std::make_unique<Common::Session>(TestHelper::getOscherslebenSession()),
                    TestHelper::getJsonOscherslebenSession().toJson()};
            })));

        auto fut = storage.store(std::move(session));
        fut.waitForFinished();
        QVERIFY(fut.takeResult().success);
        auto const sessionFile = QString::fromStdString(filePath / sessionFileName.toUtf8().constData());
        auto fileContent = QFile{sessionFile};
        QVERIFY(fileContent.open(QIODevice::ReadOnly | QIODevice::Text));
        auto const storedData = fileContent.readAll();
        QCOMPARE(storedData, TestHelper::getJsonOscherslebenSession().toJson());
        auto sessionInfoFile = QString::fromStdString(filePath / sessionInfoFileName.toUtf8().constData());
        auto infoFileContent = QFile{sessionInfoFile};
        QVERIFY(infoFileContent.open(QIODevice::ReadOnly | QIODevice::Text));
        auto const storedInfoData = infoFileContent.readAll();
        QCOMPARE(storedInfoData, TestHelper::getJsonOscherslebenSessionInfo().toJson());
        QVERIFY(testing::Mock::VerifyAndClearExpectations(&serializer));
    };

    void testGetSessionInfos()
    {
        auto testDir = QTemporaryDir{};
        auto filePath = std::filesystem::path{testDir.path().toUtf8().constData()};
        auto serializer = TestHelper::SessionJsonSerializerMock{};
        auto deserializer = TestHelper::SessionJsonDeserializerMock{};
        auto storage = FilesystemJsonStorage{filePath, &serializer, &deserializer};
        createTestFiles(filePath);

        EXPECT_CALL(deserializer, deserializeInfo(testing::_))
            .Times(1)
            .WillOnce(testing::Return(QtConcurrent::run([]() -> std::optional<std::unique_ptr<Common::SessionInfo>> {
                return std::make_unique<Common::SessionInfo>(TestHelper::getOscherslebenSessionInfo());
            })));

        auto fut = storage.getSessionInfos();
        fut.waitForFinished();
        auto const infos = fut.result();
        QCOMPARE(infos.size(), 1);
        QCOMPARE(infos.at(0), TestHelper::getOscherslebenSessionInfo());
        QVERIFY(testing::Mock::VerifyAndClearExpectations(&deserializer));
    };

    void testRemoveSession()
    {
        QString const sessionFileName = "oschersleben_01_01_1970_13_00_00_000.session";
        QString const sessionInfoFileName = "oschersleben_01_01_1970_13_00_00_000.info";
        auto testDir = QTemporaryDir{};
        auto filePath = std::filesystem::path{testDir.path().toUtf8().constData()};
        auto serializer = TestHelper::SessionJsonSerializerMock{};
        auto deserializer = TestHelper::SessionJsonDeserializerMock{};
        auto storage = FilesystemJsonStorage{filePath, &serializer, &deserializer};
        createTestFiles(filePath);
        auto sessionInfo = TestHelper::getOscherslebenSessionInfo();

        auto fut = storage.remove(sessionInfo);
        fut.waitForFinished();
        QVERIFY(fut.takeResult());
        auto const sessionFile = QString::fromStdString(filePath / sessionFileName.toUtf8().constData());
        QVERIFY(!QFile::exists(sessionFile));
        auto const sessionInfoFile = QString::fromStdString(filePath / sessionInfoFileName.toUtf8().constData());
        QVERIFY(!QFile::exists(sessionInfoFile));
    };

    void testLoadSession()
    {
        auto testDir = QTemporaryDir{};
        auto filePath = std::filesystem::path{testDir.path().toUtf8().constData()};
        auto serializer = TestHelper::SessionJsonSerializerMock{};
        auto deserializer = TestHelper::SessionJsonDeserializerMock{};
        auto storage = FilesystemJsonStorage{filePath, &serializer, &deserializer};
        createTestFiles(filePath);

        EXPECT_CALL(deserializer, deserialize(testing::_))
            .Times(1)
            .WillOnce(testing::Return(QtConcurrent::run([]() -> std::optional<std::unique_ptr<Common::Session>> {
                return std::make_unique<Common::Session>(TestHelper::getOscherslebenSession());
            })));

        auto fut = storage.load(TestHelper::getOscherslebenSessionInfo());
        fut.waitForFinished();
        auto const sessionOpt = fut.takeResult();
        QVERIFY(sessionOpt.has_value());
        QCOMPARE(sessionOpt.value(), // NOLINT(bugprone-unchecked-optional-access)
                 TestHelper::getOscherslebenSession());
        QVERIFY(testing::Mock::VerifyAndClearExpectations(&deserializer));
    };
};

} // namespace RapidAndroid::Session::Test

QTEST_MAIN(RapidAndroid::Session::Test::FilesystemStorageTest)
#include "FilesystemStorageTest.moc"
