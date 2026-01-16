// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "TimeFormatter.hpp"
#include <Common/TimeFormatter.hpp>

namespace RapidAndroid
{

QString TimeFormatter::formatTime(QTime const& time) noexcept
{
    return Common::TimeFormatter::formatTime(time);
}

} // namespace RapidAndroid
