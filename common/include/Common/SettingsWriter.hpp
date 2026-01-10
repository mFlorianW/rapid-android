// SPDX-FileCopyrightText: 2024-2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_COMMON_SETTINGSWRITER_HPP
#define RAPIDANDROID_COMMON_SETTINGSWRITER_HPP

#include <QSettings>
#include <QVariant>

namespace RapidAndroid::Common
{
class SettingsBackend;
class DeviceSettings;

/**
 * Write only interfacce for the global settings of the Shell
 */
class SettingsWriter
{
public:
    /**
     * Constructs a @ref Rapid::RapidShell::Settings instance
     * @param settingsBackend The settings that execute the concret store operations
     *                        The Settings doesn't take the ownership of the pointer so the caller must
     *                        guarantee that the object is valid as long the Settings class exists.
     */
    SettingsWriter(SettingsBackend* settingsBackend);

    /**
     * Default destructor
     */
    ~SettingsWriter();

    /**
     * Deleted copy operator
     */
    SettingsWriter& operator=(SettingsWriter const&) = delete;

    /**
     * Deleted copy constructor
     */
    SettingsWriter(SettingsWriter const&) = delete;

    /**
     * Deleted move operator
     */
    SettingsWriter& operator=(SettingsWriter&&) noexcept = delete;

    /**
     * Deleted move constructor
     */
    SettingsWriter(SettingsWriter const&&) noexcept = delete;

    /**
     * Stores the device settings
     * @param deviceSettings The list of device settings that shall be stored.
     * @return true the successful stored
     * @return false Failed to store
     */
    [[nodiscard]] bool storeDeviceSettings(QList<DeviceSettings> const& deviceSettings) noexcept;

private:
    SettingsBackend* mSettingsBackend;
};

} // namespace RapidAndroid::Common

#endif // RAPIDANDROID_COMMON_SETTINGSWRITER_HPP
