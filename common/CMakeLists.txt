add_library(RapidAndroidCommon STATIC)
add_library(RapidAndroid::Common ALIAS RapidAndroidCommon)

target_sources(RapidAndroidCommon PRIVATE
    include/Common/GenericListModel.hpp
    include/Common/DeviceSettings.hpp
    include/Common/SettingsReader.hpp
    include/Common/SettingsBackend.hpp

    src/SettingsReader.cpp
    src/SettingsKeys.hpp
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
    add_executable(test_common_generic_list_model)
    target_sources(test_common_generic_list_model PRIVATE
        tests/GenericListModelTest.cpp
    )
    target_link_libraries(test_common_generic_list_model PRIVATE
        Qt6::Test
        RapidAndroid::Common
    )
    add_test(NAME test_common_generic_list_model COMMAND test_common_generic_list_model)

    add_executable(test_common_settings_reader)
    target_sources(test_common_settings_reader PRIVATE
        tests/SettingsReaderTest.cpp
        tests/SettingsMemoryBackend.cpp
        tests/SettingsMemoryBackend.hpp
    )
    target_include_directories(test_common_settings_reader PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_link_libraries(test_common_settings_reader PRIVATE
        Qt6::Test
        RapidAndroid::Common
    )
    add_test(NAME test_common_settings_reader COMMAND test_common_settings_reader)
endif()
