// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_WORKFLOW_ISESSIONMANAGEMENT_HPP
#define RAPIDANDROID_WORKFLOW_ISESSIONMANAGEMENT_HPP

#include <QObject>

namespace RapidAndroid::Workflow
{

class ISessionManagement : public QObject
{
    Q_OBJECT
public:
    Q_DISABLE_COPY_MOVE(ISessionManagement)

    ~ISessionManagement() override = default;

protected:
    ISessionManagement() = default;
};

} // namespace RapidAndroid::Workflow

#endif // RAPIDANDROID_WORKFLOW_ISESSIONMANAGEMENT_HPP
