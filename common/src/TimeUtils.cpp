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

} // namespace RapidAndroid::Common::TimeUtils
