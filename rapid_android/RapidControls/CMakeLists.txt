# SPDX-FileCopyrightText: 2026 All contributors
#
# SPDX-License-Identifier: GPL-2.0-or-later

qt_add_qml_module(RapidControls
    URI "Rapid.Controls"
    OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml/Rapid/Controls"
    STATIC
    QML_FILES
        qml/NavButton.qml
        qml/ListDelegateBackground.qml
        qml/StatusIndicator.qml
        qml/ContextMenu.qml
        qml/ListItem.qml
        qml/ListDelegate.qml
)

add_library(RapidAndroid::Controls ALIAS RapidControls)
add_library(RapidAndroid::ControlsPlugin ALIAS RapidControlsplugin)

target_link_libraries(RapidControls
    PUBLIC
        Qt6::Quick
)

target_include_directories(RapidControls
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

