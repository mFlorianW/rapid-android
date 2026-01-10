// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_COMMON_TRACK_HPP
#define RAPIDANDROID_COMMON_TRACK_HPP

#include <Common/Position.hpp>
#include <QString>
#include <QVector>
#include <optional>

namespace RapidAndroid::Common
{

/**
 * Represents a track and its marker positions.
 *
 * Fields:
 * - name: track name
 * - startline: position of the start line marker
 * - finishline: optional position of the finish line marker
 * - sectors: ordered positions delimiting sector boundaries, the position in the vector defines the sector number
 */
struct Track
{
    QString name; //!< Track name.
    Position startline; //!< Position of the start line marker.
    std::optional<Position> finishline; //!< Optional position of the finish line marker.
    QVector<Position> sectors; //!< Ordered positions delimiting sector boundaries.

    /**
     * Equality operator.
     * @param other The other Track to compare with.
     * @return true if all members are equal, false otherwise.
     */
    bool operator==(Track const& other) const noexcept = default;

    /**
     * Inequality operator.
     * @param other The other Track to compare with.
     * @return true if any member differs, false otherwise.
     */
    bool operator!=(Track const& other) const noexcept = default;
};

} // namespace RapidAndroid::Common

#endif // RAPIDANDROID_COMMON_TRACK_HPP
