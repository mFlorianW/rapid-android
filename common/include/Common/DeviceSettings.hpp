// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_COMMON_DEVICESETTINGS_HPP
#define RAPIDANDROID_COMMON_DEVICESETTINGS_HPP

#include <QHostAddress>
#include <QQmlEngine>

namespace RapidAndroid::Common
{

/**
 * Global Device settings for a laptimer.
 */
class DeviceSettings
{
    Q_GADGET
    QML_VALUE_TYPE(deviceSettings)

    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(QString ip READ getIpAddress WRITE setIpAddress)
    Q_PROPERTY(quint16 port MEMBER port)
    Q_PROPERTY(quint16 defaultDevice MEMBER defaultDevice)

public:
    QString name;
    QHostAddress ip;
    quint16 port;
    bool defaultDevice = false;

    QString getIpAddress() const noexcept
    {
        return ip.toString();
    }

    void setIpAddress(QString const ipString) noexcept
    {
        auto const address = QHostAddress{ipString};
        if (not address.isNull()) {
            ip = address;
        }
    }

    bool operator==(DeviceSettings const& rhs) const = default;
    bool operator!=(DeviceSettings const& rhs) const = default;
};

} // namespace RapidAndroid::Common

#endif // RAPIDANDROID_COMMON_DEVICESETTINGS_HPP
