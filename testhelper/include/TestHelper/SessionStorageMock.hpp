// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_TESTHELPER_SESSIONSTORAGEMOCK_HPP
#define RAPIDANDROID_TESTHELPER_SESSIONSTORAGEMOCK_HPP

#include <Workflow/ISessionStorage.hpp>
#include <gmock/gmock.h>

namespace RapidAndroid::TestHelper
{

class SessionStorageMock
{
public:
    // NOLINTBEGIN(bugprone-exception-escape)
    // clang-format off
    MOCK_METHOD(QFuture<QVector<RapidAndroid::Common::SessionInfo>>, getSessionInfos, (), (noexcept));
    MOCK_METHOD(QFuture<std::optional<RapidAndroid::Common::Session>>, load, (RapidAndroid::Common::SessionInfo const&), (noexcept));
    MOCK_METHOD(QFuture<Workflow::StoreResult>, store, (std::unique_ptr<RapidAndroid::Common::Session>), (noexcept));
    MOCK_METHOD(QFuture<bool>, remove, (RapidAndroid::Common::SessionInfo const&), (noexcept));
    // clang-format on
    // NOLINTEND(bugprone-exception-escape)
};

} // namespace RapidAndroid::TestHelper

#endif // RAPIDANDROID_TESTHELPER_SESSIONSTORAGEMOCK_HPP
