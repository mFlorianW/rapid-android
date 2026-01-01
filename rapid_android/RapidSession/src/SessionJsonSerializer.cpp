// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtConcurrentRun>
#include <RapidSession/SessionJsonSerializer.hpp>

namespace RapidAndroid::Session
{

namespace
{

QJsonObject serializeTrack(Common::Track const& track) noexcept
{
    auto obj = QJsonObject{};
    obj.insert("name", track.name);
    obj.insert("startline",
               QJsonObject{{"latitude", track.startline.latitude}, {"longitude", track.startline.longitude}});
    if (track.finishline.has_value()) {
        obj.insert("finishline",
                   QJsonObject{{"latitude", track.finishline->latitude}, {"longitude", track.finishline->longitude}});
    }
    auto sectorsArray = QJsonArray{};
    for (auto const& sector : track.sectors) {
        auto sectorObj = QJsonObject{};
        sectorObj.insert("latitude", sector.latitude);
        sectorObj.insert("longitude", sector.longitude);
        sectorsArray.append(sectorObj);
    }
    obj.insert("sectors", sectorsArray);
    return obj;
}

QJsonObject serializeLap(Common::Lap const& lap) noexcept
{
    auto obj = QJsonObject{};
    auto sectorsArray = QJsonArray{};
    for (auto const& sectorTime : lap.sectors) {
        sectorsArray.append(sectorTime.toString("HH:mm:ss.zzz"));
    }
    obj.insert("sectors", sectorsArray);

    auto logPointsArray = QJsonArray{};
    for (auto const& logPoint : lap.logPoints) {
        auto logPointObj = QJsonObject{};
        logPointObj.insert("latitude", logPoint.latitude);
        logPointObj.insert("longitude", logPoint.longitude);
        logPointObj.insert("velocity", logPoint.velocity);
        logPointObj.insert("time", logPoint.time.toString("HH:mm:ss.zzz"));
        logPointObj.insert("date", logPoint.date.toString("dd.MM.yyyy"));
        logPointsArray.append(logPointObj);
    }
    obj.insert("log_points", logPointsArray);

    return obj;
}

Workflow::SerializeResult serializeSession(std::unique_ptr<Common::Session> session) noexcept
{
    auto jsonDocument = QJsonDocument{};
    auto obj = QJsonObject{};
    obj.insert("id", "sess-123");
    obj.insert("date", session->getDate().toString("dd.MM.yyyy"));
    obj.insert("time", session->getTime().toString("HH:mm:ss.zzz"));

    auto trackObj = serializeTrack(session->getTrack());
    obj.insert("track", trackObj);

    auto lapsArray = QJsonArray{};
    auto const laps = session->getLaps();
    for (auto const& lap : laps) {
        auto lapObj = serializeLap(lap);
        lapsArray.append(lapObj);
    }
    obj.insert("laps", lapsArray);

    jsonDocument.setObject(obj);
    return Workflow::SerializeResult{std::move(session), jsonDocument.toJson()};
}

} // namespace

QFuture<Workflow::SerializeResult> SessionJsonSerializer::serialize(std::unique_ptr<Common::Session> session) noexcept
{
    return QtConcurrent::run([s = std::move(session)]() mutable -> Workflow::SerializeResult {
        return serializeSession(std::move(s));
    });
}

} // namespace RapidAndroid::Session
