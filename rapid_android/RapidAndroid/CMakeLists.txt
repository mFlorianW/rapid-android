# SPDX-FileCopyrightText: 2026 All contributors
#
# SPDX-License-Identifier: GPL-2.0-or-later

qt_add_qml_module(RapidAndroid
    URI "Rapid.Android"
    OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml/Rapid/Android"
    STATIC
    QML_FILES
        qml/RapidAndroid.qml
        # pages
        qml/pages/LaptimerPage.qml
        qml/pages/LivePage.qml
        qml/pages/SessionsPage.qml
        qml/pages/TracksPage.qml
    RESOURCES
        img/Stopwatch.svg
        img/Live.svg
        img/Sessions.svg
        img/Laptimer.svg
        img/RaceTrack.svg
    SOURCES
        GlobalContext.cpp
        GlobalContext.hpp
        LoggingCategories.cpp
        LoggingCategories.hpp
        TimeFormatter.cpp
        TimeFormatter.hpp
    DEPENDENCIES
        RapidAndroid::Workflow
)

add_library(RapidAndroid::Android ALIAS RapidAndroid)
add_library(RapidAndroid::AndroidPlugin ALIAS RapidAndroidplugin)

target_link_libraries(RapidAndroid
    PUBLIC
        Qt6::Quick
        RapidAndroid::Workflow
        RapidAndroid::ControlsPlugin
        RapidAndroid::Laptimer
        RapidAndroid::LaptimerPlugin
        RapidAndroid::Session
        RapidAndroid::SessionPlugin
        RapidAndroid::LiveSession
        RapidAndroid::LiveSessionPlugin
)

target_include_directories(RapidAndroid
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
