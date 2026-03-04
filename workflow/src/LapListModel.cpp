// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Workflow/Private/LapListModel.hpp"
#include <Common/TimeFormatter.hpp>

namespace RapidAndroid::Workflow
{

LapListModel::LapListModel()
    : GenericListModel<Common::Lap>{{{DisplayRole::Laptime, "laptime"}, {DisplayRole::SectorTimes, "sectorTimes"}}}
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
    if (not lap.has_value()) {
        return {};
    }
    switch (static_cast<DisplayRole>(role)) {
    case DisplayRole::SectorTimes: {
        QStringList sectors;
        for (auto const& sector : (*lap)->sectors) {
            sectors.append(Common::TimeFormatter::formatTime(sector));
        }
        return sectors;
        break;
    }
    case DisplayRole::Laptime:
        return Common::TimeFormatter::formatTime((*lap)->laptime());
        break;
    }
    return {};
}

} // namespace RapidAndroid::Workflow
