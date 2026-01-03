// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_WORKFLOW_ISESSIONANALYZER_HPP
#define RAPIDANDROID_WORKFLOW_ISESSIONANALYZER_HPP

#include <Common/Session.hpp>
#include <QAbstractListModel>
#include <QObject>

namespace RapidAndroid::Workflow
{

class ISessionAnalyzer : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QAbstractListModel* lapListModel READ getLapListModel CONSTANT)

public:
    Q_DISABLE_COPY_MOVE(ISessionAnalyzer)

    /**
     * @brief Default destructor.
     */
    ~ISessionAnalyzer() override = default;

    /**
     * @brief Returns the model containing lap data.
     * @return A pointer to the QAbstractListModel containg the laps of the analyzed session.
     */
    virtual QAbstractListModel* getLapListModel() noexcept = 0;

    /**
     * @brief Analyzes the provided session data.
     * @details Updates the lap time model with the laps of the passed session.
     * @param session The session data to be analyzed.
     */
    Q_INVOKABLE virtual void analyzeSession(RapidAndroid::Common::Session const& session) noexcept = 0;

Q_SIGNALS:
    /**
     * @brief Emitted when the analysis of a session is completed.
     */
    void sessionAnalyzed();

protected:
    ISessionAnalyzer() = default;
};

} // namespace RapidAndroid::Workflow

#endif // RAPIDANDROID_WORKFLOW_ISESSIONANALYZER_HPP
