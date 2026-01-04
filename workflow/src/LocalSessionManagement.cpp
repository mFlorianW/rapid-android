// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Workflow/LocalSessionManagement.hpp"
#include "LoggingCategories.hpp"

namespace RapidAndroid::Workflow
{

QLoggingCategory const& lsmLog()
{
    return lsm();
}

} // namespace RapidAndroid::Workflow
