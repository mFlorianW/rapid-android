// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_TIMEFORMATTER_HPP
#define RAPIDANDROID_TIMEFORMATTER_HPP

#include <Common/TimeFormatter.hpp>
#include <QtQmlIntegration/qqmlintegration.h>

namespace RapidAndroid
{

/**
 * @brief QML singleton wrapper for time formatting utilities.
 */
class TimeFormatter : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    Q_DISABLE_COPY_MOVE(TimeFormatter)

    /**
     * @brief Constructor.
     *
     * @param parent Parent QObject.
     */
    explicit TimeFormatter() = default;

    /**
     * @brief Destructor.
     */
    ~TimeFormatter() override = default;

    /**
     * @brief Formats a QTime representing a lap time into a human-readable string.
     * @details The formatted string will only contain values that are greater than zero.
     *
     * For example:
     * - A time of 01:02:03.456 will be formatted as "01:02:03.456"
     * - A time of 00:01:23.456 will be formatted as "01:23.456"
     * - A time of 00:00:45.678 will be formatted as "45.678"
     * - A time of 00:00:00.123 will be formatted as "00.123"
     *
     * @param time The QTime to format.
     * @return Formatted lap time string.
     */
    Q_INVOKABLE static QString formatTime(QTime const& time) noexcept;
};

} // namespace RapidAndroid

#endif // RAPIDANDROID_TIMEFORMATTER_HPP
