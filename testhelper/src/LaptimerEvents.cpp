// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "TestHelper/LaptimerEvents.hpp"

namespace RapidAndroid::TestHelper
{

QString laptimeEventJson(QTime const& time)
{
    return QString{
        R"json(
    {
        "event": "current_laptime",
        "data": {
            "time": "%1"
        }
    }
    )json"}
        .arg(time.toString("hh:mm:ss.zzz"));
}

QString lapFinishedEventJson(QTime const& time)
{
    return QString{
        R"json(
    {
        "event": "lap_finished",
        "data": {
            "time": "%1"
        }
    }
    )json"}
        .arg(time.toString("hh:mm:ss.zzz"));
}

} // namespace RapidAndroid::TestHelper
