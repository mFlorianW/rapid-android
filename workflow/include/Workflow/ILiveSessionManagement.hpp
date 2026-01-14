// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_WORKFLOW_ISESSIONLIVEVIEW_HPP
#define RAPIDANDROID_WORKFLOW_ISESSIONLIVEVIEW_HPP

#include <Common/DeviceSettings.hpp>
#include <QObject>
#include <QTime>

namespace RapidAndroid::Workflow
{

class ILiveSessionManagement : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QTime currentLaptime READ getCurrentLaptime NOTIFY currentLaptimeChanged)

    Q_PROPERTY(QTime lastLaptime READ getLastLaptime NOTIFY lastLaptimeChanged)

    Q_PROPERTY(int lapCount READ getLapCount NOTIFY currentLapChanged)

public:
    Q_DISABLE_COPY_MOVE(ILiveSessionManagement)

    ~ILiveSessionManagement() override = default;

    [[nodiscard]] virtual QTime getCurrentLaptime() const noexcept = 0;

    [[nodiscard]] virtual QTime getLastLaptime() const noexcept = 0;

    [[nodiscard]] virtual QTime getBestLaptime() const noexcept = 0;

    [[nodiscard]] virtual int getLapCount() const noexcept = 0;

    Q_INVOKABLE virtual void setDeviceSettings(RapidAndroid::Common::DeviceSettings const& settings) = 0;

Q_SIGNALS:
    void currentLaptimeChanged();
    void lastLaptimeChanged();
    void bestLaptimeChanged();
    void currentLapChanged();

protected:
    ILiveSessionManagement() = default;
};

} // namespace RapidAndroid::Workflow

#endif // RAPIDANDROID_WORKFLOW_ISESSIONLIVEVIEW_HPP
