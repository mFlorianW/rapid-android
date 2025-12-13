// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPID_COMMON_TESTS_SETTINGS_MEMORY_BACKEND_HPP
#define RAPID_COMMON_TESTS_SETTINGS_MEMORY_BACKEND_HPP

#include <Common/SettingsBackend.hpp>
#include <QVariantHash>

namespace RapidAndroid::Common::Tests
{

class SettingsMemoryBackend : public Common::SettingsBackend
{
public:
    [[nodiscard]] bool storeValue(QAnyStringView const& key, QVariant const& value) noexcept override;

    [[nodiscard]] QVariant getValue(QAnyStringView const& key) const noexcept override;

private:
    QVariantHash mSingleValues;
};

} // namespace RapidAndroid::Common::Tests

#endif // RAPID_COMMON_TESTS_SETTINGS_MEMORY_BACKEND_HPP
