add_library(RapidAndroidCommon STATIC)
add_library(RapidAndroid::Common ALIAS RapidAndroidCommon)

target_sources(RapidAndroidCommon PRIVATE
    include/Common/GenericListModel.hpp
    include/Common/DeviceSettings.hpp
    include/Common/SettingsReader.hpp
    include/Common/SettingsBackend.hpp
    include/Common/SettingsWriter.hpp
    include/Common/DeviceSettingsListModel.hpp
    include/Common/Private/SettingsKeys.hpp
    include/Common/Lap.hpp
    include/Common/GnssPosition.hpp
    include/Common/Position.hpp
    include/Common/Session.hpp
    include/Common/SessionInfo.hpp

    src/SettingsReader.cpp
    src/SettingsWriter.cpp
    src/DeviceSettingsListModel.cpp
    src/Lap.cpp
    src/Session.cpp
)

target_link_libraries(RapidAndroidCommon PUBLIC
    Qt6::Core
    Qt6::Network
    Qt6::Qml
)

target_include_directories(RapidAndroidCommon
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(ENABLE_TEST)
    add_executable(test_common_generic_list_model tests/GenericListModelTest.cpp)
    add_test(NAME test_common_generic_list_model COMMAND test_common_generic_list_model)
    target_link_libraries(test_common_generic_list_model PRIVATE
        Qt6::Test
        RapidAndroid::Common
    )

    add_executable(test_common_settings_reader tests/SettingsReaderTest.cpp)
    add_test(NAME test_common_settings_reader COMMAND test_common_settings_reader)
    target_include_directories(test_common_settings_reader PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_link_libraries(test_common_settings_reader PRIVATE
        Qt6::Test
        RapidAndroid::Common
        RapidAndroid::TestHelper
    )

    add_executable(test_common_settings_writer tests/SettingsWriterTest.cpp)
    add_test(NAME test_common_settings_writer COMMAND test_common_settings_writer)
    target_include_directories(test_common_settings_writer PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_link_libraries(test_common_settings_writer PRIVATE
        Qt6::Test
        RapidAndroid::Common
        RapidAndroid::TestHelper
    )

    add_executable(test_common_device_settings_list_model tests/DeviceSettingsListModelTest.cpp)
    add_test(NAME test_common_device_settings_list_model COMMAND test_common_device_settings_list_model)
    target_link_libraries(test_common_device_settings_list_model PRIVATE
        Qt6::Test
        RapidAndroid::Common
    )

    add_executable(test_common_lap tests/LapTest.cpp)
    add_test(NAME test_common_lap COMMAND test_common_lap)
    target_link_libraries(test_common_lap PRIVATE
        Qt6::Test
        RapidAndroid::Common
    )
endif()
