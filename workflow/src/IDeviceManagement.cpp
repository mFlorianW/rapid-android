// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Workflow/IDeviceManagement.hpp"
#include "LoggingCategories.hpp"

using namespace RapidAndroid::Common;

namespace RapidAndroid::Workflow
{

RapidAndroid::Common::DeviceSettings IDeviceManagement::create(QString const& name,
                                                               QString const& ip,
                                                               QString port,
                                                               bool enabled) noexcept
{
    auto device = DeviceSettings{.name = name, .port = static_cast<quint16>(port.toUInt()), .defaultDevice = enabled};
    device.setIpAddress(ip);
    qCInfo(devicemanagement) << "Created device configuration:" << device;
    return device;
}

} // namespace RapidAndroid::Workflow
