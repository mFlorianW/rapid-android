add_library(RapidAndroidWorkflow STATIC)
add_library(RapidAndroid::Workflow ALIAS RapidAndroidWorkflow)

target_sources(RapidAndroidWorkflow PRIVATE
    include/Workflow/IDeviceManagement.hpp
    include/Workflow/DeviceManagement.hpp

    src/LoggingCategories.cpp
    src/LoggingCategories.hpp
    src/DeviceManagement.cpp
)

target_link_libraries(RapidAndroidWorkflow PUBLIC
    Qt6::Core
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
endif()
