// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "Common/DeviceSettings.hpp"
#include <QString>

namespace RapidAndroid::Common
{
class SettingsBackend;

/**
 * Reads the global settings from the provided backend.
 */
class SettingsReader
{
public:
    /**
     * Creates an instance of the GlobalSettingsReader
     * @param settingsBackend The settings backend for the store and load operations
     */
    SettingsReader(SettingsBackend* settingsBackend);

    /**
     * Default destructor
     */
    ~SettingsReader();

    /**
     * Deleted copy constructor
     */
    SettingsReader(SettingsReader const&);

    /**
     * Deleted copy operator
     */
    SettingsReader& operator=(SettingsReader const&);

    /**
     * Deleted move constructor
     */
    SettingsReader(SettingsReader const&&) noexcept;

    /**
     * Deleted move operator
     */
    SettingsReader& operator=(SettingsReader&&) noexcept;

    /**
     * Gives the stored device settings in the stored backend.
     * If no settings are stored the device settings will be empty.
     * @return Success: The stored device settings.
     *         Error or not found: An empty list is returned.
     */
    QList<DeviceSettings> getDeviceSettings() const noexcept;

private:
    SettingsBackend* mSettingsBackend;
};

} // namespace RapidAndroid::Common
