// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_WORKFLOW_LIVESESSIONMANAGEMENT_HPP
#define RAPIDANDROID_WORKFLOW_LIVESESSIONMANAGEMENT_HPP

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

        connect(mEventSource, &EventSourceType::currentLaptimeChanged, this, [this](Common::LaptimeEvent const& event) {
            mCurrentLaptime = event.laptime;
            Q_EMIT currentLaptimeChanged();
        });
        connect(mEventSource, &EventSourceType::laptimeFinished, this, [this](Common::LapFinishedEvent const& event) {
            ++mLapCount;
            mLastLaptime = event.laptime;
            if (mBestLaptime == QTime{0, 0, 0, 0} || event.laptime < mBestLaptime) {
                mBestLaptime = event.laptime;
                Q_EMIT bestLaptimeChanged();
            }
            Q_EMIT lapCountChanged();
            Q_EMIT lastLaptimeChanged();
        });
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
     * @copydoc ILiveSessionManagement::currentLap
     */
    [[nodiscard]] quint32 getLapCount() const noexcept override
    {
        return mLapCount;
    }

    /**
     * @copydoc ILiveSessionManagement::setDeviceSettings
     */
    void setDeviceSettings(RapidAndroid::Common::DeviceSettings const& settings) override
    {
        mEventSource->setLaptimerConfig(settings);
    }

private:
    EventSourceType* mEventSource{nullptr};
    QTime mCurrentLaptime{0, 0, 0, 0};
    QTime mLastLaptime{0, 0, 0, 0};
    QTime mBestLaptime{0, 0, 0, 0};
    quint32 mLapCount{0};
};

} // namespace RapidAndroid::Workflow

#endif // RAPIDANDROID_WORKFLOW_LIVESESSIONMANAGEMENT_HPP
