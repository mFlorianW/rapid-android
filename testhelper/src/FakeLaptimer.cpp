// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "TestHelper/FakeLaptimer.hpp"
#include <QHttpServerResponse>
#include <QJsonObject>
#include <QTcpServer>
#include <QTest>
#include <TestHelper/Session.hpp>
#include <memory>

namespace RapidAndroid::TestHelper
{

namespace
{

QJsonDocument getSessionsResponse() noexcept
{
    // clang-format off
    static auto rsp = QByteArray{"{" \
        "\"total\": 2," \
        "\"sessions\":  [" \
            "{" \
              "\"id\": \"sess-123\"," \
              "\"date\": \"2012-04-23T18:25:43.511Z\"," \
              "\"track_name\": \"Oschersleben\"," \
              "\"laps\": 12" \
            "}," \
            "{" \
              "\"id\": \"sess-456\"," \
              "\"date\": \"2012-04-23T19:25:43.511Z\"," \
              "\"track_name\": \"Assen\"," \
              "\"laps\": 4" \
            "}" \
        "]" \
    "}"};
    // clang-format on

    static QJsonParseError* error = nullptr;
    static auto jsonDocument = QJsonDocument::fromJson(QByteArray{rsp}, error);
    if (error != nullptr && error->error != QJsonParseError::NoError) {
        auto const errorMsg = QString{"Failed to parse hardcoded JSON response: %1"}.arg(error->errorString());
        qFatal("%s", errorMsg.toUtf8().constData());
    }
    return jsonDocument;
}

} // namespace

FakeLaptimer::FakeLaptimer()
{
    mServer.route("/v1/sessions", QHttpServerRequest::Method::Get, []() -> QHttpServerResponse {
        return getSessionsResponse().object();
    });
    mServer.route("/v1/sessions/<arg>", QHttpServerRequest::Method::Delete, [](QString id) -> QHttpServerResponse {
        if (id == "sess-123" or id == "sess-456") {
            return QHttpServerResponse{QHttpServerResponse::StatusCode::Ok};
        }
        return QHttpServerResponse{QHttpServerResponse::StatusCode::NotFound};
    });
    mServer.route("/v1/sessions/<arg>", QHttpServerRequest::Method::Get, [](QString id) -> QHttpServerResponse {
        if (id == "sess-123") {
            return getJsonOscherslebenSession().object();
        }
        return QHttpServerResponse{QHttpServerResponse::StatusCode::NotFound};
    });
    auto* tcpServer = new QTcpServer(); // NOLINT(cppcoreguidelines-owning-memory)
    if (!tcpServer->listen(QHostAddress::LocalHost, 0) or !mServer.bind(tcpServer)) {
        delete tcpServer; // NOLINT(cppcoreguidelines-owning-memory)
        qFatal("Failed to start FakeLaptimer server");
    }
    qInfo() << QString{"FakeLaptimer server started on %1:%2"}
                   .arg(QHostAddress{QHostAddress::LocalHost}.toString())
                   .arg(mServer.serverPorts().at(0));
}

FakeLaptimer::~FakeLaptimer() = default;

Common::DeviceSettings FakeLaptimer::deviceSettings() const
{
    return Common::DeviceSettings{.name = "Fake Laptimer",
                                  .ip = QHostAddress::LocalHost,
                                  .port = mServer.serverPorts().at(0),
                                  .defaultDevice = false};
}

Common::SessionInfo FakeLaptimer::getSessionInfo1() const
{
    return Common::SessionInfo{.id = "sess-123",
                               .date = QDateTime::fromString("2012-04-23T18:25:43.511Z", Qt::ISODate),
                               .trackName = "Oschersleben",
                               .laps = 12};
}

Common::SessionInfo FakeLaptimer::getSessionInfo2() const
{
    return Common::SessionInfo{.id = "sess-456",
                               .date = QDateTime::fromString("2012-04-23T19:25:43.511Z", Qt::ISODate),
                               .trackName = "Assen",
                               .laps = 4};
}

std::unique_ptr<Common::Session> FakeLaptimer::getSession1() const
{
    return std::make_unique<Common::Session>(getOscherslebenSession());
}

} // namespace RapidAndroid::TestHelper
