// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <Common/Lap.hpp>
#include <QTest>

namespace RapidAndroid::Common::Tests
{
class LapTest : public QObject
{
    Q_OBJECT
public:
    Q_DISABLE_COPY_MOVE(LapTest)
    LapTest() = default;
    ~LapTest() override = default;
private Q_SLOTS:
    void testLaptime()
    {
        Lap lap;
        lap.sectors = {QTime(0, 1, 30, 123), QTime(0, 1, 45, 456), QTime(0, 1, 40, 789)}; // Example sector times
        QCOMPARE(lap.laptime(), QTime(0, 4, 56, 368)); // Expected total lap time
    }
};

} // namespace RapidAndroid::Common::Tests

QTEST_MAIN(RapidAndroid::Common::Tests::LapTest)
#include "LapTest.moc"
