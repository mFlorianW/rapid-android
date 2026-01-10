// SPDX-FileCopyrightText: 2025, 2026 All contributors
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
    { a.getSessionInfos() } -> std::same_as<QFuture<QVector<RapidAndroid::Common::SessionInfo>>>;
    { a.load(std::declval<Common::SessionInfo const&>()) } -> std::same_as<QFuture<std::optional<RapidAndroid::Common::Session>>>;
    { a.store(std::declval<std::unique_ptr<Common::Session>>()) } -> std::same_as<QFuture<StoreResult>>;
    { a.remove(std::declval<Common::SessionInfo const&>()) } -> std::same_as<QFuture<bool>>;
};

// clang-format on

} // namespace RapidAndroid::Workflow

#endif // RAPIDANDROID_WORKFLOW_ISESSIONSTORAGE_HPP
