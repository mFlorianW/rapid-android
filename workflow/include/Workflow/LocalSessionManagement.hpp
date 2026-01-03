// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_WORKFLOW_LOCALSESSIONMANAGEMENT_HPP
#define RAPIDANDROID_WORKFLOW_LOCALSESSIONMANAGEMENT_HPP

#include <QFutureWatcher>
#include <QLoggingCategory>
#include <Workflow/ILocalSessionManagement.hpp>
#include <Workflow/ISessionDeserializer.hpp>
#include <Workflow/ISessionSerializer.hpp>
#include <Workflow/ISessionStorage.hpp>
#include <Workflow/Private/SessionListModel.hpp>
#include <memory>

namespace RapidAndroid::Workflow
{
/**
 * @brief Returns the logging category for LocalSessionManagement.
 *
 * @return QLoggingCategory reference for LocalSessionManagement logging.
 */
QLoggingCategory const& lsmLog();

/**
 * @class RapidAndroid::Workflow::LocalSessionManagement
 * @brief Implementation of ILocalSessionManagement for managing sessions stored locally.
 *
 * This class interacts with a local session storage to retrieve, remove, and load session data.
 * It exposes the session information via a QAbstractListModel.
 */
template <typename StorageType>
    requires SessionStorageConcept<StorageType>
class LocalSessionManagement : public ILocalSessionManagement
{
public:
    Q_DISABLE_COPY_MOVE(LocalSessionManagement)

    /**
     * @brief Constructs LocalSessionManagement with the provided storage backend.
     * @param storage Non-owning pointer to a storage implementation that satisfies SessionStorageConcept.
     * @note The @p storage pointer must remain valid for the lifetime of this object.
     */
    LocalSessionManagement(StorageType* storage)
        : mStorage{storage}
    {
    }

    /**
     * @brief Destructor.
     */
    ~LocalSessionManagement() override = default;

    /**
     * @brief Asynchronously refreshes the session info list from storage.
     *
     * Starts a background task to fetch session infos and updates the internal model
     * when finished. If a refresh is already in progress, the call is ignored.
     *
     * @note Emits sessionInfoListRefreshed() after the task completes.
     */
    void refreshSessionInfos() noexcept override
    {
        if (mRefreshTask && mRefreshTask->isRunning()) {
            return;
        }
        auto future = mStorage->getSessionInfos();
        mRefreshTask = std::make_unique<QFutureWatcher<QVector<Common::SessionInfo>>>();
        connect(mRefreshTask.get(), &QFutureWatcher<QVector<Common::SessionInfo>>::finished, this, [this]() {
            onRefreshTaskFinished();
        });
        mRefreshTask->setFuture(future);
    }

    /**
     * @brief Asynchronously removes a session from storage and updates the model.
     * @param info The session info identifying the session to remove.
     *
     * If a remove task is already running, emits sessionRemoved(false) and returns.
     * On completion, the model is updated and a signal indicating success is emitted.
     *
     * @note Emits sessionRemoved(success).
     */
    QAbstractListModel* getSessionInfoListModel() noexcept override
    {
        return std::addressof(mSessionListModel);
    }

    /**
     * @brief Asynchronously removes a session from storage and updates the model.
     * @param info The session info identifying the session to remove.
     *
     * If a remove task is already running, emits sessionRemoved(false) and returns.
     * On completion, the model is updated and a signal indicating success is emitted.
     *
     * @note Emits sessionRemoved(success).
     */
    Q_INVOKABLE void remove(RapidAndroid::Common::SessionInfo const& info) noexcept override
    {
        if (mRemoveTask && mRemoveTask->isRunning()) {
            Q_EMIT sessionRemoved(false);
            return;
        }
        auto future = mStorage->remove(info);
        mRemoveTask = std::make_unique<QFutureWatcher<bool>>();
        connect(mRemoveTask.get(), &QFutureWatcher<bool>::finished, this, [this, info]() {
            onRemoveTaskFinished(info);
        });
        mRemoveTask->setFuture(future);
    }

    /**
     * @brief Asynchronously loads a session's full data from storage.
     * @param info The session info identifying the session to load.
     *
     * If a load task is already running, emits sessionLoaded(false, info, std::nullopt) and returns.
     * On completion, emits the result with the loaded session if available.
     *
     * @note Emits sessionLoaded(success, info, result).
     */
    Q_INVOKABLE void load(RapidAndroid::Common::SessionInfo const& info) noexcept override
    {
        if (mLoadTask && mLoadTask->isRunning()) {
            Q_EMIT sessionLoaded(false, info, Common::Session{});
            return;
        }
        auto future = mStorage->load(info);
        mLoadTask = std::make_unique<QFutureWatcher<std::optional<Common::Session>>>();
        connect(mLoadTask.get(), &QFutureWatcher<std::optional<Common::Session>>::finished, this, [this, info]() {
            onLoadTaskFinished(info);
        });
        mLoadTask->setFuture(future);
    }

private Q_SLOTS:
    /**
     * @brief Handles completion of the refresh task.
     *
     * Updates or inserts session infos into the model, logs actions,
     * resets the task, and emits sessionInfoListRefreshed().
     */
    void onRefreshTaskFinished()
    {
        auto result = mRefreshTask->result();
        for (auto const& info : std::as_const(result)) {
            auto row = mSessionListModel.getRows([info](RapidAndroid::Common::SessionInfo const& item) {
                return item.id == info.id;
            });
            if (row.empty()) {
                mSessionListModel.insertItem(info);
                qCDebug(lsmLog) << "Loaded session info:" << info.id;
            } else {
                Q_UNUSED(mSessionListModel.updateItem(row.at(0), info));
                qCDebug(lsmLog) << "Session info already exists, updating:" << info.id;
            }
        }
        mRefreshTask.reset();
        Q_EMIT sessionInfoListRefreshed();
    }

    /**
     * @brief Handles completion of the remove task.
     * @param info The session info that was requested to be removed.
     *
     * Removes the item from the model on success, logs the outcome,
     * resets the task, and emits sessionRemoved(result).
     */
    void onRemoveTaskFinished(RapidAndroid::Common::SessionInfo const& info)
    {
        auto result = mRemoveTask->result();
        if (result) {
            qCDebug(lsmLog) << "Succuess removed session:" << info.id;
            Q_UNUSED(mSessionListModel.removeItem(info));
        }
        mRemoveTask.reset();
        Q_EMIT sessionRemoved(result);
    }

    /**
     * @brief Handles completion of the load task.
     * @param info The session info that was requested to be loaded.
     *
     * Logs success/failure, resets the task, and emits sessionLoaded(success, info, result).
     */
    void onLoadTaskFinished(RapidAndroid::Common::SessionInfo const& info)
    {
        auto result = mLoadTask->result();
        bool success = result.has_value();
        mLoadTask.reset();
        qCDebug(lsmLog) << "Load session" << info.id << (success ? "succeeded." : "failed.");
        Q_EMIT sessionLoaded(success, info, *result);
    }

private:
    SessionListModel mSessionListModel;
    StorageType* mStorage;
    std::unique_ptr<QFutureWatcher<QVector<Common::SessionInfo>>> mRefreshTask{nullptr};
    std::unique_ptr<QFutureWatcher<bool>> mRemoveTask{nullptr};
    std::unique_ptr<QFutureWatcher<std::optional<Common::Session>>> mLoadTask{nullptr};
};

} // namespace RapidAndroid::Workflow

#endif // RAPIDANDROID_WORKFLOW_LOCALSESSIONMANAGEMENT_HPP
