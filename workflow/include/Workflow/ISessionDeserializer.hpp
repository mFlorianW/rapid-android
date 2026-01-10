// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_WORKFLOW_ISESSIONDESERIALIZER_HPP
#define RAPIDANDROID_WORKFLOW_ISESSIONDESERIALIZER_HPP

#include <Common/Session.hpp>
#include <Common/SessionInfo.hpp>
#include <QFuture>
#include <optional>

namespace RapidAndroid::Workflow
{

/**
 * @brief Concept to verify that a type implements the SessionDeserializer interface.
 */
template <typename T>
concept SessionDeserializerConcept = requires(T a, QByteArray data) {
    { a.deserialize(data) } -> std::same_as<QFuture<std::optional<std::unique_ptr<Common::Session>>>>;
    { a.deserializeInfo(data) } -> std::same_as<QFuture<std::optional<std::unique_ptr<Common::SessionInfo>>>>;
};

}; // namespace RapidAndroid::Workflow

#endif // RAPIDANDROID_WORKFLOW_ISESSIONDESERIALIZER_HPP
