// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Workflow/Private/SessionListModel.hpp"

namespace RapidAndroid::Workflow
{

SessionListModel::SessionListModel()
    : Common::GenericListModel<Common::SessionInfo>{
          QHash<qint32, QByteArray>{{DisplayRole::SessionInfo, "sessionInfo"}, {Qt::DisplayRole, "display"}}}
{
}

SessionListModel::~SessionListModel() = default;

QVariant SessionListModel::data(QModelIndex const& index, int role) const noexcept
{
    if (not index.isValid() or index.row() < 0 or index.row() >= rowCount(QModelIndex{})) {
        return {};
    }
    auto element = getElement(static_cast<std::size_t>(index.row()));
    if (not element.has_value()) {
        return {};
    }
    if (role == ::Qt::DisplayRole) {
        return QVariant::fromValue(*element);
    } else if (role == DisplayRole::SessionInfo) {
        return QVariant::fromValue(*element.value());
    }
    return {};
}

} // namespace RapidAndroid::Workflow
