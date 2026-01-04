// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Workflow/Private/SessionSortModel.hpp"
#include "Workflow/Private/SessionListModel.hpp"
#include <Common/SessionInfo.hpp>
#include <QTimeZone>

namespace RapidAndroid::Workflow
{

SessionSortModel::SessionSortModel() noexcept
    : QSortFilterProxyModel{}
{
    setSortRole(SessionListModel::SessionInfo);
    setDynamicSortFilter(true);
    sort(0, Qt::DescendingOrder);
}

bool SessionSortModel::lessThan(QModelIndex const& left, QModelIndex const& right) const
{
    auto const leftSessionInfo = left.data(SessionListModel::SessionInfo).value<Common::SessionInfo>();
    auto const rightSessionInfo = right.data(SessionListModel::SessionInfo).value<Common::SessionInfo>();
    return leftSessionInfo.date < rightSessionInfo.date;
}

} // namespace RapidAndroid::Workflow
