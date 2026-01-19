// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_RAPIDLIVESESSION_LIVESESSIONJSONDESERIALIZER_HPP
#define RAPIDANDROID_RAPIDLIVESESSION_LIVESESSIONJSONDESERIALIZER_HPP

#include <QtConcurrentRun>
#include <RapidLiveSession/ILiveSessionEventDeserializer.hpp>

namespace RapidAndroid::RapidLiveSession
{

/**
 * @brief The LiveSessionJsonDeserializer class is responsible for deserializing JSON data into Event objects.
 */
class LiveSessionJsonDeserializer
{
public:
    /**
     * @brief Deserializes the given JSON data into an Event object asynchronously.
     * @param data The JSON data to deserialize.
     * @return A QFuture that will contain an optional Event object upon completion.
     */
    QFuture<std::optional<Event>> deserialize(QByteArray const& data);

private:
    /**
     * @brief Parses a JSON object into an Event object.
     * @param dataObj The JSON object to parse.
     * @return An optional Event object if parsing is successful; otherwise, std::nullopt.
     */
    std::optional<Event> parseLaptimeEvent(QJsonObject const& dataObj);

    /**
     * @brief Parses a JSON object into a LapFinished Event object.
     * @param dataObj The JSON object to parse.
     * @return An optional Event object if parsing is successful; otherwise, std::nullopt.
     */
    std::optional<Event> parseLapFinishedEvent(QJsonObject const& dataObj);

    /**
     * @brief Parses a time string into a QTime object.
     * @param timeStr The time string to parse.
     * @return An optional QTime object if parsing is successful; otherwise, std::nullopt.
     */
    std::optional<QTime> parseTime(QString const& timeStr);

    /**
     * @brief Parses a JSON object into a CurrentSession Event object.
     * @param dataObj The JSON object to parse.
     * @return An optional Event object if parsing is successful; otherwise, std::nullopt.
     */
    std::optional<Event> parseCurrentSessionEvent(QJsonObject const& dataObj);
};

} // namespace RapidAndroid::RapidLiveSession

#endif // RAPIDANDROID_RAPIDLIVESESSION_LIVESESSIONJSONDESERIALIZER_HPP
