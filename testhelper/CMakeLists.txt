# SPDX-FileCopyrightText: 2026 All contributors
#
# SPDX-License-Identifier: GPL-2.0-or-later

find_package(GTest REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS HttpServer)

add_library(RapidAndroidTestHelper STATIC)
add_library(RapidAndroid::TestHelper ALIAS RapidAndroidTestHelper)

target_sources(RapidAndroidTestHelper PRIVATE
    include/TestHelper/SettingsMemoryBackend.hpp
    include/TestHelper/SessionStorageMock.hpp
    include/TestHelper/FakeLaptimer.hpp
    include/TestHelper/Session.hpp
    include/TestHelper/Session

    src/SettingsMemoryBackend.cpp
    src/FakeLaptimer.cpp
    src/Session.cpp
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
        Qt6::HttpServer
        GTest::gmock
        RapidAndroid::Common
        RapidAndroid::Workflow
)

