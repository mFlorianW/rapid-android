// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Common/TimeFormatter.hpp"

namespace RapidAndroid::Common::TimeFormatter
{

QString formatTime(QTime const& time)
{
    if (time == QTime{0, 0, 0, 0}) {
        return QTime{0, 0, 0, 0}.toString("mm:ss.zzz");
    } else if (time.minute() == 0) {
        return time.toString("ss.zzz");
    } else if (time.hour() == 0) {
        return time.toString("mm:ss.zzz");
    } else {
        return time.toString("hh:mm:ss.zzz");
    }
}

} // namespace RapidAndroid::Common::TimeFormatter
