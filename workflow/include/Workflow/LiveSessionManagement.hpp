// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_WORKFLOW_LIVESESSIONMANAGEMENT_HPP
#define RAPIDANDROID_WORKFLOW_LIVESESSIONMANAGEMENT_HPP

#include <Common/TimeUtils.hpp>
#include <Workflow/ILiveSessionEventSource.hpp>
#include <Workflow/ILiveSessionManagement.hpp>

namespace RapidAndroid::Workflow
{

template <typename EventSourceType>
    requires LiveSessionEventSourceConcept<EventSourceType>
class LiveSessionManagement : public ILiveSessionManagement
{
public:
    Q_DISABLE_COPY_MOVE(LiveSessionManagement)

    /**
     * @brief Constructor.
     *
     * @param eventSource Pointer to the event source providing live session events.
     */
    LiveSessionManagement(EventSourceType* eventSource) noexcept
        : mEventSource{std::move(eventSource)}
    {
        Q_ASSERT(mEventSource != nullptr);
        connect(mEventSource,
                &EventSourceType::currentSessionEventReceived,
                this,
                &LiveSessionManagement::onCurrentSessionReceived);
    }

    /**
     * @brief Destructor.
     */
    ~LiveSessionManagement() override = default;

    /**
     * @copydoc ILiveSessionManagement::currentLaptime
     */
    [[nodiscard]] QTime getCurrentLaptime() const noexcept override
    {
        return mCurrentLaptime;
    }

    /**
     * @copydoc ILiveSessionManagement::lastLaptime
     */
    [[nodiscard]] QTime getLastLaptime() const noexcept override
    {
        return mLastLaptime;
    }

    /**
     * @copydoc ILiveSessionManagement::bestLaptime
     */
    [[nodiscard]] QTime getBestLaptime() const noexcept override
    {
        return mBestLaptime;
    }

    /**
     * @copydoc ILiveSessionManagement::getBestLaptimeDiff
     */
    [[nodiscard]] QTime getBestLaptimeDiff() const noexcept override
    {
        if (mLastLaptime == QTime{0, 0, 0, 0} or mBestLaptime == QTime{0, 0, 0, 0}) {
            return QTime{0, 0, 0, 0};
        }
        return Common::TimeUtils::durationBetween(mLastLaptime, mBestLaptime);
    }
    /**
     * @copydoc ILiveSessionManagement::bestLaptimeLap
     */
    [[nodiscard]] quint32 getBestLaptimeLap() const noexcept override
    {
        return mBestLaptimeLap;
    }

    /**
     * @copydoc ILiveSessionManagement::averageLaptime
     */
    [[nodiscard]] QTime getAverageLaptime() const noexcept override
    {
        return Common::TimeUtils::averageDuration(mLaptimes);
    }

    /**
     * @copydoc ILiveSessionManagement::currentLap
     */
    [[nodiscard]] quint32 getLapCount() const noexcept override
    {
        return mLapCount;
    }

    /**
     * @copydoc ILiveSessionManagement::trackName
     */
    [[nodiscard]] QString getTrackName() const noexcept override
    {
        return mTrackName;
    }

    /**
     * @copydoc ILiveSessionManagement::setDeviceSettings
     */
    void setDeviceSettings(RapidAndroid::Common::DeviceSettings const& settings) override
    {
        mEventSource->setLaptimerConfig(settings);
    }

private:
    void onCurrentSessionReceived(Common::CurrentSessionEvent const& event) noexcept
    {
        auto const& laps = event.session->getLaps();
        std::ranges::transform(laps, std::back_inserter(mLaptimes), [](Common::Lap const& lap) -> QTime {
            return lap.laptime();
        });
        mTrackName = event.session->getTrack().name;
        Q_EMIT trackNameChanged();
        if (not mLaptimes.isEmpty()) {
            mLapCount = static_cast<quint32>(laps.size());
            Q_EMIT lapCountChanged();
            auto const bestLapIter = std::ranges::min_element(mLaptimes);
            mBestLaptime = *bestLapIter;
            mBestLaptimeLap = static_cast<quint32>(std::distance(mLaptimes.begin(), bestLapIter)) + 1;
            Q_EMIT bestLaptimeChanged();
            mLastLaptime = laps.isEmpty() ? QTime{0, 0, 0, 0} : laps.last().laptime();
            Q_EMIT lastLaptimeChanged();
            Q_EMIT averageLaptimeChanged();
            Q_EMIT bestLaptimeDiffChanged();
        }
        connect(mEventSource,
                &EventSourceType::currentLaptimeChanged,
                this,
                [this](Common::LaptimeEvent const& ltEvent) {
                    mCurrentLaptime = ltEvent.laptime;
                    Q_EMIT currentLaptimeChanged();
                });
        connect(mEventSource, &EventSourceType::laptimeFinished, this, [this](Common::LapFinishedEvent const& lfEvent) {
            ++mLapCount;
            mLastLaptime = lfEvent.laptime;
            mLaptimes.append(lfEvent.laptime);
            if (mBestLaptime == QTime{0, 0, 0, 0} or lfEvent.laptime < mBestLaptime) {
                mBestLaptime = lfEvent.laptime;
                mBestLaptimeLap = mLapCount;
                Q_EMIT bestLaptimeChanged();
            }
            Q_EMIT lapCountChanged();
            Q_EMIT bestLaptimeDiffChanged();
            Q_EMIT lastLaptimeChanged();
            Q_EMIT averageLaptimeChanged();
        });
    }

private:
    EventSourceType* mEventSource{nullptr};
    QTime mCurrentLaptime{0, 0, 0, 0};
    QTime mLastLaptime{0, 0, 0, 0};
    QTime mBestLaptime{0, 0, 0, 0};
    QVector<QTime> mLaptimes;
    quint32 mLapCount{0};
    quint32 mBestLaptimeLap{0};
    QString mTrackName{tr("Unknown Track")};
};

} // namespace RapidAndroid::Workflow

#endif // RAPIDANDROID_WORKFLOW_LIVESESSIONMANAGEMENT_HPP
