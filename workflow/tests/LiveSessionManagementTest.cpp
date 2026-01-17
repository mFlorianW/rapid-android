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
        auto lapCountSpy = QSignalSpy{mLsm.get(), &Lsm::lapCountChanged};

        QCOMPARE(mLsm->getLapCount(), defaultLapCount);

        Q_EMIT mLses->laptimeFinished(Common::LapFinishedEvent{.laptime = laptime});

        QCOMPARE(lapCountSpy.count(), 1);
        QCOMPARE(mLsm->property("lapCount").toUInt(), lapCount);
    }

    void testLastLaptimeUpdate()
    {
        auto const laptime = QTime{0, 1, 30, 500};
        auto const defaultLaptime = QTime{0, 0, 0, 0};
        auto lastLaptimeSpy = QSignalSpy{mLsm.get(), &Lsm::lastLaptimeChanged};

        QCOMPARE(mLsm->property("lastLaptime").value<QTime>(), defaultLaptime);

        Q_EMIT mLses->laptimeFinished(Common::LapFinishedEvent{.laptime = laptime});

        QCOMPARE(lastLaptimeSpy.count(), 1);
        QCOMPARE(mLsm->property("lastLaptime").value<QTime>(), laptime);
    }

    void testBestLaptimeUpdate()
    {
        auto const defaultLaptime = QTime{0, 0, 0, 0};
        auto const laptime = QTime{0, 1, 30, 500};

        QCOMPARE(mLsm->property("bestLaptime").value<QTime>(), defaultLaptime);

        Q_EMIT mLses->laptimeFinished(Common::LapFinishedEvent{.laptime = laptime});
        QCOMPARE(mLsm->property("bestLaptime").value<QTime>(), laptime);

        auto const betterLaptime = QTime{0, 1, 20, 300};
        Q_EMIT mLses->laptimeFinished(Common::LapFinishedEvent{.laptime = betterLaptime});
        QCOMPARE(mLsm->property("bestLaptime").value<QTime>(), betterLaptime);
    }

    void testBestLaptimeDiffUpdate()
    {
        auto const defaultDiff = QTime{0, 0, 0, 0};
        auto const bestLaptime = QTime{0, 1, 20, 300};
        auto const currentLaptime = QTime{0, 1, 30, 500};
        auto const expectedDiff = QTime{0, 0, 10, 200};
        auto bestLaptimeDiffSpy = QSignalSpy{mLsm.get(), &Lsm::bestLaptimeDiffChanged};

        QCOMPARE(mLsm->property("bestLaptimeDiff").value<QTime>(), defaultDiff);

        Q_EMIT mLses->laptimeFinished(Common::LapFinishedEvent{.laptime = bestLaptime});
        QCOMPARE(bestLaptimeDiffSpy.count(), 1);
        QCOMPARE(mLsm->property("bestLaptimeDiff").value<QTime>(), defaultDiff);

        Q_EMIT mLses->laptimeFinished(Common::LapFinishedEvent{.laptime = currentLaptime});
        QCOMPARE(bestLaptimeDiffSpy.count(), 2);
        QCOMPARE(mLsm->property("bestLaptimeDiff").value<QTime>(), expectedDiff);
    }

    void testAverageLaptime()
    {
        auto const laptime1 = QTime{0, 1, 30, 0};
        auto const laptime2 = QTime{0, 1, 0, 0};
        auto const laptime3 = QTime{0, 2, 0, 0};
        auto const expectedAverage = QTime{0, 1, 30, 0};

        Q_EMIT mLses->laptimeFinished(Common::LapFinishedEvent{.laptime = laptime1});
        Q_EMIT mLses->laptimeFinished(Common::LapFinishedEvent{.laptime = laptime2});
        Q_EMIT mLses->laptimeFinished(Common::LapFinishedEvent{.laptime = laptime3});

        QCOMPARE(mLsm->property("averageLaptime").value<QTime>(), expectedAverage);
    }

    void testBestLaptimeLapUpdate()
    {
        auto const laptime1 = QTime{0, 1, 30, 0};
        auto const laptime2 = QTime{0, 1, 0, 0};
        auto const laptime3 = QTime{0, 2, 0, 0};
        auto const expectedBestLap = 2; // laptime2 is the best

        Q_EMIT mLses->laptimeFinished(Common::LapFinishedEvent{.laptime = laptime1});
        Q_EMIT mLses->laptimeFinished(Common::LapFinishedEvent{.laptime = laptime2});
        Q_EMIT mLses->laptimeFinished(Common::LapFinishedEvent{.laptime = laptime3});

        QCOMPARE(mLsm->property("bestLaptimeLap").toUInt(), expectedBestLap);
    }
};

} // namespace RapidAndroid::Workflow::Tests

QTEST_MAIN(RapidAndroid::Workflow::Tests::LiveSessionManagementTest)
#include "LiveSessionManagementTest.moc"
