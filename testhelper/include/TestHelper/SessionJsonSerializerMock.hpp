// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_TESTHELPER_SESSIONJSONSERIALIZERMOCK_HPP
#define RAPIDANDROID_TESTHELPER_SESSIONJSONSERIALIZERMOCK_HPP

#include <Common/Session.hpp>
#include <QJsonDocument>
#include <Workflow/ISessionSerializer.hpp>
#include <gmock/gmock.h>

namespace RapidAndroid::TestHelper
{

class SessionJsonSerializerMock
{

public:
    MOCK_METHOD(QFuture<Workflow::SerializeResult>, serialize, (std::unique_ptr<Common::Session>), ());
};

} // namespace RapidAndroid::TestHelper

#endif // RAPIDANDROID_TESTHELPER_SESSIONJSONSERIALIZERMOCK_HPP
