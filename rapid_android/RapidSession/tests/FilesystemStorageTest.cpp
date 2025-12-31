// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <Common/Session.hpp>
#include <QObject>
#include <QTemporaryDir>
#include <QTest>
#include <RapidSession/FilesystemStorage.hpp>
#include <TestHelper/Session.hpp>
#include <TestHelper/SessionJsonSerializerMock.hpp>

namespace RapidAndroid::Session::Test
{

// using FilesystemJsonStorage = RapidAndroid::Session::FilesystemStorage<TestHelper::SessionJsonSerializerMock>;

class FilesystemStorageTest : public QObject
{
    Q_OBJECT
public:
    Q_DISABLE_COPY_MOVE(FilesystemStorageTest)

    FilesystemStorageTest() = default;

    ~FilesystemStorageTest() override = default;

private Q_SLOTS:
    void testStoreSession()
    {
        QString const sessionFileName = "oschersleben_01_01_1970_13_00_00_000.session";
        QString const sessionInfoFileName = "oschersleben_01_01_1970_13_00_00_000.info";
        auto testDir = QTemporaryDir{};
        auto filePath = std::filesystem::path{testDir.path().toUtf8().constData()};
        auto serializer = TestHelper::SessionJsonSerializerMock{};
        auto storage =
            RapidAndroid::Session::FilesystemStorage<TestHelper::SessionJsonSerializerMock>{filePath, &serializer};
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
};

} // namespace RapidAndroid::Session::Test

QTEST_MAIN(RapidAndroid::Session::Test::FilesystemStorageTest)
#include "FilesystemStorageTest.moc"
