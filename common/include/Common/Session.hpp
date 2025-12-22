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
 * @brief Aggregates metadata and results for a single driving session.
 *
 * Contains the session date/time, the track where it took place, and the sequence of recorded laps.
 */
struct Session
{
    /**
     * @brief Calendar date of the session.
     */
    QDate date;

    /**
     * @brief Start time of the session.
     */
    QTime time;

    /**
     * @brief Track on which the session was recorded.
     */
    Track track;

    /**
     * @brief Recorded laps in chronological order.
     */
    QVector<Lap> laps;

    /**
     * @brief Equality operator.
     * @param other The other Session to compare with.
     * @return true if all members are equal, false otherwise.
     */
    bool operator==(Session const& other) const noexcept = default;

    /**
     * @brief Inequality operator.
     * @param other The other Session to compare with.
     * @return true if any member differs, false otherwise.
     */
    bool operator!=(Session const& other) const noexcept = default;
};

} // namespace RapidAndroid::Common

#endif // RAPIDANDROID_COMMON_SESSION_HPP
