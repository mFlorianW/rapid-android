// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "SettingsMemoryBackend.hpp"

namespace RapidAndroid::Common::Tests
{

bool SettingsMemoryBackend::storeValue(QAnyStringView const& key, QVariant const& value) noexcept
{
    mSingleValues.insert(key.toString(), value);
    return true;
}

QVariant SettingsMemoryBackend::getValue(QAnyStringView const& key) const noexcept
{
    return mSingleValues[key.toString()];
}

} // namespace RapidAndroid::Common::Tests
