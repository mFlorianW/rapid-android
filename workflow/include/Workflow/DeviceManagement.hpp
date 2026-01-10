// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_WORKFLOW_DEVICEMANAGEMENT
#define RAPIDANDROID_WORKFLOW_DEVICEMANAGEMENT

// #include <Common/SettingsListModel.hpp>
#include <QObject>
#include <Workflow/IDeviceManagement.hpp>
// #include <Work

class QAbstractItemModel;

namespace RapidAndroid::Common
{
class SettingsWriter;
class SettingsReader;
class SettingsBackend;
class DeviceSettingsListModel;
} // namespace RapidAndroid::Common

namespace RapidAndroid::Workflow
{

/**
 * @brief DeviceManagement impelmentation with QSettings backend.
 *
 * @details Every application should have only one instance of the DeviceManagement because the current impelmentation doesn't support multiple instances.
 */
class DeviceManagement : public IDeviceManagement
{
    Q_OBJECT

public:
    /**
     * Creates an instnace of DeviceManagement
     *
     */
    DeviceManagement(Common::SettingsBackend* settingsBackend);

    /**
     * Default destructor
     */
    ~DeviceManagement() override;

    /**
     * Disabled copy constructor
     */
    DeviceManagement(DeviceManagement const&) = delete;

    /**
     * Disablbed copy assignment operator
     */
    DeviceManagement& operator=(DeviceManagement const&) = delete;

    /**
     * Disabled move constructor
     */
    DeviceManagement(DeviceManagement&&) noexcept = delete;

    /**
     * Disablbed move assignment operator
     */
    DeviceManagement& operator=(DeviceManagement&&) noexcept = delete;

    /**
     * @copydoc IDeviceManagement::getModel
     */
    [[nodiscard]] QAbstractItemModel const* const getModel() const noexcept override;

    /**
     * @copydoc IDeviceManagement::getActiveLaptimer
     */
    [[nodiscard]] RapidAndroid::Common::DeviceSettings getActiveLaptimer() const noexcept override;

    /**
     * @copydoc IDeviceManagement::store
     */
    Q_INVOKABLE bool store(RapidAndroid::Common::DeviceSettings const& device) noexcept override;

    /**
     * @copydoc IDeviceManagement::remove
     */
    Q_INVOKABLE bool remove(RapidAndroid::Common::DeviceSettings const& device) noexcept override;

    /**
     * @copydoc IDeviceManagement::update
     */
    Q_INVOKABLE bool update(RapidAndroid::Common::DeviceSettings const& oldDevice,
                            RapidAndroid::Common::DeviceSettings const& newDevice) noexcept override;

    /**
     * @copydoc IDeviceManagement::enable
     */
    Q_INVOKABLE bool enable(RapidAndroid::Common::DeviceSettings device) noexcept;

private:
    Common::SettingsBackend* mSettingsBackend{nullptr};
    std::unique_ptr<Common::SettingsReader> mGlobalSettingsReader;
    std::unique_ptr<Common::SettingsWriter> mGlobalSettingsWriter;
    std::unique_ptr<Common::DeviceSettingsListModel> mModel;
};

} // namespace RapidAndroid::Workflow

#endif // RAPIDANDROID_WORKFLOW_DEVICEMANAGEMENT
