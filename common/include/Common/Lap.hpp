// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_COMMON_LAP_HPP
#define RAPIDANDROID_COMMON_LAP_HPP

#include <Common/GnssPosition.hpp>
#include <QTime>

namespace RapidAndroid::Common
{

/**
 * @brief A recoreded lap.
 *
 * A Lap aggregates per-sector timing information and the GNSS track samples captured during the lap.
 *
 * - sectors: Timing values for each sector, ordered as driven.
 * - logPoints: GNSS positions sampled along the lap path in chronological order.
 */
struct Lap
{
    QVector<QTime> sectors;
    QVector<GnssPosition> logPoints;

    /**
      * @brief Returns the accumulated lap time.
      *
      * Computes the total lap time by aggregating all sector timings stored in sectors.
      * @return Total lap time for this lap.
      */
    QTime laptime() const noexcept;

    /**
     * @brief Equality operator.
     * @param other The other Lap to compare with.
     * @return true if all members are equal, false otherwise.
     */
    bool operator==(Lap const& other) const noexcept = default;

    /**
     * @brief Inequality operator.
     * @param other The other Lap to compare with.
     * @return true if any member differs, false otherwise.
     */
    bool operator!=(Lap const& other) const noexcept = default;
};

} // namespace RapidAndroid::Common

#endif // RAPIDANDROID_COMMON_LAP_HPP
