// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_WORKFLOW_SESSIONLISTMODEL_HPP
#define RAPIDANDROID_WORKFLOW_SESSIONLISTMODEL_HPP

#include <Common/GenericListModel.hpp>
#include <Common/SessionInfo.hpp>

namespace RapidAndroid::Workflow
{

class SessionListModel : public Common::GenericListModel<Common::SessionInfo>
{
    Q_OBJECT
public:
    Q_DISABLE_COPY_MOVE(SessionListModel)

    /**
     * @brief The custom display roles supported by the model
     * @details The main purpose for the roles are for the QML context
     */
    enum DisplayRole
    {
        SessionInfo = ::Qt::UserRole + 1,
    };
    Q_ENUM(DisplayRole)

    SessionListModel();
    ~SessionListModel() override;

    QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const noexcept override;
};

} // namespace RapidAndroid::Workflow

#endif // RAPIDANDROID_WORKFLOW_SESSIONLISTMODEL_HPP
