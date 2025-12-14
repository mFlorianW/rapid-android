// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later
#ifndef RAPID_COMMON_SETTINGSKEYS_HPP
#define RAPID_COMMON_SETTINGSKEYS_HPP

#include <QString>

/**
 * @file
 * This file contains the keys for the global settings
 */

namespace RapidAndroid::Common
{

/**
 * This key stores the amount of device settings in the global settings backend.
 */
constexpr auto DeviceSettingsSize = QStringView(u"deviceSettingsSize");

/**
 * This key is used to store the device settings name in the global settings it contais a place holder for the
 * setttings index.
 */
constexpr auto DeviceSettingsName = QStringView(u"deviceSettings%1Name");

/**
 * This key is used to store the device settings ip in the global settings it contais a place holder for the setttings
 * index.
 */
constexpr auto DeviceSettingsIp = QStringView(u"deviceSettings%1Ip");

/**
 * This key is used to store the device settings port in the global settings it contais a place holder for the
 * setttings index.
 */
constexpr auto DeviceSettingsPort = QStringView(u"deviceSettings%1Port");

/**
 * This key is used to store the device settings default device option in the global settings it contais a place holder
 * for the setttings index.
 */
constexpr auto DeviceSettingsDef = QStringView(u"deviceSettings%1Default");

} // namespace RapidAndroid::Common

#endif // RAPID_COMMON_SETTINGSKEYS_HPP
