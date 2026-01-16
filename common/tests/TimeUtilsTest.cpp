// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Common/TimeUtils.hpp"
#include <QObject>
#include <QTest>

namespace RapidAndroid::Common::Tests
{

class TimeUtilsTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testDurationBetween_data()
    {
        QTest::addColumn<QTime>("start");
        QTest::addColumn<QTime>("end");
        QTest::addColumn<QTime>("expectedDuration");

        QTest::newRow("same time") << QTime(10, 0, 0) << QTime(10, 0, 0) << QTime(0, 0, 0);
        QTest::newRow("positive duration") << QTime(10, 0, 0) << QTime(11, 30, 0) << QTime(1, 30, 0);
        QTest::newRow("negative duration") << QTime(11, 30, 0) << QTime(10, 0, 0) << QTime(1, 30, 0);
        QTest::newRow("crossing midnight") << QTime(23, 0, 0) << QTime(1, 0, 0) << QTime(2, 0, 0);
    }
    void testDurationBetween()
    {
        QFETCH(QTime, start);
        QFETCH(QTime, end);
        QFETCH(QTime, expectedDuration);

        QTime result = TimeUtils::durationBetween(start, end);
        QCOMPARE(result, expectedDuration);
    }
};

} // namespace RapidAndroid::Common::Tests
QTEST_MAIN(RapidAndroid::Common::Tests::TimeUtilsTest)
#include "TimeUtilsTest.moc"
