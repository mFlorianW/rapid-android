# SPDX-FileCopyrightText: 2026 All contributors
#
# SPDX-License-Identifier: GPL-2.0-or-later

qt_add_qml_module(RapidLiveSession
    URI "Rapid.LiveSession"
    OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml/Rapid/LiveSession"
    STATIC
    QML_FILES
        qml/LiveSession.qml
    SOURCES
        # Publics
        include/RapidLiveSession/LiveSessionWebsocketEventSource.hpp
        include/RapidLiveSession/ILiveSessionEventDeserializer.hpp
        include/RapidLiveSession/LiveSessionJsonDeserializer.hpp
        # Sources
        src/LoggingCategories.cpp
        src/LoggingCategories.hpp
        src/LiveSessionJsonDeserializer.cpp
        src/LiveSessionWebsocketEventSource.cpp
    RESOURCES
        img/Flag.svg
)

add_library(RapidAndroid::LiveSession ALIAS RapidLiveSession)
add_library(RapidAndroid::LiveSessionPlugin ALIAS RapidLiveSessionplugin)

target_link_libraries(RapidLiveSession
    PUBLIC
        Qt6::Quick
        Qt6::WebSockets
        RapidAndroid::ControlsPlugin
        RapidAndroid::Workflow
)

target_include_directories(RapidLiveSession
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include
)

if(ENABLE_TEST)
    add_executable(test_rapid_live_session_websocket_event_source tests/LiveSessionWebsocketEventSourceTest.cpp)
    add_test(NAME test_rapid_live_session_websocket_event_source COMMAND test_rapid_live_session_websocket_event_source)
    target_link_libraries(test_rapid_live_session_websocket_event_source PRIVATE
        Qt6::Test
        RapidAndroid::TestHelper
        RapidAndroid::Common
        RapidAndroid::Workflow
        RapidAndroid::LiveSession
    )
    add_executable(test_rapid_live_session_json_deserializer tests/LiveSessionJsonDeserializerTest.cpp)
    add_test(NAME test_rapid_live_session_json_deserializer COMMAND test_rapid_live_session_json_deserializer)
    target_link_libraries(test_rapid_live_session_json_deserializer PRIVATE
        Qt6::Test
        RapidAndroid::TestHelper
        RapidAndroid::Common
        RapidAndroid::LiveSession
    )
endif()
