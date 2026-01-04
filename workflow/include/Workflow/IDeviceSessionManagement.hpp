// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_WORKFLOW_IDEVICESESSIONMANAGEMENT_HPP
#define RAPIDANDROID_WORKFLOW_IDEVICESESSIONMANAGEMENT_HPP

#include <Common/DeviceSettings.hpp>
#include <QAbstractItemModel>
#include <QObject>

namespace RapidAndroid::Workflow
{
/**
 * @class RapidAndroid::Workflow::IDeviceSessionManagement
 * @brief Abstract interface for synchronizing device session information and exposing them via a list model.
 *
 * The session infos are exposed via a QAbstractListModel that can be consumed by the UI layer.
 *
 * Responsibilities:
 * - Provide an invokable method to start synchronization for a given device configuration
 * - Provide an invokable method to delete a session on the device 
 * - Provide an invokable method to download a session from the device 
 * - Provide read-only access to the model reflecting the current session infos
 * - Notify callers about the synchronization, download and delete result via a signal, including error context
 *
 * Threading:
 * - Unless documented otherwise by the implementation, methods and signals are expected to be used on the Qt main thread.
 */
class IDeviceSessionManagement : public QObject
{
    Q_OBJECT

    /**
     * @property QAbstractListModel* model
     * @brief Model exposing the device session information.
     */
    Q_PROPERTY(QAbstractItemModel* model READ getDeviceSessionInfoListModel CONSTANT)

public:
    Q_DISABLE_COPY_MOVE(IDeviceSessionManagement)

    /**
     * @enum Operation result
     * @brief Outcome of a operation in the device session management.
     */
    enum class Result : quint8
    {
        Failure,
        Success
    };
    Q_ENUM(Result)

    /**
     * @brief Virtual destructor.
     */
    ~IDeviceSessionManagement() override = default;

    /**
     * @brief Start synchronization of device session information for the given device settings.
     * @param settings Device configuration used to reach and query the device.
     * @note This function is QML-invokable.
     * @post On completion, the model returned by getDeviceSessionInfoListModel() reflects the synchronized state,
     *       and sessionInfosSynchronized(status, errorMessage) is emitted.
     * @exceptionsafety No-throw guarantee.
     */
    virtual Q_INVOKABLE void synchronizeSessionInfos(Common::DeviceSettings const& settings) noexcept = 0;

    /**
     * @brief Initiate the download of a session from the device.
     * @param settings Device configuration used to reach and communicate with the device.
     * @param sessionId Identifier of the session to download.
     * @note This function is QML-invokable.
     * @remarks Implementations may emit their own progress/error signals and may update the model upon completion.
     * @exceptionsafety No-throw guarantee.
     */
    virtual Q_INVOKABLE void downloadSession(Common::DeviceSettings const& settings,
                                             QString const& sessionId) noexcept = 0;

    /**
     * @brief Delete a session from the device.
     * @param settings Device configuration used to reach and communicate with the device.
     * @param sessionId Identifier of the session to delete.
     * @note This function is QML-invokable.
     * @post Implementations should ensure the model reflects the deletion after completion.
     * @exceptionsafety No-throw guarantee.
     */
    virtual Q_INVOKABLE void deleteSession(Common::DeviceSettings const& settings,
                                           QString const& sessionId) noexcept = 0;

    /**
     * @brief Access the model exposing device session information.
     * @return Non-owning pointer to a QAbstractItemModel managed by the implementation.
     * @warning The returned pointer must remain valid while the object exists. It may be nullptr until the first synchronization.
     */
    [[nodiscard]] virtual QAbstractItemModel* getDeviceSessionInfoListModel() = 0;

Q_SIGNALS:
    /**
     * @brief Emitted when session information synchronization has completed.
     * @param status Result of the synchronization (Success or Failure).
     * @param errorMessage Error details if status is Failure; empty otherwise.
     */
    void sessionInfosSynchronized(RapidAndroid::Workflow::IDeviceSessionManagement::Result status,
                                  QString const& errorMessage);

    /**
     * @brief Emitted when a session has been downloaded from the device.
     * @param sessionId Identifier of the downloaded session.
     * @param result Result of the operation (Success or Failure).
     * @param errorMessage Error details if result is Failure; empty otherwise.
     */
    void sessionDownloaded(QString const& sessionId,
                           RapidAndroid::Workflow::IDeviceSessionManagement::Result result,
                           QString const& errorMessage = "");

    /**
     * @brief Emitted when a session has been deleted from the device.
     * @param sessionId Identifier of the deleted session.
     * @param result Result of the operation (Success or Failure).
     * @param errorMessage Error details if result is Failure; empty otherwise.
     */
    void sessionDeleted(QString const& sessionId,
                        RapidAndroid::Workflow::IDeviceSessionManagement::Result result,
                        QString const& errorMessage = "");

protected:
    IDeviceSessionManagement() = default;
};

} // namespace RapidAndroid::Workflow

#endif // RAPIDANDROID_WORKFLOW_IDEVICESESSIONMANAGEMENT_HPP
