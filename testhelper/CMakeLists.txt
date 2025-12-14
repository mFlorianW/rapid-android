add_library(RapidAndroidTestHelper STATIC)
add_library(RapidAndroid::TestHelper ALIAS RapidAndroidTestHelper)

target_sources(RapidAndroidTestHelper PRIVATE
    include/TestHelper/SettingsMemoryBackend.hpp

    src/SettingsMemoryBackend.cpp
)

target_include_directories(RapidAndroidTestHelper
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# C++ standard
target_compile_features(RapidAndroidTestHelper PUBLIC cxx_std_20)

# Link dependencies (adjust as needed)
# Example: Qt Test support and trompeloeil for mocks
find_package(Qt6 REQUIRED COMPONENTS Test Core)
target_link_libraries(RapidAndroidTestHelper
    PUBLIC
        Qt6::Core
        Qt6::Test
        RapidAndroid::Common
)

