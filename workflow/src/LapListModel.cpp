// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Workflow/Private/LapListModel.hpp"

namespace RapidAndroid::Workflow
{

LapListModel::LapListModel()
    : GenericListModel<Common::Lap>{{{DisplayRole::Laptime, "laptime"}}}
{
}

LapListModel::LapListModel(std::vector<Common::Lap> const& laps) noexcept
    : LapListModel{}
{
    for (auto const& lap : std::as_const(laps)) {
        insertItem(lap);
    }
}

LapListModel::~LapListModel() = default;

QVariant LapListModel::data(QModelIndex const& index, int role) const noexcept
{
    auto lap = getElement(static_cast<std::size_t>(index.row()));
    if (lap.has_value() and role == DisplayRole::Laptime) {
        return (*lap)->laptime().toString("mm:ss.zzz");
    }
    return {};
}

} // namespace RapidAndroid::Workflow
