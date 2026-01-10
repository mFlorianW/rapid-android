// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QObject>
#include <QTest>
#include <RapidSession/SessionJsonSerializer.hpp>
#include <TestHelper/Session.hpp>

namespace RapidAndroid::Session::Test
{
class SessionJsonSerializerTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testSerialize()
    {
        Common::Session session = TestHelper::getOscherslebenSession();
        SessionJsonSerializer serializer;
        auto future = serializer.serialize(std::make_unique<Common::Session>(session));
        future.waitForFinished();
        auto serializeResult = future.takeResult();
        QVERIFY(serializeResult.data.has_value());
        QCOMPARE(QJsonDocument::fromJson(serializeResult.data.value()), // NOLINT(bugprone-unchecked-optional-access)
                 TestHelper::getJsonOscherslebenSession());
        QCOMPARE(*serializeResult.session, session);
    }

    void testSerializeSessionInfo()
    {
        Common::Session session = TestHelper::getOscherslebenSession();
        SessionJsonSerializer serializer;
        auto future = serializer.serializeInfo(std::make_unique<Common::Session>(session),
                                               QStringLiteral("oschersleben_01_01_1970_13_00_00_000"));
        future.waitForFinished();
        auto serializeResult = future.takeResult();
        QVERIFY(serializeResult.data.has_value());
        QCOMPARE(QJsonDocument::fromJson(serializeResult.data.value()), // NOLINT(bugprone-unchecked-optional-access)
                 TestHelper::getJsonOscherslebenSessionInfo());
        QCOMPARE(*serializeResult.session, session);
    }
};

} // namespace RapidAndroid::Session::Test

QTEST_MAIN(RapidAndroid::Session::Test::SessionJsonSerializerTest)
#include "SessionJsonSerializerTest.moc"
