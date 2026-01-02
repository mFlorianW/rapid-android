// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_GLOBALCONTEXT_HPP
#define RAPIDANDROID_GLOBALCONTEXT_HPP

#include "FileSettingsBackend.hpp"
#include <Common/DeviceSettings.hpp>
#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>
#include <RapidSession/FilesystemStorage.hpp>
#include <RapidSession/SessionJsonDeserializer.hpp>
#include <RapidSession/SessionJsonSerializer.hpp>
#include <Workflow/DeviceManagement.hpp>
#include <Workflow/HttpDeviceSessionManagement.hpp>
#include <Workflow/LocalSessionManagement.hpp>
#include <memory>

namespace RapidAndroid
{

/**
 * @brief Global context for the RapidAndroid application.
 *
 * Exposes application-wide services and workflows to C++ and QML.
 * Registered as a QML element and singleton to be accessible as a single shared instance from QML.
 */
class GlobalContext : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    /**
     * @property Rapid::Workflow::Qt::DeviceManagement*
     *
     * Gives the DeviceManagement workflow for managing the laptimer.
     */
    Q_PROPERTY(RapidAndroid::Workflow::IDeviceManagement* deviceManagement READ getDeviceManagement CONSTANT)

    /**
     * @property RapidAndroid::Workflow::IDeviceSessionManagement*
     *
     * Gives the DeviceSessionManagement workflow for managing device sessions.
     */
    Q_PROPERTY(RapidAndroid::Workflow::IDeviceSessionManagement* deviceSessionManagement READ getDeviceSessionManagement
                   CONSTANT)

    /**
     * @property RapidAndroid::Workflow::ILocalSessionManagement*
     *
     * Gives the LocalSessionManagement workflow for managing local sessions.
     */
    Q_PROPERTY(
        RapidAndroid::Workflow::ILocalSessionManagement* localSessionManagement READ getLocalSessionManagement CONSTANT)

public:
    Q_DISABLE_COPY_MOVE(GlobalContext)

    /**
     * @brief Construct the global context.
     */
    GlobalContext();

    /**
     * @brief Destroy the global context.
     */
    ~GlobalContext() override;

    /**
     * @brief Get the device-management workflow.
     * @return Non-null pointer owned by GlobalContext. Do not delete.
     */
    Workflow::DeviceManagement* getDeviceManagement() const noexcept;

    /**
     * @brief Get the device-session-management workflow.
     * @return Non-null pointer owned by GlobalContext. Do not delete.
     */
    RapidAndroid::Workflow::IDeviceSessionManagement* getDeviceSessionManagement() const noexcept;

    /**
     * @brief Get the local-session-management workflow.
     * @return Non-null pointer owned by GlobalContext. Do not delete.
     */
    Workflow::ILocalSessionManagement* getLocalSessionManagement() noexcept;

private:
    FileSettingsBackend mSettingsBackend;
    std::unique_ptr<Workflow::DeviceManagement> mDeviceManagement;

    using Storage = Session::FilesystemStorage<Session::SessionJsonSerializer, Session::SessionJsonDeserializer>;
    Session::SessionJsonSerializer mSessionSerializer;
    Session::SessionJsonDeserializer mSessionJsonDeserializer;
    Storage mSessionStorage;

    using DeviceSessionMgmt = Workflow::HttpDeviceSessionManagement<Session::SessionJsonDeserializer, Storage>;
    RapidAndroid::Session::SessionJsonDeserializer mSessionDeserializer;
    std::unique_ptr<DeviceSessionMgmt> mDeviceSessionManagement;

    using LocalSessionMgmt = Workflow::LocalSessionManagement<Storage>;
    std::unique_ptr<LocalSessionMgmt> mLocalSessionManagement;
};

} // namespace RapidAndroid

#endif // RAPIDANDROID_GLOBALCONTEXT_HPP
