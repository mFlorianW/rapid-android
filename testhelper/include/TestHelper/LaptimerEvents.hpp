// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_TESTHELPER_LAPTIMEREVENTS_HPP
#define RAPIDANDROID_TESTHELPER_LAPTIMEREVENTS_HPP

#include <QString>
#include <QTime>

namespace RapidAndroid::TestHelper
{

/**
 * @brief Generates a JSON string representing a laptime event with the given time.
 * @param time The laptime to include in the event.
 * @return A JSON string representing the laptime event.
 */
QString laptimeEventJson(QTime const& time);

/**
 * @brief Generates a JSON string representing a lap finished event with the given time.
 * @param time The total time at which the lap was finished.
 * @return A JSON string representing the lap finished event.
 */
QString lapFinishedEventJson(QTime const& time);

/**
 * @brief Generates a JSON string representing a current session event with the given session.
 * @param session The seralized session to include in the event.
 * @return A JSON string representing the current session event.
 */
QString currentSessionEventJson(QString const& session);

} // namespace RapidAndroid::TestHelper

#endif // RAPIDANDROID_TESTHELPER_LAPTIMEREVENTS_HPP
