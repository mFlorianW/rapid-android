// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Common/TimeUtils.hpp"

namespace RapidAndroid::Common::TimeUtils
{

QTime durationBetween(QTime const& start, QTime const& end)
{
    constexpr int dayMs = 24 * 60 * 60 * 1000;

    int forward = start.msecsTo(end);
    if (forward < 0) {
        forward += dayMs;
    }

    int backward = dayMs - forward; // the other way around
    int ms = std::min(forward, backward);

    return QTime(0, 0).addMSecs(ms);
}

QTime averageDuration(std::span<QTime const> durations)
{
    if (durations.empty()) {
        return {0, 0, 0, 0};
    }

    qint64 totalMs = 0;
    for (auto const& duration : durations) {
        totalMs += duration.msecsSinceStartOfDay();
    }

    qint64 averageMs = totalMs / static_cast<qint64>(durations.size());
    return QTime(0, 0).addMSecs(static_cast<int>(averageMs));
}

} // namespace RapidAndroid::Common::TimeUtils
