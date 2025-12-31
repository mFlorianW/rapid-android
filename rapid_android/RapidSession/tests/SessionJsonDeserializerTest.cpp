// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QObject>
#include <QTest>
#include <RapidSession/SessionJsonDeserializer.hpp>
#include <TestHelper/Session.hpp>

namespace RapidAndroid::Session::Test
{

class SessionJsonDeserializerTest : public QObject
{
    Q_OBJECT
public:
    Q_DISABLE_COPY_MOVE(SessionJsonDeserializerTest)
    SessionJsonDeserializerTest() = default;
    ~SessionJsonDeserializerTest() override = default;

private Q_SLOTS:
    void testDeserialize()
    {
        auto const jsonData = TestHelper::getJsonOscherslebenSession().toJson();
        auto deserializedSessionFuture = SessionJsonDeserializer::deserialize(jsonData);
        deserializedSessionFuture.waitForFinished();
        auto const deserializedSessionOpt = deserializedSessionFuture.takeResult();
        QVERIFY(deserializedSessionOpt.has_value());
        QCOMPARE(**deserializedSessionOpt, // NOLINT(bugprone-unchecked-optional-access)
                 TestHelper::getOscherslebenSession());
    }
};

} // namespace RapidAndroid::Session::Test

QTEST_MAIN(RapidAndroid::Session::Test::SessionJsonDeserializerTest)
#include "SessionJsonDeserializerTest.moc"
