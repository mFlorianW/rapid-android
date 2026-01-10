// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "RapidSession/FilesystemStorage.hpp"
#include "LoggingCategories.hpp"

namespace RapidAndroid::Session
{

QLoggingCategory const& fsLogCat()
{
    return fsStorageLog();
}

} // namespace RapidAndroid::Session
