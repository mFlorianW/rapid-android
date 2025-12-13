// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Common/SettingsReader.hpp"
#include "Common/SettingsBackend.hpp"
#include "SettingsKeys.hpp"

namespace RapidAndroid::Common
{

SettingsReader::SettingsReader(SettingsBackend* settingsBackend)
    : mSettingsBackend{settingsBackend}
{
}

SettingsReader::~SettingsReader() = default;

QList<DeviceSettings> SettingsReader::getDeviceSettings() const noexcept
{
    auto const deviceCount = mSettingsBackend->getValue(DeviceSettingsSize);
    if (not deviceCount.isValid()) {
        return {};
    }
    auto devices = QList<DeviceSettings>{};
    devices.resize(deviceCount.toUInt());
    for (qsizetype i = 0; i < deviceCount.toUInt(); ++i) {
        auto const deviceName = mSettingsBackend->getValue(DeviceSettingsName.toString().arg(i));
        if (not deviceName.isValid()) {
            return {};
        }
        devices[i].name = deviceName.toString();
        auto const deviceIp = mSettingsBackend->getValue(DeviceSettingsIp.toString().arg(i));
        if (not deviceIp.isValid()) {
            return {};
        }
        auto ip = QHostAddress{deviceIp.toString()};
        if (ip.isNull()) {
            return {};
        }
        devices[i].ip = ip;
        auto const port = mSettingsBackend->getValue(DeviceSettingsPort.toString().arg(i));
        if (not port.isValid() and not port.canConvert<quint16>()) {
            return {};
        }
        devices[i].port = port.value<quint16>();
        auto const defaultDevice = mSettingsBackend->getValue(DeviceSettingsDef.toString().arg(i));
        if (not port.isValid() and not port.canConvert<bool>()) {
            return {};
        }
        devices[i].defaultDevice = defaultDevice.toBool();
    }
    return devices;
}

} // namespace RapidAndroid::Common
