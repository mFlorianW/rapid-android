// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <Common/Session.hpp>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QString>
#include <QTimeZone>
#include <QtConcurrentRun>
#include <Workflow/ISessionSerializer.hpp>
#include <Workflow/ISessionStorage.hpp>
#include <filesystem>

namespace RapidAndroid::Session
{

QLoggingCategory const& fsLogCat();

template <typename SerializerType>
    requires Workflow::SessionSerializerConcept<SerializerType>
class FilesystemStorage
{

public:
    /**
     * @brief Construct storage with a root directory and a serializer.
     *
     * @param path Filesystem path used as the storage root directory.
     * @param serializer Non-owning pointer to the session serializer.
     */
    FilesystemStorage(std::filesystem::path path, SerializerType* serializer) noexcept
        : mStoragePath{std::move(path)}
        , mSerializer{serializer}
    {
    }

    /**
     * @brief Default destructor.
     */
    ~FilesystemStorage() = default;

    /**
     * @brief Copy constructor.
     */
    FilesystemStorage(FilesystemStorage const&) = default;

    /**
     * @brief Copy assignment operator.
     */
    FilesystemStorage& operator=(FilesystemStorage const&) = default;

    /**
     * @brief Move constructor.
     */
    FilesystemStorage(FilesystemStorage&&) noexcept = default;

    /**
     * @brief Move assignment operator.
     */
    FilesystemStorage& operator=(FilesystemStorage&&) noexcept = default;

    /**
     * @brief Enumerate stored session metadata.
     * @return A list of available session infos in the storage directory.
     */
    QVector<Common::SessionInfo> getSessionInfos() const noexcept
    {
        return {};
    }

    /**
     * @brief Load a session from storage.
     *
     * @param sessionInfo Metadata identifying the session to load.
     * @return The loaded session if successful; std::nullopt otherwise.
     */
    std::optional<Common::Session> load(Common::SessionInfo const& sessionInfo) noexcept
    {
        return std::nullopt;
    }

    /**
     * @brief Store a session asynchronously to the filesystem.
     *
     * @param session The session to store.
     * @return A QFuture that will hold the result of the storage operation.
     */
    QFuture<Workflow::StoreResult> store(std::unique_ptr<Common::Session> session) noexcept
    {
        return QtConcurrent::run([this, s = std::move(session)]() mutable -> Workflow::StoreResult {
            return this->storeImpl(std::move(s));
        });
    }

    /**
     * @brief Remove a stored session from the filesystem.
     *
     * @param session The session to remove.
     * @return true on success; false on error.
     */
    bool remove(Common::Session const& session) noexcept
    {
        return false;
    }

private:
    Workflow::StoreResult storeImpl(std::unique_ptr<Common::Session> session) noexcept
    {
        bool result = true;
        auto sessionId = QString{"%1_%2_%3"}.arg(session->getTrack().name.toLower(),
                                                 session->getDate().toString("dd_MM_yyyy"),
                                                 session->getTime().toString("HH_mm_ss_zzz"));
        auto const sessionFileName = QString{"%1.session"}.arg(sessionId);
        auto const sessionInfoFileName = QString{"%1.info"}.arg(sessionId);
        auto const infoFilePath = QString::fromStdString(mStoragePath / sessionInfoFileName.toUtf8().constData());
        auto infoFile = QFile{infoFilePath};
        if (infoFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            auto jsonObj = QJsonObject{};
            jsonObj.insert("id", sessionId);
            auto const date = QDateTime{session->getDate(), session->getTime(), QTimeZone::UTC};
            jsonObj.insert("date", date.toString("yyyy-MM-ddTHH:mm:ss.zzz"));
            jsonObj.insert("track_name", session->getTrack().name);
            jsonObj.insert("laps", static_cast<qint32>(session->getLaps().size()));
            auto jsonDoc = QJsonDocument{};
            jsonDoc.setObject(jsonObj);
            infoFile.write(jsonDoc.toJson());
        } else {
            qCCritical(fsLogCat()) << "Failed to open file for writing session info:" << infoFilePath;
            result = false;
        }

        if (result) {
            qCDebug(fsLogCat()) << "Stored session info to" << infoFilePath;
            auto const filePath = QString::fromStdString(mStoragePath / sessionFileName.toUtf8().constData());
            auto file = QFile{filePath};
            auto serializeTask = mSerializer->serialize(std::move(session));
            serializeTask.waitForFinished();
            auto serializeResult = serializeTask.takeResult();
            auto const serializedData = serializeResult.data;
            session = std::move(serializeResult.session);

            if (serializedData.has_value() && file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                auto data = serializedData.value();
                auto const writtenBytes = file.write(data);
                if (writtenBytes < data.size()) {
                    qCCritical(fsLogCat()) << "Failed to write complete session data to file:" << filePath;
                    result = false;
                }
            } else {
                qCCritical(fsLogCat()) << "Failed to open file for writing session:" << filePath;
                result = false;
            }
            qCDebug(fsLogCat()) << "Stored session to" << filePath;
        }
        return {.session = std::move(session), .success = result};
    }

private:
    std::filesystem::path mStoragePath;
    SerializerType* mSerializer{nullptr};
};

} // namespace RapidAndroid::Session
