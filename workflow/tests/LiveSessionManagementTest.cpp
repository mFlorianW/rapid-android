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

private:
    std::unique_ptr<RapidAndroid::TestHelper::LiveSessionEventSourceMock> mLses;
    std::unique_ptr<Lsm> mLsm;

private Q_SLOTS:
    void init()
    {
        mLses = std::make_unique<TestHelper::LiveSessionEventSourceMock>();
        mLsm = std::make_unique<Lsm>(mLses.get());
    }

    void testCurrentLaptimeUpdates()
    {
        QSignalSpy currentLaptimeSpy(mLsm.get(), &Lsm::currentLaptimeChanged);
        auto const laptime = QTime{0, 1, 30, 500};
        auto const defaultLaptime = QTime{0, 0, 0, 0};

        QCOMPARE(mLsm->getCurrentLaptime(), defaultLaptime);

        Q_EMIT mLses->currentLaptimeChanged(RapidAndroid::Common::LaptimeEvent{.laptime = laptime});

        QCOMPARE(currentLaptimeSpy.count(), 1);
        QCOMPARE(mLsm->property("currentLaptime").value<QTime>(), laptime);
    }

    void testLapCountIncrement()
    {
        auto constexpr defaultLapCount = 0;
        auto constexpr lapCount = 1;
        auto const laptime = QTime{0, 1, 30, 500};
        QSignalSpy lapCountSpy(mLsm.get(), &Lsm::lapCountChanged);

        QCOMPARE(mLsm->getLapCount(), defaultLapCount);

        Q_EMIT mLses->laptimeFinished(Common::LapFinishedEvent{.laptime = laptime});

        QCOMPARE(lapCountSpy.count(), 1);
        QCOMPARE(mLsm->property("lapCount").toUInt(), lapCount);
    }
};

} // namespace RapidAndroid::Workflow::Tests

QTEST_MAIN(RapidAndroid::Workflow::Tests::LiveSessionManagementTest)
#include "LiveSessionManagementTest.moc"
