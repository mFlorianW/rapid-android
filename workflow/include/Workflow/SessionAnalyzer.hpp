// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_WORKFLOW_SESSIONANALYZER_HPP
#define RAPIDANDROID_WORKFLOW_SESSIONANALYZER_HPP

#include <Workflow/ISessionAnalyzer.hpp>
#include <Workflow/Private/LapListModel.hpp>

namespace RapidAndroid::Workflow
{

/**
 * @copydoc ISessionAnalyzer
 */
class SessionAnalyzer : public ISessionAnalyzer
{
    Q_OBJECT
public:
    Q_DISABLE_COPY_MOVE(SessionAnalyzer)

    /**
     * @brief Default constructor.
     */
    SessionAnalyzer();

    /**
     * @brief Default destructor.
     */
    ~SessionAnalyzer() override;

    /**
     * @copydoc ISessionAnalyzer::getLapListModel
     */
    QAbstractListModel* getLapListModel() noexcept override;

    /**
     * @copydoc ISessionAnalyzer::analyzeSession
     */
    void analyzeSession(Common::Session const& session) noexcept override;

private:
    LapListModel mLapModel;
};

} // namespace RapidAndroid::Workflow

#endif // RAPIDANDROID_WORKFLOW_SESSIONANALYZER_HPP
