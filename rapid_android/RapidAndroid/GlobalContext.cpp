// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "GlobalContext.hpp"

namespace RapidAndroid
{

GlobalContext::GlobalContext()
    : devieManagement(std::make_unique<Workflow::DeviceManagement>(&mSettingsBackend))
{
}

GlobalContext::~GlobalContext() = default;

Workflow::DeviceManagement* GlobalContext::getDeviceManagement() const noexcept
{
    return devieManagement.get();
}

} // namespace RapidAndroid
