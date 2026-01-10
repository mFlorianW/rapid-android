# SPDX-FileCopyrightText: 2026 All contributors
#
# SPDX-License-Identifier: GPL-2.0-or-later

qt_add_qml_module(RapidLaptimer
    URI "Rapid.Laptimer"
    OUTPUT_DIRECTORY "Rapid/Laptimer"
    IMPORT_PATH "Rapid/Laptimer"
    STATIC
    QML_FILES
        qml/Laptimer.qml
        # controls
        qml/controls/DelegateContent.qml
        # dialogs
        qml/dialogs/LaptimerConfigurationDialog.qml
    RESOURCES
        img/ClockHourWatch.svg
        img/ConnectionHigh.svg
        img/ConnectionLow.svg
        img/ConnectionMedium.svg
        img/ConnectionOff.svg
        img/ConnectionVeryHigh.svg
        img/ConnectionVeryLow.svg
        img/Gear.svg
        img/Play.svg
        img/Edit.svg
        img/Trash.svg
)

add_library(RapidAndroid::Laptimer ALIAS RapidLaptimer)
add_library(RapidAndroid::LaptimerPlugin ALIAS RapidLaptimerplugin)

target_link_libraries(RapidLaptimer
    PUBLIC
        Qt6::Quick
        RapidAndroid::ControlsPlugin
)

target_include_directories(RapidLaptimer
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)


