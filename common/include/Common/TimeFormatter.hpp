// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_COMMON_TIMEFORMATER_HPP
#define RAPIDANDROID_COMMON_TIMEFORMATER_HPP

#include <QString>
#include <QTime>

namespace RapidAndroid::Common::TimeFormatter
{

/**
 * @brief Formats a QTime representing a lap time into a human-readable string.
 * @details The formatted string will only contain values that are greater than zero.
 *
 * For example:
 * - A time of 01:02:03.456 will be formatted as "01:02:03.456"
 * - A time of 00:01:23.456 will be formatted as "01:23.456"
 * - A time of 00:00:45.678 will be formatted as "00:45.678"
 * - A time of 00:00:00.123 will be formatted as "00:00.123"
 *
 * @param time The QTime to format.
 * @return Formatted lap time string.
 */
QString formatTime(QTime const& time);

} // namespace RapidAndroid::Common::TimeFormatter

#endif // RAPIDANDROID_COMMON_TIMEFORMATER_HPP
