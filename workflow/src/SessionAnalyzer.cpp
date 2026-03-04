// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <Common/Lap.hpp>
#include <Workflow/SessionAnalyzer.hpp>
#include <ranges>

namespace RapidAndroid::Workflow
{

namespace
{

qreal calculateTopSpeed(QVector<Common::Lap> const& laps) noexcept
{
    auto velocities = laps | std::views::transform([](auto const& lap) {
                          return std::ranges::max(lap.logPoints, {}, &Common::GnssPosition::velocity).velocity;
                      });
    return std::ranges::max(velocities);
}

} // namespace

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
    mTopSpeed = calculateTopSpeed(laps);
    Q_EMIT sessionAnalyzed();
}

QTime SessionAnalyzer::getBestLapTime() const noexcept
{
    return mBestLapTime;
}

qreal SessionAnalyzer::getTopSpeed() const noexcept
{
    return mTopSpeed;
}

} // namespace RapidAndroid::Workflow
