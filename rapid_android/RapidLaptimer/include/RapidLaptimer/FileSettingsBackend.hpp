// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_RAPIDANDROID_FILESETTINGSBACKEND_HPP
#define RAPIDANDROID_RAPIDANDROID_FILESETTINGSBACKEND_HPP

#include <Common/SettingsBackend.hpp>
#include <QSettings>

namespace RapidAndroid
{

/**
 * @brief Persistent settings backend backed by QSettings.
 *
 * Implements Common::SettingsBackend to store and retrieve key-value pairs using QSettings.
 * Keys are provided as QAnyStringView and values as QVariant.
 */
class FileSettingsBackend : public Common::SettingsBackend
{
public:
    /**
     * Creates an instance of FileSettingsBackend.
     */
    FileSettingsBackend();

    /**
     * Default destructor.
     */
    ~FileSettingsBackend() override;

    /**
     * Disabled copy constructor.
     */
    FileSettingsBackend(FileSettingsBackend const&) = delete;

    /**
     * Disabled copy assignment operator.
     */
    FileSettingsBackend& operator=(FileSettingsBackend const&) = delete;

    /**
     * Default move constructor.
     */
    FileSettingsBackend(FileSettingsBackend&&) noexcept = default;

    /**
     * Default move assignment operator.
     */
    FileSettingsBackend& operator=(FileSettingsBackend&&) noexcept = default;

    /**
     * @copydoc Common::SettingsBackend::storeValue
     */
    [[nodiscard]] bool storeValue(QAnyStringView const& key, QVariant const& value) noexcept override;

    /**
     * @copydoc Common::SettingsBackend::getValue
     */
    [[nodiscard]] QVariant getValue(QAnyStringView const& key) const noexcept override;

private:
    std::unique_ptr<QSettings> mSettings;
};

} // namespace RapidAndroid

#endif // RAPIDANDROID_RAPIDANDROID_FILESETTINGSBACKEND_HPP
