// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <Workflow/SessionAnalyzer.hpp>

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
    Q_EMIT sessionAnalyzed();
}

} // namespace RapidAndroid::Workflow
