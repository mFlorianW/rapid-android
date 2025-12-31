// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_WORKFLOW_HTTPDEVICESESSIONMANAGEMENT_HPP
#define RAPIDANDROID_WORKFLOW_HTTPDEVICESESSIONMANAGEMENT_HPP

#include <Common/SessionInfo.hpp>
#include <QDebug>
#include <QFuture>
#include <QFutureWatcher>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QtConcurrentRun>
#include <Workflow/IDeviceSessionManagement.hpp>
#include <Workflow/ISessionDeserializer.hpp>
#include <Workflow/ISessionStorage.hpp>
#include <Workflow/Private/SessionListModel.hpp>

namespace RapidAndroid::Workflow
{

/**
 * @brief Returns the logging category for RestDeviceSessionManagement.
 *
 * @return QLoggingCategory reference for RestDeviceSessionManagement logging.
 */
QLoggingCategory const& dsmLog();

/**
 * @class RapidAndroid::Workflow::RestDeviceSessionManagement
 * @brief Implementation of IDeviceSessionManagement for devices supporting HTTP APIs for downloading session data.
 *
 * This class communicates with the device using HTTP requests to synchronize session information, download sessions, and delete sessions. 
 * It exposes the session information via a QAbstractListModel.
 */
template <typename DeserializerType, typename StorageType>
    requires SessionDeserializerConcept<DeserializerType> and SessionStorageConcept<StorageType>
class HttpDeviceSessionManagement : public IDeviceSessionManagement
{
private:
    using DeserializerFutureWatcher = QFutureWatcher<std::optional<std::unique_ptr<Common::Session>>>;
    using StoreFutureWatcher = QFutureWatcher<StoreResult>;

public:
    Q_DISABLE_COPY_MOVE(HttpDeviceSessionManagement)

    /**
     * @brief default Constructor.
     */
    HttpDeviceSessionManagement(DeserializerType* deserializer, StorageType* storage) noexcept
        : mSessionDeserializer{std::move(deserializer)}
        , mSessionStorage{std::move(storage)}
    {
    }

    /**
     * @brief default Destructor.
     */
    ~HttpDeviceSessionManagement() override = default;

    /**
     * @copydoc IDeviceSessionManagement::getDeviceSessionInfoListModel
     */
    [[nodiscard]] Q_INVOKABLE QAbstractListModel* getDeviceSessionInfoListModel() override
    {
        return std::addressof(mSessionInfoListModel);
    }

    /**
     * @copydoc IDeviceSessionManagement::synchronizeSessionInfos
     */
    Q_INVOKABLE void synchronizeSessionInfos(RapidAndroid::Common::DeviceSettings const& settings) noexcept override
    {
        auto request = QNetworkRequest{
            QUrl{QString{"http://%1:%2/v1/sessions"}.arg(settings.getIpAddress(), QString::number(settings.port))}};
        auto* reply = mNetworkAccessManager.get(request);
        connect(reply, &QNetworkReply::finished, this, [this, reply, settings]() {
            auto const error = reply->error() == QNetworkReply::NoError ? IDeviceSessionManagement::Result::Success
                                                                        : IDeviceSessionManagement::Result::Failure;
            auto errMsg = QString{};
            if (error == IDeviceSessionManagement::Result::Success) {
                auto const jsonDoc = QJsonDocument::fromJson(reply->readAll());
                auto sessionCount = jsonDoc.object().value("total").toVariant().value<quint32>();
                for (quint32 i = 0; i < sessionCount; ++i) {
                    auto sessionObj = jsonDoc.object().value("sessions").toArray().at(i).toObject();
                    auto sessionInfo = Common::SessionInfo{};
                    sessionInfo.id = sessionObj.value("id").toString();
                    sessionInfo.date = QDateTime::fromString(sessionObj.value("date").toString(), Qt::ISODate);
                    sessionInfo.trackName = sessionObj.value("track_name").toString();
                    sessionInfo.laps = static_cast<quint32>(sessionObj.value("laps").toInt());
                    if (not mSessionInfoListModel.contains(sessionInfo)) {
                        mSessionInfoListModel.insertItem(sessionInfo);
                    }
                }

                // mD->sessionInfoListModel.updateFromJson(responseData);
                qInfo(dsmLog()) << "Successfully synchronized session infos from " << settings;
            } else {
                errMsg = reply->errorString();
                qCCritical(dsmLog()) << "Failed to synchronize session infos:" << reply->errorString();
            }
            Q_EMIT sessionInfosSynchronized(error, errMsg);
            reply->deleteLater();
        });
        qCInfo(dsmLog()) << "Initiated synchronization of session infos from" << settings;
    }

    /**
     * @copydoc IDeviceSessionManagement::downloadSession
     */
    Q_INVOKABLE void downloadSession(RapidAndroid::Common::DeviceSettings const& settings,
                                     QString const& sessionId) noexcept override
    {
        auto request = QNetworkRequest{QUrl{QString{"http://%1:%2/v1/sessions/%3"}.arg(settings.getIpAddress(),
                                                                                       QString::number(settings.port),
                                                                                       sessionId)}};
        auto* reply = mNetworkAccessManager.get(request);
        connect(reply, &QNetworkReply::finished, this, [this, reply, sessionId]() {
            auto error = reply->error() == QNetworkReply::NoError ? IDeviceSessionManagement::Result::Success
                                                                  : IDeviceSessionManagement::Result::Failure;
            if (error == IDeviceSessionManagement::Result::Success) {
                auto sessionData = reply->readAll();
                onSessionDownloadFinished(sessionId, std::move(sessionData));
            } else {
                qCCritical(dsmLog()) << "Failed to download session" << sessionId << ":" << reply->errorString();
                Q_EMIT sessionDownloaded(sessionId, error);
            }
            reply->deleteLater();
        });
        qCInfo(dsmLog()) << QString{"Initiated download of session %1 from %2"}.arg(sessionId, settings.name);
    }

    /**
     * @copydoc IDeviceSessionManagement::deleteSession
     */
    Q_INVOKABLE void deleteSession(RapidAndroid::Common::DeviceSettings const& settings,
                                   QString const& sessionId) noexcept override
    {
        auto request = QNetworkRequest{QUrl{QString{"http://%1:%2/v1/sessions/%3"}.arg(settings.getIpAddress(),
                                                                                       QString::number(settings.port),
                                                                                       sessionId)}};
        auto* reply = mNetworkAccessManager.deleteResource(request);
        connect(reply, &QNetworkReply::finished, this, [this, reply, sessionId]() {
            auto const error = reply->error() == QNetworkReply::NoError ? IDeviceSessionManagement::Result::Success
                                                                        : IDeviceSessionManagement::Result::Failure;
            auto errMsg = QString{};
            if (error == IDeviceSessionManagement::Result::Success) {
                auto sessionRow = mSessionInfoListModel.getRows([&sessionId](Common::SessionInfo const& info) {
                    return info.id == sessionId;
                });
                if (not sessionRow.isEmpty()) {
                    if (not mSessionInfoListModel.removeItem(sessionRow.at(0))) {
                        qCCritical(dsmLog())
                            << "Failed to remove session" << sessionId << "from local model after deletion.";
                    }
                }
                qInfo(dsmLog()) << "Successfully deleted session" << sessionId;
            } else {
                errMsg = reply->errorString();
                qCCritical(dsmLog()) << "Failed to delete session" << sessionId << ":" << reply->errorString();
            }
            Q_EMIT sessionDeleted(sessionId, error);
            reply->deleteLater();
        });
        qCInfo(dsmLog()) << QString{"Initiated deletion of session %1 from %2"}.arg(sessionId, settings.name);
    }

private:
    void onSessionDownloadFinished(QString const& sessionId, QByteArray sessionData)
    {
        auto sessionFuture = mSessionDeserializer->deserialize(sessionData);
        mDeserializerFutureWatcher = std::make_unique<DeserializerFutureWatcher>();
        mDeserializerFutureWatcher->setFuture(sessionFuture);
        connect(mDeserializerFutureWatcher.get(), &DeserializerFutureWatcher::finished, this, [this, sessionId]() {
            onSessionDeserializationFinished(sessionId);
        });
    }

    void onSessionDeserializationFinished(QString const& sessionId)
    {
        auto sessionOpt = mDeserializerFutureWatcher->future().takeResult();
        if (sessionOpt.has_value()) {
            auto session = std::move(*sessionOpt);
            mStoreFutureWatcher = std::make_unique<StoreFutureWatcher>();
            mStoreFutureWatcher->setFuture(mSessionStorage->store(std::move(session)));
            connect(mStoreFutureWatcher.get(), &StoreFutureWatcher::finished, this, [this, sessionId]() {
                onSessionStoreFinished(sessionId);
            });
        } else {
            qCCritical(dsmLog()) << "Failed to deserialize session" << sessionId;
            Q_EMIT sessionDownloaded(sessionId, IDeviceSessionManagement::Result::Failure);
        }
    }

    void onSessionStoreFinished(QString const& sessionId)
    {
        auto storeResult = mStoreFutureWatcher->future().takeResult();
        auto result =
            storeResult.success ? IDeviceSessionManagement::Result::Success : IDeviceSessionManagement::Result::Failure;
        qCDebug(dsmLog()) << "Session" << sessionId
                          << "stored with result:" << (storeResult.success ? "Success" : "Failure");
        Q_EMIT sessionDownloaded(sessionId, result);
    }

private:
    RapidAndroid::Workflow::SessionListModel mSessionInfoListModel;
    QNetworkAccessManager mNetworkAccessManager;
    DeserializerType* mSessionDeserializer{nullptr};
    StorageType* mSessionStorage{nullptr};
    std::unique_ptr<DeserializerFutureWatcher> mDeserializerFutureWatcher;
    std::unique_ptr<StoreFutureWatcher> mStoreFutureWatcher;
};

} // namespace RapidAndroid::Workflow

#endif // RAPIDANDROID_WORKFLOW_HTTPDEVICESESSIONMANAGEMENT_HPP
