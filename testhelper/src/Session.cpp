// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "TestHelper/Session.hpp"

namespace RapidAndroid::TestHelper
{

QJsonDocument getJsonOscherslebenSession() noexcept
{
    // clang-format off
    static auto rsp = QByteArray{
    "{"
        "\"id\": \"sess-123\","
        "\"date\": \"01.01.1970\","
        "\"time\": \"13:00:00.000\","
        "\"track\": {"
            "\"name\": \"Oschersleben\","
            "\"startline\": {"
                "\"latitude\": 52.025833,"
                "\"longitude\": 11.279166"
            "},"
            "\"finishline\": {"
                "\"latitude\": 52.025833,"
                "\"longitude\": 11.279166"
            "},"
            "\"sectors\": ["
                "{"
                    "\"latitude\": 52.025833,"
                    "\"longitude\": 11.279166"
                "},"
                "{"
                    "\"latitude\": 52.025833,"
                    "\"longitude\": 11.279166"
                "}"
            "]"
        "},"
        "\"laps\": ["
            "{"
                "\"sectors\": ["
                    "\"00:00:25.144\","
                    "\"00:00:25.144\","
                    "\"00:00:25.144\","
                    "\"00:00:25.144\","
                    "\"00:00:25.144\""
                "],"
                "\"log_points\": ["
                    "{"
                        "\"velocity\": 100.0,"
                        "\"longitude\": 11.0,"
                        "\"latitude\": 52.0,"
                        "\"time\": \"00:00:00.000\","
                        "\"date\": \"01.01.1970\""
                    "},"
                    "{"
                        "\"velocity\": 100.0,"
                        "\"longitude\": 11.0,"
                        "\"latitude\": 52.0,"
                        "\"time\": \"00:00:00.000\","
                        "\"date\": \"01.01.1970\""
                    "}"
                "]"
            "}"
        "]"
    "}"};
    // clang-format on

    static auto error = QJsonParseError{};
    static auto jsonDocument = QJsonDocument::fromJson(QByteArray{rsp}, &error);
    if (error.error != QJsonParseError::NoError) {
        auto const errorMsg = QString{"Failed to parse hardcoded JSON response: %1"}.arg(error.errorString());
        qFatal("%s", errorMsg.toUtf8().constData());
    }
    return jsonDocument;
}

QJsonDocument getJsonOscherslebenSessionInfo() noexcept
{
    // clang-format off
    static auto rsp = QByteArray{
        "{"
            "\"id\":\"oschersleben_01_01_1970_13_00_00_000\","
            "\"date\":\"1970-01-01T13:00:00.000\","
            "\"track_name\": \"Oschersleben\","
            "\"laps\":1"
        "}"
    };
    // clang-format on
    static auto error = QJsonParseError{};
    static auto jsonDocument = QJsonDocument::fromJson(QByteArray{rsp}, &error);
    if (error.error != QJsonParseError::NoError) {
        auto const errorMsg = QString{"Failed to parse hardcoded JSON response: %1"}.arg(error.errorString());
        qFatal("%s", errorMsg.toUtf8().constData());
    }
    return jsonDocument;
}

Common::Session getOscherslebenSession() noexcept
{
    // clang-format off
    static auto session = Common::Session{};
    session.setDate(QDate{1970, 1, 1});
    session.setTime(QTime{13, 0, 0, 0});
    session.setTrack(Common::Track
    {
        .name = "Oschersleben",
        .startline = Common::Position{.latitude = 52.025833, .longitude = 11.279166},
        .finishline = Common::Position{.latitude = 52.025833, .longitude = 11.279166},
        .sectors = 
        {
        Common::Position{.latitude = 52.025833, .longitude = 11.279166},
        Common::Position{.latitude = 52.025833, .longitude = 11.279166}
        }
    });
    session.setLaps(
    {
        Common::Lap
        {
            .sectors =
                {
                QTime{0, 0, 25, 144},
                QTime{0, 0, 25, 144},
                QTime{0, 0, 25, 144},
                QTime{0, 0, 25, 144},
                QTime{0, 0, 25, 144}
                },
                .logPoints =
                {
                Common::GnssPosition
                    {
                        .latitude = 52.0,
                        .longitude = 11.0,
                        .velocity = 100.0,
                        .time = QTime{0, 0, 0, 0},
                        .date = QDate{1970, 1, 1}
                    },
                Common::GnssPosition
                    {
                        .latitude = 52.0,
                        .longitude = 11.0,
                        .velocity = 100.0,
                        .time = QTime{0, 0, 0, 0},
                        .date = QDate{1970, 1, 1}
                    }
                }
        }
    });
    // clang-format on
    return session;
}

Common::SessionInfo getOscherslebenSessionInfo() noexcept
{
    return Common::SessionInfo{.id = "oschersleben_01_01_1970_13_00_00_000",
                               .date = QDateTime{QDate{1970, 1, 1}, QTime{13, 0, 0, 0}},
                               .trackName = "Oschersleben",
                               .laps = 1};
}

} // namespace RapidAndroid::TestHelper
