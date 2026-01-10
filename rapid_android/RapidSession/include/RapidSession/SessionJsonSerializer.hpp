// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_SESSIONJSONSERIALIZER_HPP
#define RAPIDANDROID_SESSIONJSONSERIALIZER_HPP

#include <Common/Session.hpp>
#include <Workflow/ISessionSerializer.hpp>

namespace RapidAndroid::Session
{

/**
 * @brief Utility for serializing a session to JSON.
 *
 * Provides a static function to convert a Common::Session instance into a UTF-8 encoded JSON representation. 
 * This class is not intended to be instantiated.
 */
class SessionJsonSerializer
{
public:
    /**
     * @brief Serializes a session to JSON format asynchronously.
     *
     * @param session A unique pointer to the session to be serialized.
     * @return A QFuture that will contain an optional SerializeResult with the serialized JSON data on success,
     *         or std::nullopt on failure.
     */
    QFuture<Workflow::SerializeResult> serialize(std::unique_ptr<Common::Session> session) noexcept;

    /**
     * @brief Serializes only the session info to JSON format asynchronously.
     *
     * @param session A unique pointer to the session whose info is to be serialized.
     * @param sessionId The identifier of the session.
     * @return A QFuture that will contain an optional SerializeResult with the serialized JSON data on success,
     *         or std::nullopt on failure.
     */
    QFuture<Workflow::SerializeResult> serializeInfo(std::unique_ptr<Common::Session> session,
                                                     QString sessionId) noexcept;
};

} // namespace RapidAndroid::Session

#endif // RAPIDANDROID_SESSIONJSONSERIALIZER_HPP
