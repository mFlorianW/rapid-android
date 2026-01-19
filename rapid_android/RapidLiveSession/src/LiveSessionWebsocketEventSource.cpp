// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "LoggingCategories.hpp"
#include <QLoggingCategory>
#include <RapidLiveSession/LiveSessionWebsocketEventSource.hpp>

namespace RapidAndroid::RapidLiveSession
{

QLoggingCategory const& rljeLog()
{
    return rlje();
}

} // namespace RapidAndroid::RapidLiveSession
