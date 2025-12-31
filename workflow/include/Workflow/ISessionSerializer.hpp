// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_WORKFLOW_ISESSIONSERIALIZER_HPP
#define RAPIDANDROID_WORKFLOW_ISESSIONSERIALIZER_HPP

#include <Common/Session.hpp>
#include <QFuture>
#include <utility>

namespace RapidAndroid::Workflow
{

/**
 * @brief Structure to hold the result of a serialization operation.
 */
struct SerializeResult
{
    std::unique_ptr<Common::Session> session;
    std::optional<QByteArray> data;

    SerializeResult() = default;
    SerializeResult(std::unique_ptr<Common::Session> sess, QByteArray d) noexcept
        : session{std::move(sess)}
        , data{std::move(d)}
    {
    }
    ~SerializeResult() = default;

    SerializeResult(SerializeResult const&) = delete;
    SerializeResult& operator=(SerializeResult const&) = delete;
    SerializeResult(SerializeResult&&) noexcept = default;
    SerializeResult& operator=(SerializeResult&&) noexcept = default;
};

/**
 * @brief Concept to verify that a type implements the SessionSerializer interface.
 */
template <typename T>
concept SessionSerializerConcept = requires(T a, std::unique_ptr<Common::Session> session) {
    { a.serialize(std::move(session)) } -> std::same_as<QFuture<Workflow::SerializeResult>>;
};

}; // namespace RapidAndroid::Workflow

#endif // RAPIDANDROID_WORKFLOW_ISESSIONSERIALIZER_HPP
