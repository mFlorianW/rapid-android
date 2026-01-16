// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Common/TimeFormatter.hpp"
#include <QTest>

namespace RapidAndroid::Common::Tests
{
class TimeFormatterTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testFormatTime_data()
    {
        QTest::addColumn<QTime>("inputTime");
        QTest::addColumn<QString>("expectedOutput");

        QTest::newRow("full time") << QTime(1, 2, 3, 456) << QString("01:02:03.456");
        QTest::newRow("minutes and seconds") << QTime(0, 1, 23, 456) << QString("01:23.456");
        QTest::newRow("seconds only") << QTime(0, 0, 45, 678) << QString("45.678");
        QTest::newRow("milliseconds only") << QTime(0, 0, 0, 123) << QString("00.123");
        QTest::newRow("zero time") << QTime(0, 0, 0, 0) << QString("00:00.000");
    }

    void testFormatTime()
    {
        QFETCH(QTime, inputTime);
        QFETCH(QString, expectedOutput);

        QString formattedTime = Common::TimeFormatter::formatTime(inputTime);
        QCOMPARE(formattedTime, expectedOutput);
    }
};

} // namespace RapidAndroid::Common::Tests
QTEST_MAIN(RapidAndroid::Common::Tests::TimeFormatterTest)
#include "TimeFormatterTest.moc"
