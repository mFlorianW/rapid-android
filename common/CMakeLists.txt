add_library(RapidAndroidCommon STATIC)
add_library(RapidAndroid::Common ALIAS RapidAndroidCommon)

target_sources(RapidAndroidCommon PRIVATE
    include/Common/GenericListModel.hpp
    include/Common/DeviceSettings.hpp
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
    add_executable(test_common)

    target_sources(test_common PRIVATE
        tests/GenericListModelTest.cpp
    )

    target_link_libraries(test_common PRIVATE
        Qt6::Test
        RapidAndroid::Common
    )

    add_test(NAME test_common COMMAND test_common)
endif()
