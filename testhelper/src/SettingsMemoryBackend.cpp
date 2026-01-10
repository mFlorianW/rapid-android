// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <TestHelper/SettingsMemoryBackend.hpp>

namespace RapidAndroid::TestHelper
{

void SettingsMemoryBackend::enableAlwaysFailureMode() noexcept
{
    mAlwaysFail = true;
}

bool SettingsMemoryBackend::storeValue(QAnyStringView const& key, QVariant const& value) noexcept
{
    if (mAlwaysFail) {
        return false;
    }
    mSingleValues.insert(key.toString(), value);
    return true;
}

QVariant SettingsMemoryBackend::getValue(QAnyStringView const& key) const noexcept
{
    if (mAlwaysFail) {
        return {};
    }
    return mSingleValues[key.toString()];
}

} // namespace RapidAndroid::TestHelper
