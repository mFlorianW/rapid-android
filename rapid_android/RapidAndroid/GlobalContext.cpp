// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "GlobalContext.hpp"
#include "LoggingCategories.hpp"
#include <QCoreApplication>
#include <QDebug>
#include <QStandardPaths>

namespace RapidAndroid
{

struct Wf
{
    Q_GADGET
    QML_FOREIGN(RapidAndroid::Workflow::IDeviceSessionManagement)
    QML_NAMED_ELEMENT(DeviceSessionManagement)
    QML_UNCREATABLE("Interface class - an instance of it is provided by a global context.")
};

std::filesystem::path const sessionStoragePath()
{
    auto const dataLocation = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    if (dataLocation.isEmpty()) {
        qFatal("Failed to determine writable location for session storage.");
    }
    auto path = std::filesystem::path{dataLocation.toStdString()} /
                QCoreApplication::instance()->applicationName().replace(' ', '_').toStdString() / "sessions";
    if (not std::filesystem::exists(path) and not std::filesystem::create_directories(path)) {
        qFatal("Failed to create session storage directory at %s", path.string().c_str());
    }
    qCInfo(gContext) << "Using session storage path:" << QString::fromStdString(path.string());
    return path;
}

GlobalContext::GlobalContext()
    : mDeviceManagement(std::make_unique<Workflow::DeviceManagement>(&mSettingsBackend))
    , mSessionStorage{sessionStoragePath(), &mSessionSerializer}
    , mDeviceSessionManagement{std::make_unique<DeviceSessionMgmt>(&mSessionDeserializer, &mSessionStorage)}
{
}

GlobalContext::~GlobalContext() = default;

Workflow::DeviceManagement* GlobalContext::getDeviceManagement() const noexcept
{
    return mDeviceManagement.get();
}

RapidAndroid::Workflow::IDeviceSessionManagement* GlobalContext::getDeviceSessionManagement() const noexcept
{
    return mDeviceSessionManagement.get();
}

} // namespace RapidAndroid

#include "GlobalContext.moc"
