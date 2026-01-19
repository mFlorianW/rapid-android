// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "RapidLiveSession/LiveSessionJsonDeserializer.hpp"
#include "LoggingCategories.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <RapidSession/SessionJsonDeserializer.hpp>

namespace RapidAndroid::RapidLiveSession
{

QFuture<std::optional<Event>> LiveSessionJsonDeserializer::deserialize(QByteArray const& data)
{
    return QtConcurrent::run([this, data]() -> std::optional<Event> {
        auto jsonError = QJsonParseError{};
        auto jsondoc = QJsonDocument::fromJson(data, &jsonError);
        if (jsondoc.isNull() || !jsondoc.isObject()) {
            qCCritical(rljd).noquote() << "Failed to parse JSON data for LiveSession. Error:"
                                       << jsonError.errorString();
            return std::nullopt;
        }

        if (jsondoc.object().value("event").toString() == QStringLiteral("current_laptime")) {
            return parseLaptimeEvent(jsondoc.object().value("data").toObject());
        } else if (jsondoc.object().value("event").toString() == QStringLiteral("lap_finished")) {
            return parseLapFinishedEvent(jsondoc.object().value("data").toObject());
        } else if (jsondoc.object().value("event").toString() == QStringLiteral("current_session")) {
            return parseCurrentSessionEvent(jsondoc.object().value("data").toObject());
        }

        qCCritical(rljd) << "Unknown LiveSession event type in JSON data";
        return std::nullopt;
    });
}

std::optional<Event> LiveSessionJsonDeserializer::parseLaptimeEvent(QJsonObject const& dataObj)
{
    auto timeStr = dataObj.value("time").toString();
    QTime laptime = QTime::fromString(timeStr, "hh:mm:ss.zzz");
    auto timeOpt = parseTime(timeStr);
    if (timeOpt.has_value()) {
        Common::LaptimeEvent laptimeEvent{laptime};
        return Event{laptimeEvent};
    }
    qCCritical(rljd) << "Invalid laptime format in LiveSession event:" << timeStr;
    return std::nullopt;
}

std::optional<Event> LiveSessionJsonDeserializer::parseLapFinishedEvent(QJsonObject const& dataObj)
{
    auto timeStr = dataObj.value("time").toString();
    QTime laptime = QTime::fromString(timeStr, "hh:mm:ss.zzz");
    auto timeOpt = parseTime(timeStr);
    if (timeOpt.has_value()) {
        Common::LapFinishedEvent lapFinishedEvent{laptime};
        return Event{lapFinishedEvent};
    }
    qCCritical(rljd) << "Invalid laptime format in LiveSession lap_finished event:" << timeStr;
    return std::nullopt;
}

std::optional<QTime> LiveSessionJsonDeserializer::parseTime(QString const& timeStr)
{
    QTime time = QTime::fromString(timeStr, "hh:mm:ss.zzz");
    if (!time.isValid()) {
        qCCritical(rljd) << "Invalid time format:" << timeStr;
        return std::nullopt;
    }
    return time;
}

std::optional<Event> LiveSessionJsonDeserializer::parseCurrentSessionEvent(QJsonObject const& dataObj)
{
    auto session = Session::SessionJsonDeserializer::deserialize(dataObj.value("session").toObject());
    session.waitForFinished();
    auto sessionOpt = session.takeResult();
    if (sessionOpt.has_value()) {
        return Common::CurrentSessionEvent{std::move(sessionOpt.value())};
    }
    qCCritical(rljd) << "Failed to parse current_session data in LiveSession";
    return std::nullopt;
}

} // namespace RapidAndroid::RapidLiveSession
