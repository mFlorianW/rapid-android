// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_COMMON_POSITION_HPP
#define RAPIDANDROID_COMMON_POSITION_HPP

namespace RapidAndroid::Common
{

/**
 * @brief Geographic position (WGS84) expressed in decimal degrees.
 *
 * Latitude ranges from -90.0 (south) to +90.0 (north).
 * Longitude ranges from -180.0 (west) to +180.0 (east).
 */
struct Position
{
    double latitude; //!< Latitude in decimal degrees.
    double longitude; //!< Longitude in decimal degrees.

    /**
     * @brief Equality operator.
     * @param other The other Position to compare with.
     * @return true if both latitude and longitude are equal, false otherwise.
     */
    bool operator==(Position const& other) const noexcept = default;

    /**
     * @brief Inequality operator.
     * @param other The other Position to compare with.
     * @return true if either latitude or longitude differ, false otherwise.
     */
    bool operator!=(Position const& other) const noexcept = default;
};

} // namespace RapidAndroid::Common

#endif // RAPIDANDROID_COMMON_POSITION_HPP
