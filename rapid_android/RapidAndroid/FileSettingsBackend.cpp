// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "FileSettingsBackend.hpp"
#include <QCoreApplication>

namespace RapidAndroid
{

FileSettingsBackend::FileSettingsBackend()
    : mSettings{std::make_unique<QSettings>(QCoreApplication::organizationName(), QCoreApplication::applicationName())}
{
}

FileSettingsBackend::~FileSettingsBackend() = default;

bool FileSettingsBackend::storeValue(QAnyStringView const& key, QVariant const& value) noexcept
{
    mSettings->setValue(key, value);
    return true;
}

QVariant FileSettingsBackend::getValue(QAnyStringView const& key) const noexcept
{
    return mSettings->value(key);
}

} // namespace RapidAndroid
