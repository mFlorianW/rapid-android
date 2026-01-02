// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_TESTHELPER_JSONOSCHERSLEBENSESSION_HPP
#define RAPIDANDROID_TESTHELPER_JSONOSCHERSLEBENSESSION_HPP

#include <Common/Session.hpp>
#include <Common/SessionInfo.hpp>
#include <QJsonDocument>

namespace RapidAndroid::TestHelper
{

/**
 * @brief Provides a JSON document representing a session from Oschersleben track.
 *
 * @return QJsonDocument containing the session data.
 */
QJsonDocument getJsonOscherslebenSession() noexcept;

/**
 * @brief Provides a JSON document representing session info from Oschersleben track.
 *
 * @return QJsonDocument containing the session info data.
 */
QJsonDocument getJsonOscherslebenSessionInfo() noexcept;

/**
 * @brief Provides a Common::Session object representing a session from Oschersleben track.
 *
 * @return Common::Session containing the session data.
 */
Common::Session getOscherslebenSession() noexcept;

/**
 * @brief Provides a Common::SessionInfo object representing session info from Oschersleben track.
 *
 * @return Common::SessionInfo containing the session info data.
 */
Common::SessionInfo getOscherslebenSessionInfo() noexcept;

} // namespace RapidAndroid::TestHelper

#endif // RAPIDANDROID_TESTHELPER_JSONOSCHERSLEBENSESSION_HPP
