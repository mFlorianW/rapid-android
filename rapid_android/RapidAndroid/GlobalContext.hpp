// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_GLOBALCONTEXT_HPP
#define RAPIDANDROID_GLOBALCONTEXT_HPP

#include "FileSettingsBackend.hpp"
#include <Common/DeviceSettings.hpp>
#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>
#include <Workflow/DeviceManagement.hpp>
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
    Q_PROPERTY(Workflow::DeviceManagement* deviceManagement READ getDeviceManagement CONSTANT)

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

private:
    FileSettingsBackend mSettingsBackend;
    std::unique_ptr<Workflow::DeviceManagement> devieManagement;
};

} // namespace RapidAndroid

#endif // RAPIDANDROID_GLOBALCONTEXT_HPP
