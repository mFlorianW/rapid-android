// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPID_COMMON_QT_SETTINGSBACKEND_HPP
#define RAPID_COMMON_QT_SETTINGSBACKEND_HPP

#include <QAnyStringView>

namespace RapidAndroid::Common
{

class SettingsBackend
{
public:
    /**
     * Default destructor
     */
    virtual ~SettingsBackend() = default;

    /**
     * Deleted copy constructor
     */
    SettingsBackend(SettingsBackend const&) = delete;

    /**
     * Deleted copy operator
     */
    SettingsBackend& operator=(SettingsBackend const&) = delete;

    /**
     * Deleted copy constructor
     */
    SettingsBackend(SettingsBackend&&) noexcept = default;

    /**
     * Deleted move operator
     */
    SettingsBackend& operator=(SettingsBackend&&) noexcept = default;

    /**
     * Stores a value under the given key.
     * @param key The key under which the value shall be stored to load it later.
     * @param value The value that shall be stored.
     * @return true Successful stored.
     *         false Failed to store.
     */
    [[nodiscard]] virtual bool storeValue(QAnyStringView const& key, QVariant const& value) noexcept = 0;

    /**
     * Gives the stored value for the passed key.
     * If the key is not found an invalid QVariant is returned.
     * @param key The key of the settings parameter
     * @return Success The value in a QVariant
     *         Failure An invalud QVariant
     */
    [[nodiscard]] virtual QVariant getValue(QAnyStringView const& key) const noexcept = 0;

protected:
    SettingsBackend() = default;
};

} // namespace RapidAndroid::Common

#endif // RAPID_COMMON_QT_SETTINGSBACKEND_HPP
