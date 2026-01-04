// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_COMMON_GNSSPOSITION_HPP
#define RAPIDANDROID_COMMON_GNSSPOSITION_HPP

#include <QDate>
#include <QTime>

namespace RapidAndroid::Common
{

/**
 * @brief GNSS position fix.
 *
 * Represents a geographic position (WGS84) in decimal degrees with ground velocity and the date/time of the position.
 */
struct GnssPosition
{
    double latitude{0.0}; //!< Latitude in decimal degrees [-90.0, +90.0].
    double longitude{0.0}; //!< Longitude in decimal degrees [-180.0, +180.0].
    double velocity{0.0}; //!< Ground velocity in m/s.
    QTime time; //!< Time of the GNSS fix.
    QDate date; //!< Date of the GNSS fix.

    /**
     * @brief Equality operator.
     * @param other The other GnssPosition to compare with.
     * @return true if all members are equal, false otherwise.
     */
    bool operator==(GnssPosition const& other) const noexcept = default;

    /**
     * @brief Inequality operator.
     * @param other The other GnssPosition to compare with.
     * @return true if any member differs, false otherwise.
     */
    bool operator!=(GnssPosition const& other) const noexcept = default;
};

} // namespace RapidAndroid::Common

#endif // RAPIDANDROID_COMMON_GNSSPOSITION_HPP
