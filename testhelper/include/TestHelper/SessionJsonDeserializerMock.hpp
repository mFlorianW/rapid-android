// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_TESTHELPER_SESSIONDESERIALIZERMOCK_HPP
#define RAPIDANDROID_TESTHELPER_SESSIONDESERIALIZERMOCK_HPP

#include <Common/Session.hpp>
#include <QFuture>
#include <gmock/gmock.h>
#include <optional>

namespace RapidAndroid::TestHelper
{

class SessionJsonDeserializerMock
{
public:
    MOCK_METHOD(QFuture<std::optional<std::unique_ptr<Common::Session>>>, deserialize, (QByteArray), ());
};

} // namespace RapidAndroid::TestHelper

#endif // RAPIDANDROID_TESTHELPER_SESSIONDESERIALIZERMOCK_HPP
