// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_COMMON_SESSION_HPP
#define RAPIDANDROID_COMMON_SESSION_HPP

#include <Common/Lap.hpp>
#include <Common/Track.hpp>
#include <QDate>
#include <QTime>
#include <QVector>

namespace RapidAndroid::Common
{

/**
 * @brief Private data structure for Session, using the implicit sharing idiom.
 */
struct SessionData : public QSharedData
{
    SessionData() = default;
    ~SessionData() = default;

    SessionData(SessionData const& other) = default;
    SessionData& operator=(SessionData const& other) = delete;
    SessionData(SessionData&& other) noexcept = default;
    SessionData& operator=(SessionData&& other) noexcept = delete;

    QDate date;
    QTime time;
    Track track;
    QVector<Lap> laps;

    bool operator==(SessionData const& other) const noexcept
    {
        return date == other.date && time == other.time && track == other.track && laps == other.laps;
    }

    bool operator!=(SessionData const& other) const noexcept
    {
        return !(*this == other);
    }
};

/**
 * @brief Aggregates metadata and results for a single driving session.
 *
 * Contains the session date/time, the track where it took place, and the sequence of recorded laps.
 */
class Session
{
public:
    /**
     * @brief Default constructor.
     */
    Session();

    /**
     * @brief Destructor.
     */
    ~Session();

    /**     
     * @brief Copy constructor.
     * @param other The Session to copy from.
     */
    Session(Session const& other);

    /**
     * @brief Copy assignment operator.
     * @param other The Session to copy from.
     * @return Reference to this Session.
     */
    Session& operator=(Session const& other);

    /**
     * @brief Move constructor.
     * @param other The Session to move from.
     */
    Session(Session&& other) noexcept;

    /**
     * @brief Move assignment operator.
     * @param other The Session to move from.
     * @return Reference to this Session.
     */
    Session& operator=(Session&& other) noexcept;

    /**     
     * @brief Copy constructor.
     * @param other The Session to copy from.
     */
    QDate getDate() const noexcept;

    /**
     * @brief Sets the date of the session.
     * @param date The date to set.
     */
    void setDate(QDate const& date) noexcept;

    /**
     * @brief Gets the time of the session.
     * @return The time of the session.
     */
    QTime getTime() const noexcept;

    /**
     * @brief Sets the time of the session.
     * @param time The time to set.
     */
    void setTime(QTime const& time) noexcept;

    /**
     * @brief Gets the track of the session.
     * @return The track of the session.
     */
    Track getTrack() const noexcept;

    /**
     * @brief Sets the track of the session.
     * @param track The track to set.
     */
    void setTrack(Track const& track) noexcept;

    /**
     * @brief Gets the laps of the session.
     * @return The laps of the session.
     */
    QVector<Lap> getLaps() const noexcept;

    /**
     * @brief Sets the laps of the session.
     * @param laps The laps to set.
     */
    void setLaps(QVector<Lap> const& laps) noexcept;

    /**
     * @brief Equality operator.
     * @param other The other Session to compare with.
     * @return true if all members are equal, false otherwise.
     */
    bool operator==(Session const& other) const noexcept
    {
        return mData->operator==(*other.mData);
    }

    /**
     * @brief Inequality operator.
     * @param other The other Session to compare with.
     * @return true if any member differs, false otherwise.
     */
    bool operator!=(Session const& other) const noexcept
    {
        return !(*this == other);
    }

private:
    QSharedDataPointer<SessionData> mData;
};

} // namespace RapidAndroid::Common

#endif // RAPIDANDROID_COMMON_SESSION_HPP
