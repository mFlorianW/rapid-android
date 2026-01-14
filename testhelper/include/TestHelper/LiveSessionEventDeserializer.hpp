// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_TESTHELPER_LIVESESSIONEVENTDESERIALIZER_HPP
#define RAPIDANDROID_TESTHELPER_LIVESESSIONEVENTDESERIALIZER_HPP

#include <RapidLiveSession/ILiveSessionEventDeserializer.hpp>
#include <gmock/gmock.h>

namespace RapidAndroid::TestHelper
{
class LiveSessionEventDeserializer
{
public:
    MOCK_METHOD(QFuture<std::optional<RapidLiveSession::Event>>, deserialize, (QByteArray const& data), ());
};

} // namespace RapidAndroid::TestHelper

#endif // RAPIDANDROID_TESTHELPER_LIVESESSIONEVENTDESERIALIZER_HPP
