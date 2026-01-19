// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "LoggingCategories.hpp"
#include <Common/Lap.hpp>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtConcurrentRun>
#include <RapidSession/SessionJsonDeserializer.hpp>
#include <memory>

namespace RapidAndroid::Session
{

namespace
{

QDate deserializeDate(QString const& dateStr)
{
    return QDate::fromString(dateStr, "dd.MM.yyyy");
}

QTime deserializeTime(QString const& timeStr)
{
    return QTime::fromString(timeStr, "hh:mm:ss.zzz");
}

Common::Position deserializePosition(QJsonObject const& positionObject)
{
    auto position = Common::Position{};
    position.latitude = positionObject.value("latitude").toDouble();
    position.longitude = positionObject.value("longitude").toDouble();
    return position;
}

QVector<Common::Lap> deserializeLaps(QJsonArray const& lapsArray)
{
    QVector<Common::Lap> laps;
    for (auto const& lapValue : lapsArray) {
        laps.append(Common::Lap{});
        auto const lapObject = lapValue.toObject();
        auto const sectors = lapObject.value("sectors").toArray();
        for (auto const& sectorValue : sectors) {
            auto const sectorTimeStr = sectorValue.toString();
            auto const sectorTime = deserializeTime(sectorTimeStr);
            laps.back().sectors.append(sectorTime);
        }
        auto const logPoints = lapObject.value("log_points").toArray();
        for (auto const& logPointValue : logPoints) {
            auto const logPointObject = logPointValue.toObject();
            auto logPoint = Common::GnssPosition{};
            logPoint.latitude = logPointObject.value("latitude").toDouble();
            logPoint.longitude = logPointObject.value("longitude").toDouble();
            logPoint.velocity = logPointObject.value("velocity").toDouble();
            logPoint.time = deserializeTime(logPointObject.value("time").toString());
            logPoint.date = deserializeDate(logPointObject.value("date").toString());
            laps.back().logPoints.append(logPoint);
        }
    }
    return laps;
}

Common::Track deserializeTrack(QJsonObject const& trackObject)
{
    auto track = Common::Track{};
    track.name = trackObject.value("name").toString();
    track.startline = deserializePosition(trackObject.value("startline").toObject());
    if (trackObject.contains("finishline")) {
        track.finishline = deserializePosition(trackObject.value("finishline").toObject());
    }
    auto sectorsArray = trackObject.value("sectors").toArray();
    for (auto const& sectorValue : std::as_const(sectorsArray)) {
        auto const sectorObject = sectorValue.toObject();
        track.sectors.append(deserializePosition(sectorObject));
    }
    return track;
}

std::optional<std::unique_ptr<Common::Session>> deserializeSession(QByteArray data)
{
    auto error = QJsonParseError{};
    auto const jsonDoc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        qCCritical(sJDeserializer) << "Failed to deserialize session JSON data:" << error.errorString();
        return std::nullopt;
    }
    auto session = std::make_unique<Common::Session>();
    auto lapsArray = jsonDoc.object().value("laps").toArray();
    if (not lapsArray.isEmpty()) {
        session->setLaps(deserializeLaps(lapsArray));
    }
    auto trackObject = jsonDoc.object().value("track").toObject();
    if (not trackObject.isEmpty()) {
        session->setTrack(deserializeTrack(trackObject));
    }
    session->setDate(deserializeDate(jsonDoc.object().value("date").toString()));
    session->setTime(QTime::fromString(jsonDoc.object().value("time").toString(), Qt::ISODate));

    return session;
}

} // namespace

QFuture<std::optional<std::unique_ptr<Common::Session>>> SessionJsonDeserializer::deserialize(QJsonObject const& data)
{
    return QtConcurrent::run(
        [](QJsonObject const& obj) -> std::optional<std::unique_ptr<Common::Session>> {
            auto session = std::make_unique<Common::Session>();
            auto lapsArray = obj.value("laps").toArray();
            if (not lapsArray.isEmpty()) {
                session->setLaps(deserializeLaps(lapsArray));
            }
            auto trackObject = obj.value("track").toObject();
            if (not trackObject.isEmpty()) {
                session->setTrack(deserializeTrack(trackObject));
            }
            session->setDate(deserializeDate(obj.value("date").toString()));
            session->setTime(QTime::fromString(obj.value("time").toString(), Qt::ISODate));

            return session;
        },
        data);
}

QFuture<std::optional<std::unique_ptr<Common::Session>>> SessionJsonDeserializer::deserialize(QByteArray data)
{
    return QtConcurrent::run(
        [](QByteArray raw) -> std::optional<std::unique_ptr<Common::Session>> {
            return deserializeSession(std::move(raw));
        },
        std::move(data));
}

QFuture<std::optional<std::unique_ptr<Common::SessionInfo>>> SessionJsonDeserializer::deserializeInfo(QByteArray data)
{
    return QtConcurrent::run(
        [](QByteArray raw) -> std::optional<std::unique_ptr<Common::SessionInfo>> {
            auto error = QJsonParseError{};
            auto const jsonDoc = QJsonDocument::fromJson(raw, &error);
            if (error.error != QJsonParseError::NoError) {
                qCCritical(sJDeserializer) << "Failed to deserialize session info JSON data:" << error.errorString();
                return std::nullopt;
            }
            auto const obj = jsonDoc.object();
            auto sessionInfo = std::make_unique<Common::SessionInfo>();
            sessionInfo->id = obj.value("id").toString();
            sessionInfo->date = QDateTime::fromString(obj.value("date").toString(), Qt::ISODate);
            sessionInfo->trackName = obj.value("track_name").toString();
            sessionInfo->laps = static_cast<quint32>(obj.value("laps").toInt());
            return sessionInfo;
        },
        std::move(data));
}

} // namespace RapidAndroid::Session
