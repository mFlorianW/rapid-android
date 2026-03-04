// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <Workflow/SessionAnalyzer.hpp>
#include <ranges>

namespace RapidAndroid::Workflow
{

SessionAnalyzer::SessionAnalyzer() = default;
SessionAnalyzer::~SessionAnalyzer() = default;

QAbstractListModel* SessionAnalyzer::getLapListModel() noexcept
{
    return std::addressof(mLapModel);
}

void SessionAnalyzer::analyzeSession(Common::Session const& session) noexcept
{
    mLapModel.clear();
    auto const laps = session.getLaps();
    for (auto const& lap : laps) {
        mLapModel.insertItem(lap);
    }
    mBestLapTime = std::ranges::min(laps, {}, &Common::Lap::laptime).laptime();
    Q_EMIT sessionAnalyzed();
}

QTime SessionAnalyzer::getBestLapTime() const noexcept
{
    return mBestLapTime;
}

} // namespace RapidAndroid::Workflow
