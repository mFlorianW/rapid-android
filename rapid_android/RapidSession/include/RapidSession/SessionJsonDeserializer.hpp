// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_SESSIONSESSIONJSONDESERIALIZER_HPP
#define RAPIDANDROID_SESSIONSESSIONJSONDESERIALIZER_HPP

#include <QJsonObject>
#include <Workflow/ISessionDeserializer.hpp>

namespace RapidAndroid::Session
{

/**
 * @class SessionJsonDeserializer
 * @brief Utility for deserializing a session from JSON.
 *
 * Parses a UTF-8 encoded JSON payload and produces a Common::Session instance if the input is valid.
 * Returns std::nullopt when the payload cannot be parsed or does not satisfy the expected schema.
 */
class SessionJsonDeserializer
{
public:
    /**
     * @brief Deserialize a session from a JSON byte array.
     *
     * The input is expected to be UTF-8 encoded JSON. If the data cannot be parsed or is semantically invalid, the function returns std::nullopt.
     *
     * @param data JSON input as a QByteArray (UTF-8).
     * @return std::optional containing a Common::Session on success; std::nullopt on failure.
     */
    static QFuture<std::optional<std::unique_ptr<Common::Session>>> deserialize(QByteArray data);

    /**
     * @brief Deserialize session from a JSON object.
     *
     * If the data is semantically invalid, the function returns std::nullopt.
     *
     * @param data JSON input as a QJsonObject.
     * @return std::optional containing a Common::Session on success; std::nullopt on failure.
     */
    static QFuture<std::optional<std::unique_ptr<Common::Session>>> deserialize(QJsonObject const& data);

    /**
     * @brief Deserialize session info from a JSON byte array.
     *
     * The input is expected to be UTF-8 encoded JSON. If the data cannot be parsed or is semantically invalid, the function returns std::nullopt.
     *
     * @param data JSON input as a QByteArray (UTF-8).
     * @return std::optional containing a Common::SessionInfo on success; std::nullopt on failure.
     */
    static QFuture<std::optional<std::unique_ptr<Common::SessionInfo>>> deserializeInfo(QByteArray data);
};

} // namespace RapidAndroid::Session

#endif // RAPIDANDROID_SESSIONSESSIONJSONDESERIALIZER_HPP
