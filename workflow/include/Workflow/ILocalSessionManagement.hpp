// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_WORKFLOW_ISESSIONMANAGEMENT_HPP
#define RAPIDANDROID_WORKFLOW_ISESSIONMANAGEMENT_HPP

#include <Common/Session.hpp>
#include <Common/SessionInfo.hpp>
#include <QAbstractListModel>
#include <QObject>

namespace RapidAndroid::Workflow
{
/**
 * @class RapidAndroid::Workflow::ILocalSessionManagement
 * @brief Abstract interface for managing session and exposing them via a list model.
 *
 * The session infos are exposed via a QAbstractListModel that can be consumed by the UI layer.
 *
 * Responsibilities:
 * - Provide an invokable method to start synchronization of the session info model
 * - Provide an invokable method to delete a session in the local storage 
 * - Provide an invokable method to load a session from the local storage
 * - Provide read-only access to the model reflecting the current session infos
 * - Notify callers about the synchronization, load and delete result via a signal, including error context
 *
 * Threading:
 * - Unless documented otherwise by the implementation, methods and signals are expected to be used on the Qt main thread.
 */
class ILocalSessionManagement : public QObject
{
    Q_OBJECT

    /**
     * @property QAbstractListModel* model
     * @brief Model exposing the local session information.
     */
    Q_PROPERTY(QAbstractListModel* model READ getSessionInfoListModel CONSTANT)

public:
    Q_DISABLE_COPY_MOVE(ILocalSessionManagement)

    /**
     * @brief Virtual destructor.
     */
    ~ILocalSessionManagement() override = default;

    /**
     * @brief Refreshes the session info list model.
     *
     * Implementations should update the underlying model and emit sessionInfoListRefreshed() when the refresh completes.
     */
    virtual Q_INVOKABLE void refreshSessionInfos() noexcept = 0;

    /**
     * @brief Returns the session info list model.
     * @return A QAbstractListModel containing session info entries.
     * @warning The returned pointer must remain valid while the object exists.
     */
    virtual QAbstractListModel* getSessionInfoListModel() noexcept = 0;

    /**
     * @brief Removes a session identified by the given SessionInfo.
     * @param info The SessionInfo identifying the session to remove.
     * @warning Implementations should emit sessionRemoved(bool) signal upon completion.
     */
    virtual Q_INVOKABLE void remove(RapidAndroid::Common::SessionInfo const& info) noexcept = 0;

    /**
     * @brief Loads a session identified by the given SessionInfo.
     * @details If the loading failed the session parameter is a default constructed session and success is false.
     * @param info The SessionInfo identifying the session to load.
     * @warning Implementations should emit sessionLoaded(bool, SessionInfo, Session) signal upon completion.
     */
    virtual Q_INVOKABLE void load(RapidAndroid::Common::SessionInfo const& info) noexcept = 0;

Q_SIGNALS:
    /**
     * @brief Emitted after the session info list has been refreshed.
     */
    void sessionInfoListRefreshed();

    /**
     * @brief Emitted after a session has been removed.
     * @param session The session that was removed.
     * @param success true if the removal was successful; false otherwise.
     */
    void sessionRemoved(bool success);

    void sessionLoaded(bool success,
                       RapidAndroid::Common::SessionInfo const& info,
                       RapidAndroid::Common::Session const& session);

protected:
    /**
     * @brief Protected default constructor to enforce interface usage.
     */
    ILocalSessionManagement() = default;
};

} // namespace RapidAndroid::Workflow

#endif // RAPIDANDROID_WORKFLOW_ISESSIONMANAGEMENT_HPP
