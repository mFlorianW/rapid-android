// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QObject>
#include <QSignalSpy>
#include <QTest>
#include <TestHelper/LiveSessionEventSourceMock.hpp>
#include <TestHelper/Session.hpp>
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

    void sendCurrentSessionEvent()
    {
        auto currentSessionEvent = RapidAndroid::Common::CurrentSessionEvent{
            .session = std::make_unique<RapidAndroid::Common::Session>(TestHelper::getOscherslebenSession())};
        Q_EMIT mLses->currentSessionEventReceived(currentSessionEvent);
    }

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

        sendCurrentSessionEvent();
        currentLaptimeSpy.clear();
        Q_EMIT mLses->currentLaptimeChanged(RapidAndroid::Common::LaptimeEvent{.laptime = laptime});

        QCOMPARE(currentLaptimeSpy.count(), 1);
        QCOMPARE(mLsm->property("currentLaptime").value<QTime>(), laptime);
    }

    void testLapCountIncrement()
    {
        auto constexpr defaultLapCount = 0;
        auto constexpr lapCount = 2;
        auto const laptime = QTime{0, 1, 30, 500};
        auto lapCountSpy = QSignalSpy{mLsm.get(), &Lsm::lapCountChanged};

        QCOMPARE(mLsm->getLapCount(), defaultLapCount);

        sendCurrentSessionEvent();
        lapCountSpy.clear();
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

        sendCurrentSessionEvent();
        lastLaptimeSpy.clear();
        Q_EMIT mLses->laptimeFinished(Common::LapFinishedEvent{.laptime = laptime});

        QCOMPARE(lastLaptimeSpy.count(), 1);
        QCOMPARE(mLsm->property("lastLaptime").value<QTime>(), laptime);
    }

    void testBestLaptimeUpdate()
    {
        auto const defaultLaptime = QTime{0, 0, 0, 0};
        auto const laptime = QTime{0, 1, 30, 500};

        QCOMPARE(mLsm->property("bestLaptime").value<QTime>(), defaultLaptime);

        sendCurrentSessionEvent();
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

        sendCurrentSessionEvent();
        bestLaptimeDiffSpy.clear();
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
        auto const expectedAverage = QTime{0, 1, 38, 930};

        sendCurrentSessionEvent();
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
        constexpr auto const expectedBestLap = 3;

        sendCurrentSessionEvent();
        Q_EMIT mLses->laptimeFinished(Common::LapFinishedEvent{.laptime = laptime1});
        Q_EMIT mLses->laptimeFinished(Common::LapFinishedEvent{.laptime = laptime2});
        Q_EMIT mLses->laptimeFinished(Common::LapFinishedEvent{.laptime = laptime3});

        QCOMPARE(mLsm->property("bestLaptimeLap").toUInt(), expectedBestLap);
    }

    void testOnlySendUpdatesAfterSync()
    {
        auto const laptime = QTime{0, 2, 5, 720};
        auto const defaultLaptime = QTime{0, 0, 0, 0};
        auto lastLaptimeSpy = QSignalSpy{mLsm.get(), &Lsm::lastLaptimeChanged};
        constexpr auto defaultLapCount = 0u;
        auto lapCountSpy = QSignalSpy{mLsm.get(), &Lsm::lapCountChanged};
        auto bestLaptimeSpy = QSignalSpy{mLsm.get(), &Lsm::bestLaptimeChanged};
        auto averageDurationSpy = QSignalSpy{mLsm.get(), &Lsm::averageLaptimeChanged};
        auto trackNameSpy = QSignalSpy{mLsm.get(), &Lsm::trackNameChanged};

        QCOMPARE(mLsm->property("lastLaptime").value<QTime>(), defaultLaptime);

        // these emits should not trigger any updates yet because no current session event has been received
        Q_EMIT mLses->currentLaptimeChanged(RapidAndroid::Common::LaptimeEvent{.laptime = laptime});
        Q_EMIT mLses->laptimeFinished(Common::LapFinishedEvent{.laptime = laptime});

        QCOMPARE(lastLaptimeSpy.count(), 0);
        QCOMPARE(mLsm->property("lastLaptime").value<QTime>(), defaultLaptime);
        QCOMPARE(lapCountSpy.count(), 0);
        QCOMPARE(mLsm->property("lapCount").toUInt(), defaultLapCount);
        QCOMPARE(bestLaptimeSpy.count(), 0);
        QCOMPARE(mLsm->property("bestLaptime").value<QTime>(), defaultLaptime);
        QCOMPARE(averageDurationSpy.count(), 0);
        QCOMPARE(mLsm->property("averageLaptime").value<QTime>(), defaultLaptime);
        QCOMPARE(trackNameSpy.count(), 0);
        QCOMPARE(mLsm->property("trackName").toString(), QString{"Unknown Track"});

        // sending the current session event should trigger processing of the live session parameter
        sendCurrentSessionEvent();

        QCOMPARE(lastLaptimeSpy.count(), 1);
        QCOMPARE(mLsm->property("lastLaptime").value<QTime>(), laptime);
        QCOMPARE(lapCountSpy.count(), 1);
        QCOMPARE(mLsm->property("lapCount").toUInt(), 1);
        QCOMPARE(bestLaptimeSpy.count(), 1);
        QCOMPARE(mLsm->property("bestLaptime").value<QTime>(), laptime);
        QCOMPARE(averageDurationSpy.count(), 1);
        QCOMPARE(mLsm->property("averageLaptime").value<QTime>(), laptime);
        QCOMPARE(trackNameSpy.count(), 1);
        QCOMPARE(mLsm->property("trackName").toString(), QString{"Oschersleben"});
    }
};

} // namespace RapidAndroid::Workflow::Tests

QTEST_MAIN(RapidAndroid::Workflow::Tests::LiveSessionManagementTest)
#include "LiveSessionManagementTest.moc"
