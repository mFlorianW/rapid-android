// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_WORKFLOW_ILIVESESSIONEVENTDESERIALIZER_HPP
#define RAPIDANDROID_WORKFLOW_ILIVESESSIONEVENTDESERIALIZER_HPP

#include <Common/LaptimeEvents.hpp>
#include <QByteArray>
#include <QFuture>
#include <optional>

namespace RapidAndroid::RapidLiveSession
{
/**
 * Alias for the variant type that can hold any of the live session event types.
 */
using Event =
    std::variant<Common::LapStartedEvent, Common::LaptimeEvent, Common::LapSectorEvent, Common::LapFinishedEvent>;

/**
 * Concept that checks if a type T conforms to the LiveSessionEventDeserializer interface.
 * A type T is considered a LiveSessionEventDeserializer if it has a method `deserialize` that takes a QByteArray and returns a QFuture<std::optional<Event>>.
 */
template <typename T>
concept LiveSessionEventDeserializerConcept = requires(T obj, QByteArray const& data) {
    { obj.deserialize(data) } -> std::same_as<QFuture<std::optional<Event>>>;
};

} // namespace RapidAndroid::RapidLiveSession

#endif // RAPIDANDROID_WORKFLOW_ILIVESESSIONEVENTDESERIALIZER_HPP
