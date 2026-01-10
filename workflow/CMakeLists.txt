# SPDX-FileCopyrightText: 2026 All contributors
#
# SPDX-License-Identifier: GPL-2.0-or-later

add_library(RapidAndroidWorkflow STATIC)
add_library(RapidAndroid::Workflow ALIAS RapidAndroidWorkflow)

target_sources(RapidAndroidWorkflow PRIVATE
    # Publics
    include/Workflow/IDeviceManagement.hpp
    include/Workflow/DeviceManagement.hpp
    include/Workflow/ISessionStorage.hpp
    include/Workflow/ILocalSessionManagement.hpp
    include/Workflow/IDeviceSessionManagement.hpp
    include/Workflow/HttpDeviceSessionManagement.hpp
    include/Workflow/ISessionDeserializer.hpp
    include/Workflow/ISessionSerializer.hpp
    include/Workflow/LocalSessionManagement.hpp
    include/Workflow/ISessionAnalyzer.hpp
    include/Workflow/SessionAnalyzer.hpp

    # Privates
    include/Workflow/Private/SessionListModel.hpp
    include/Workflow/Private/LapListModel.hpp
    include/Workflow/Private/SessionSortModel.hpp

    src/LoggingCategories.cpp
    src/LoggingCategories.hpp
    src/DeviceManagement.cpp
    src/IDeviceManagement.cpp
    src/SessionListModel.cpp
    src/HttpDeviceSessionManagement.cpp
    src/LocalSessionManagement.cpp
    src/LapListModel.cpp
    src/SessionAnalyzer.cpp
    src/SessionSortModel.cpp
)

target_link_libraries(RapidAndroidWorkflow PUBLIC
    Qt6::Core
    Qt6::Concurrent
    RapidAndroid::Common
)

target_include_directories(RapidAndroidWorkflow
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(ENABLE_TEST)
    add_executable(test_workflow_device_management tests/DeviceManagementTest.cpp)
    add_test(NAME test_workflow_device_management COMMAND test_workflow_device_management)
    target_link_libraries(test_workflow_device_management PRIVATE
        Qt6::Test
        RapidAndroid::Common
        RapidAndroid::Workflow
        RapidAndroid::TestHelper
    )

    add_executable(test_workflow_http_device_session_management tests/HttpDeviceSessionManagementTest.cpp)
    add_test(NAME test_workflow_http_device_session_management COMMAND test_workflow_http_device_session_management)
    target_link_libraries(test_workflow_http_device_session_management PRIVATE
        Qt6::Test
        RapidAndroid::Common
        RapidAndroid::Workflow
        RapidAndroid::TestHelper
    )

    add_executable(test_workflow_local_session_management tests/LocalSessionManagementTest.cpp)
    add_test(NAME test_workflow_local_session_management COMMAND test_workflow_local_session_management)
    target_link_libraries(test_workflow_local_session_management PRIVATE
        Qt6::Test
        RapidAndroid::Common
        RapidAndroid::Workflow
        RapidAndroid::TestHelper
    )
    add_executable(test_workflow_session_analyzer tests/SessionAnalyzerTest.cpp)
    add_test(NAME test_workflow_session_analyzer COMMAND test_workflow_session_analyzer)
    target_link_libraries(test_workflow_session_analyzer PRIVATE
        Qt6::Test
        RapidAndroid::Common
        RapidAndroid::Workflow
        RapidAndroid::TestHelper
    )
endif()
