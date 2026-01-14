// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "RapidLiveSession/LiveSessionJsonDeserializer.hpp"
#include "LoggingCategories.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>

namespace RapidAndroid::RapidLiveSession
{

QFuture<std::optional<Event>> LiveSessionJsonDeserializer::deserialize(QByteArray const& data)
{
    return QtConcurrent::run([this, data]() -> std::optional<Event> {
        auto jsonError = QJsonParseError{};
        auto jsondoc = QJsonDocument::fromJson(data, &jsonError);
        if (jsondoc.isNull() || !jsondoc.isObject()) {
            qCCritical(rljd) << "Failed to parse JSON data for LiveSession. Error:" << jsonError.errorString()
                             << "Event:" << data;
            return std::nullopt;
        }

        if (jsondoc.object().value("event").toString() == QStringLiteral("current_laptime")) {
            return parseLaptimeEvent(jsondoc.object().value("data").toObject());
        }

        qCCritical(rljd) << "Unknown LiveSession event type in JSON data:" << data;
        return std::nullopt;
    });
}

std::optional<Event> LiveSessionJsonDeserializer::parseLaptimeEvent(QJsonObject const& dataObj)
{
    auto timeStr = dataObj.value("time").toString();
    QTime laptime = QTime::fromString(timeStr, "hh:mm:ss.zzz");
    if (!laptime.isValid()) {
        qCCritical(rljd) << "Invalid laptime format in LiveSession event:" << timeStr;
        return std::nullopt;
    }
    Common::LaptimeEvent laptimeEvent{laptime};
    return Event{laptimeEvent};
}

} // namespace RapidAndroid::RapidLiveSession
