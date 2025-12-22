// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Common/Lap.hpp"

namespace RapidAndroid::Common
{

QTime Lap::laptime() const noexcept
{
    QTime totalTime(0, 0, 0, 0);
    for (auto const& sector : sectors) {
        totalTime = totalTime.addMSecs(QTime(0, 0, 0, 0).msecsTo(sector));
    }
    return totalTime;
}

} // namespace RapidAndroid::Common
