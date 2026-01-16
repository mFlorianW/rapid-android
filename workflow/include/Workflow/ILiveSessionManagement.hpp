// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_WORKFLOW_ISESSIONLIVEVIEW_HPP
#define RAPIDANDROID_WORKFLOW_ISESSIONLIVEVIEW_HPP

#include <Common/DeviceSettings.hpp>
#include <QObject>
#include <QTime>

namespace RapidAndroid::Workflow
{

/**
 * @brief Interface for live session lap timing management.
 *
 * Provides access to current, last, and best lap times, the total lap count.
 */
class ILiveSessionManagement : public QObject
{
    Q_OBJECT

    /**
     * @property ILiveSessionManagement::currentLaptime
     * @brief Current lap's elapsed time.
     * @details Updated while a lap is in progress. Emits currentLaptimeChanged on change.
     */
    Q_PROPERTY(QTime currentLaptime READ getCurrentLaptime NOTIFY currentLaptimeChanged)

    /**
     * @property ILiveSessionManagement::lastLaptime
     * @brief Elapsed time of the most recently completed lap.
     * @details Emits lastLaptimeChanged when the last lap time is updated.
     */
    Q_PROPERTY(QTime lastLaptime READ getLastLaptime NOTIFY lastLaptimeChanged)

    /**
     * @property ILiveSessionManagement::bestLaptime
     * @brief Best (shortest) lap time recorded in the current session.
     * @details Emits bestLaptimeChanged when the best lap time is updated.
     */
    Q_PROPERTY(QTime bestLaptime READ getBestLaptime NOTIFY bestLaptimeChanged)

    /**
     * @property ILiveSessionManagement::bestLaptimeDiff
     * @brief Difference between the current lap time and the best lap time.
     * @details Emits bestLaptimeDiffChanged when the best lap time or current lap time changes.
     */
    Q_PROPERTY(QTime bestLaptimeDiff READ getBestLaptimeDiff NOTIFY bestLaptimeDiffChanged)

    /**
     * @property ILiveSessionManagement::lapCount
     * @brief Number of completed laps in the current session.
     * @details Emits lapCountChanged when the count changes.
     */
    Q_PROPERTY(quint32 lapCount READ getLapCount NOTIFY lapCountChanged)

public:
    Q_DISABLE_COPY_MOVE(ILiveSessionManagement)

    /**
     * @brief Virtual destructor.
     */
    ~ILiveSessionManagement() override = default;

    /**
     * @brief Returns the elapsed time of the current lap.
     * @return Current lap time as QTime. May be invalid or zero if no lap is active.
     */
    [[nodiscard]] virtual QTime getCurrentLaptime() const noexcept = 0;

    /**
     * @brief Returns the elapsed time of the most recently completed lap.
     * @return Last lap time as QTime. May be invalid or zero if no lap has been completed.
     */
    [[nodiscard]] virtual QTime getLastLaptime() const noexcept = 0;

    /**
     * @brief Returns the best (shortest) lap time recorded in the current session.
     * @return Best lap time as QTime. May be invalid or zero if no laps have been recorded.
     */
    [[nodiscard]] virtual QTime getBestLaptime() const noexcept = 0;

    /**
     * @brief Returns the difference between the current lap time and the best lap time.
     * @return Best lap time difference as QTime. May be invalid or zero if no best lap time is recorded.
     */
    [[nodiscard]] virtual QTime getBestLaptimeDiff() const noexcept = 0;

    /**
     * @brief Returns the number of completed laps in the current session.
     * @return Lap count as quint32.
     */
    [[nodiscard]] virtual quint32 getLapCount() const noexcept = 0;

    /**
     * @brief Applies device settings these settings are used to receive the live session events from the device.
     * @param settings Device settings to apply.
     */
    Q_INVOKABLE virtual void setDeviceSettings(RapidAndroid::Common::DeviceSettings const& settings) = 0;

Q_SIGNALS:
    /**
     * @brief Emitted when the current lap time changes.
     */
    void currentLaptimeChanged();

    /**
     * @brief Emitted when the last lap time is updated.
     */
    void lastLaptimeChanged();

    /**
     * @brief Emitted when the best lap time is updated.
     */
    void bestLaptimeChanged();

    /**
     * @brief Emitted when the best lap time difference changes.
     */
    void bestLaptimeDiffChanged();

    /**
     * @brief Emitted when the lap count changes.
     */
    void lapCountChanged();

protected:
    ILiveSessionManagement() = default;
};

} // namespace RapidAndroid::Workflow

#endif // RAPIDANDROID_WORKFLOW_ISESSIONLIVEVIEW_HPP
