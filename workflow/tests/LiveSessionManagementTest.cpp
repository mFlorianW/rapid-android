// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QObject>
#include <QSignalSpy>
#include <QTest>
#include <TestHelper/LiveSessionEventSourceMock.hpp>
#include <Workflow/LiveSessionManagement.hpp>

namespace RapidAndroid::Workflow::Tests
{
using Lsm = RapidAndroid::Workflow::LiveSessionManagement<RapidAndroid::TestHelper::LiveSessionEventSourceMock>;

class LiveSessionManagementTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testCurrentLaptimeUpdates()
    {
        auto lses = RapidAndroid::TestHelper::LiveSessionEventSourceMock{};
        auto lsm = Lsm{&lses};
        QSignalSpy currentLaptimeSpy(&lsm, &Lsm::currentLaptimeChanged);
        auto const laptime = QTime{0, 1, 30, 500};
        auto const defaultLaptime = QTime{0, 0, 0, 0};

        QCOMPARE(lsm.getCurrentLaptime(), defaultLaptime);

        Q_EMIT lses.currentLaptimeChanged(RapidAndroid::Common::LaptimeEvent{.laptime = laptime});

        QCOMPARE(currentLaptimeSpy.count(), 1);
        QCOMPARE(lsm.getCurrentLaptime(), laptime);
    }
};

} // namespace RapidAndroid::Workflow::Tests

QTEST_MAIN(RapidAndroid::Workflow::Tests::LiveSessionManagementTest)
#include "LiveSessionManagementTest.moc"
