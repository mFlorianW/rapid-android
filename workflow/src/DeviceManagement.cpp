// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "LoggingCategories.hpp"
#include <Common/DeviceSettingsListModel.hpp>
#include <Common/SettingsBackend.hpp>
#include <Common/SettingsReader.hpp>
#include <Common/SettingsWriter.hpp>
#include <QDebug>
#include <Workflow/DeviceManagement.hpp>

using namespace RapidAndroid::Common;

namespace RapidAndroid::Workflow
{

DeviceManagement::DeviceManagement(Common::SettingsBackend* settingsBackend)
    : IDeviceManagement{}
    , mSettingsBackend{settingsBackend}
    , mGlobalSettingsReader{std::make_unique<Common::SettingsReader>(settingsBackend)}
    , mGlobalSettingsWriter{std::make_unique<Common::SettingsWriter>(settingsBackend)}
    , mModel{std::make_unique<DeviceSettingsListModel>()}
{
    auto const devices = mGlobalSettingsReader->getDeviceSettings();
    if (devices.isEmpty()) {
        auto const defaultDevice = DeviceSettings{.name = QStringLiteral("Rapid"),
                                                  .ip = QHostAddress{"192.168.1.1"},
                                                  .port = 27018,
                                                  .defaultDevice = true};
        if (not store(defaultDevice)) {
            qCCritical(devicemanagement) << "Failed to store default laptimer.";
        } else {
            qCInfo(devicemanagement) << "No laptimer device found. Created default laptimer device:" << defaultDevice;
        }
    } else {
        for (auto const& device : devices) {
            mModel->insertItem(device);
        }
        qCInfo(devicemanagement) << "Laptimer configuration successful loaded.";
    }
}

DeviceManagement::~DeviceManagement() = default;

QAbstractItemModel const* const DeviceManagement::getModel() const noexcept
{
    return mModel.get();
}

Common::DeviceSettings DeviceManagement::getActiveLaptimer() const noexcept
{
    auto devices = mGlobalSettingsReader->getDeviceSettings();
    for (auto const& dev : std::as_const(devices)) {
        if (dev.defaultDevice) {
            return dev;
        }
    }
    return devices[0];
}

bool DeviceManagement::store(Common::DeviceSettings const& device) noexcept
{
    auto devices = mGlobalSettingsReader->getDeviceSettings();
    if (devices.contains(device)) {
        qCCritical(devicemanagement) << "Don't store laptimer configuration. Reason: Already stored";
        return false;
    }
    devices.append(device);
    if (mGlobalSettingsWriter->storeDeviceSettings(devices)) {
        mModel->insertItem(device);
        qCInfo(devicemanagement) << "Successful stored laptimer configuration: {}" << device;
        return true;
    }
    qCCritical(devicemanagement) << "Failed to store laptimer configuration. Unknwon error";
    return false;
}

bool DeviceManagement::remove(Common::DeviceSettings const& device) noexcept
{
    auto devices = mGlobalSettingsReader->getDeviceSettings();
    devices.removeAll(device);
    if (not mGlobalSettingsWriter->storeDeviceSettings(devices)) {
        qCritical(devicemanagement) << "Failed to store device configuration. Unknwon error";
        return false;
    }
    if (not mModel->removeItem(device)) {
        qCCritical(devicemanagement) << "Failed to remove laptimer configuration. Unknwon error";
        return false;
    }
    if (device.defaultDevice and not devices.empty() and not enable(devices[0])) {
        return false;
    }
    qCCritical(devicemanagement) << "Successful removed laptimer configuration: " << device;
    return true;
}

bool DeviceManagement::update(Common::DeviceSettings const& oldDevice, Common::DeviceSettings const& newDevice) noexcept
{
    auto devices = mGlobalSettingsReader->getDeviceSettings();
    bool updated = false;
    for (auto& device : devices) {
        if (device == oldDevice) {
            device = newDevice;
            updated = true;
        }
    }
    if (not updated) {
        qCCritical(devicemanagement) << "Failed to updated device configuration. No Laptimer found";
        return false;
    }
    if (not mGlobalSettingsWriter->storeDeviceSettings(devices)) {
        qCCritical(devicemanagement) << "Failed to store updated device configuration. Unknwon error";
        return false;
    }
    if (not mModel->updateItem(oldDevice, newDevice)) {
        qCCritical(devicemanagement) << "Failed to store updated device configuration in the model. Unknown error";
        return false;
    }
    qCInfo(devicemanagement) << "Successful update laptimer configuration:" << oldDevice << " with " << newDevice;
    return true;
}

Q_INVOKABLE bool DeviceManagement::enable(Common::DeviceSettings device) noexcept
{
    auto devices = mGlobalSettingsReader->getDeviceSettings();
    for (auto& dev : devices) {
        if (dev == device) {
            qCCritical(devicemanagement) << "Successful enabled " << device;
            dev.defaultDevice = true;
        } else {
            dev.defaultDevice = false;
        }
    }
    if (not mGlobalSettingsWriter->storeDeviceSettings(devices)) {
        qCCritical(devicemanagement) << "Failed to store enabled devie(s). Unknown error";
        return false;
    }
    for (qint32 i = 0; i < devices.size(); ++i) {
        if (not mModel->updateItem(i, devices[i])) {
            return false;
        }
    }
    qCCritical(devicemanagement) << "Successful stored enabled" << device;
    Q_EMIT activeLaptimerChanged();
    return true;
}

} // namespace RapidAndroid::Workflow
