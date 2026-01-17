// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_COMMON_TIMEUTILS_HPP
#define RAPIDANDROID_COMMON_TIMEUTILS_HPP

#include <QTime>
#include <span>

namespace RapidAndroid::Common::TimeUtils
{

/**
 * @brief Calculate the duration between two QTime objects.
 *
 * @details It's always the shortest duration, regardless of the order of start and end times.
 *
 * Example:
 * - start: 10:00, end: 11:30 -> duration: 1:30
 * - start: 11:30, end: 10:00 -> duration: 1:30
 * - start: 23:00, end: 01:00 -> duration: 2:00
 *
 * @param start The start time.
 * @param end The end time.
 * @return The duration as a QTime object.
 */
QTime durationBetween(QTime const& start, QTime const& end);

/**
 * @brief Calculate the average duration from a span of QTime durations.
 *
 * @param durations A span of QTime durations.
 * @return The average duration as a QTime object.
 */
QTime averageDuration(std::span<QTime const> durations);

} // namespace RapidAndroid::Common::TimeUtils
#endif // RAPIDANDROID_COMMON_TIMEUTILS_HPP
