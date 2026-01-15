// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "RapidLiveSession/LiveSessionJsonDeserializer.hpp"
#include <QObject>
#include <QTest>
#include <TestHelper/LaptimerEvents.hpp>

namespace RapidAndroid::RapidLiveSession::Tests
{

class LiveSessionJsonDeserializerTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testDeserializeLaptimeEvent()
    {
        auto deserializer = LiveSessionJsonDeserializer{};
        auto time = QTime{0, 0, 12, 345};
        auto task = deserializer.deserialize(TestHelper::laptimeEventJson(time).toUtf8());
        task.waitForFinished();
        auto eventOpt = task.result();
        QVERIFY(eventOpt.has_value());
        auto event = eventOpt.value_or(Event{});
        QVERIFY(std::holds_alternative<Common::LaptimeEvent>(event));
        QCOMPARE(std::get<Common::LaptimeEvent>(event).laptime, time);
    }

    void testDeserializeLapFinishedEvent()
    {
        auto deserializer = LiveSessionJsonDeserializer{};
        auto time = QTime{0, 1, 23, 456};
        auto task = deserializer.deserialize(TestHelper::lapFinishedEventJson(time).toUtf8());
        task.waitForFinished();
        auto eventOpt = task.result();
        QVERIFY(eventOpt.has_value());
        auto event = eventOpt.value_or(Event{});
        QVERIFY(std::holds_alternative<Common::LapFinishedEvent>(event));
        QCOMPARE(std::get<Common::LapFinishedEvent>(event).laptime, time);
    }
};

} // namespace RapidAndroid::RapidLiveSession::Tests
QTEST_MAIN(RapidAndroid::RapidLiveSession::Tests::LiveSessionJsonDeserializerTest)
#include "LiveSessionJsonDeserializerTest.moc"
