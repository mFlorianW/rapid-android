// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Common/SettingsWriter.hpp"
#include "Common/DeviceSettings.hpp"
#include "Common/Private/SettingsKeys.hpp"
#include "Common/SettingsBackend.hpp"

namespace RapidAndroid::Common
{

SettingsWriter::SettingsWriter(SettingsBackend* settingsBackend)
    : mSettingsBackend{settingsBackend}
{
}

SettingsWriter::~SettingsWriter() = default;

bool SettingsWriter::storeDeviceSettings(QList<DeviceSettings> const& deviceSettings) noexcept
{
    if (deviceSettings.isEmpty()) {
        return true;
    }
    if (not mSettingsBackend->storeValue(DeviceSettingsSize, deviceSettings.size())) {
        return false;
    }
    for (qsizetype i = 0; i < deviceSettings.size(); ++i) {
        if (not mSettingsBackend->storeValue(DeviceSettingsName.toString().arg(i), deviceSettings[i].name) or
            not mSettingsBackend->storeValue(DeviceSettingsIp.toString().arg(i), deviceSettings[i].ip.toString()) or
            not mSettingsBackend->storeValue(DeviceSettingsPort.toString().arg(i), deviceSettings[i].port) or
            not mSettingsBackend->storeValue(DeviceSettingsDef.toString().arg(i), deviceSettings[i].defaultDevice)) {
            return false;
        }
    }
    return true;
}

} // namespace RapidAndroid::Common
