# SPDX-FileCopyrightText: 2026 All contributors
#
# SPDX-License-Identifier: GPL-2.0-or-later

qt_add_qml_module(RapidSession
    URI "Rapid.Session"
    OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml/Rapid/Session"
    STATIC
    QML_FILES
        qml/Session.qml
        qml/dialogs/InfoDialog.qml
        qml/dialogs/LaptimeDialog.qml
    SOURCES
        include/RapidSession/FilesystemStorage.hpp
        include/RapidSession/SessionJsonDeserializer.hpp
        include/RapidSession/SessionJsonSerializer.hpp

        src/FilesystemStorage.cpp
        src/LoggingCategories.hpp
        src/LoggingCategories.cpp
        src/SessionJsonDeserializer.cpp
        src/SessionJsonSerializer.cpp
    DEPENDENCIES
        RapidAndroid::RapidAndroidPlugin
        RapidAndroid::ControlsPlugin
    RESOURCES
        img/Refresh.svg
        img/Trash.svg
        img/Analyze.svg
        img/Local.svg
        img/Remote.svg
        img/Download.svg
        img/UpArrow.svg
        img/DownArrow.svg
)

add_library(RapidAndroid::Session ALIAS RapidSession)
add_library(RapidAndroid::SessionPlugin ALIAS RapidSessionplugin)

target_link_libraries(RapidSession
    PUBLIC
        Qt6::Quick
        RapidAndroid::Controls
        RapidAndroid::ControlsPlugin
        RapidAndroid::Workflow
)

target_include_directories(RapidSession
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include
)

if(ENABLE_TEST)
    add_executable(test_rapid_session_filesystemstorage tests/FilesystemStorageTest.cpp)
    add_test(NAME test_rapid_session_filesystemstorage COMMAND test_rapid_session_filesystemstorage)
    target_link_libraries(test_rapid_session_filesystemstorage PRIVATE
        Qt6::Test
        RapidAndroid::Session
        RapidAndroid::TestHelper
    )

    add_executable(test_rapid_session_json_deserializer tests/SessionJsonDeserializerTest.cpp)
    add_test(NAME test_rapid_session_json_deserializer COMMAND test_rapid_session_json_deserializer)
    target_link_libraries(test_rapid_session_json_deserializer PRIVATE
        Qt6::Test
        RapidAndroid::Session
        RapidAndroid::TestHelper
    )
    add_executable(test_rapid_session_json_serializer tests/SessionJsonSerializerTest.cpp)
    add_test(NAME test_rapid_session_json_serializer COMMAND test_rapid_session_json_serializer)
    target_link_libraries(test_rapid_session_json_serializer PRIVATE
        Qt6::Test
        RapidAndroid::Session
        RapidAndroid::TestHelper
    )
endif()
