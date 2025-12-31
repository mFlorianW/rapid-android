// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_WORKFLOW_ISESSIONSTORAGE_HPP
#define RAPIDANDROID_WORKFLOW_ISESSIONSTORAGE_HPP

#include <Common/Session.hpp>
#include <Common/SessionInfo.hpp>
#include <QFuture>
#include <QVector>

namespace RapidAndroid::Workflow
{

/**
 * @brief Result of a store operation.
 */
struct StoreResult
{
    std::unique_ptr<Common::Session> session;
    bool success{false};
};

// clang-format off

/**
 * @brief Concept to verify that a type implements the ISessionStorage interface.
 */
template <typename T>
concept SessionStorageConcept = requires(T a) {
    { a.getSessionInfos() } -> std::same_as<QVector<RapidAndroid::Common::SessionInfo>>;
    { a.load(std::declval<RapidAndroid::Common::SessionInfo const&>()) } -> std::same_as<std::optional<RapidAndroid::Common::Session>>;
    { a.store(std::declval<std::unique_ptr<RapidAndroid::Common::Session>>()) } -> std::same_as<QFuture<StoreResult>>;
    { a.remove(std::declval<RapidAndroid::Common::Session const&>()) } -> std::same_as<bool>;
};

// clang-format on

} // namespace RapidAndroid::Workflow

#endif // RAPIDANDROID_WORKFLOW_ISESSIONSTORAGE_HPP
