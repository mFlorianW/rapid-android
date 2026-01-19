// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_COMMON_LIVESESSIONEVENTS_HPP
#define RAPIDANDROID_COMMON_LIVESESSIONEVENTS_HPP

#include <Common/Session.hpp>
#include <QTime>
#include <memory>

namespace RapidAndroid::Common
{

/**
 * @brief Event indicating that a lap has started.
 */
struct LapStartedEvent
{
};

/**
 * @brief Event indicating an update to the current lap time.
 */
struct LaptimeEvent
{
    QTime laptime;
};

/**
 * @brief Event indicating the completion of a lap sector.
 */
struct LapSectorEvent
{
    QTime sectorTime;
};

/**
 * @brief Event indicating that a lap has finished.
 */
struct LapFinishedEvent
{
    QTime laptime;
};

/**
 * @brief Event providing information about the current session.
 */
struct CurrentSessionEvent
{
    std::unique_ptr<Session> session;
};

} // namespace RapidAndroid::Common

#endif // RAPIDANDROID_COMMON_LIVESESSIONEVENTS_HPP
