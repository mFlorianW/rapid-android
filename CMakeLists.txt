cmake_minimum_required(VERSION 3.19)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/configuration/cmake/")

include(FeatureSummary)
include(PreCommit)
include(CompileFlags)
include(Sanitizers)
include(Iwyu)
include(CZVersion)

cz_extract_project_version("${CMAKE_SOURCE_DIR}/.cz.toml" CZ_VERSION)
project(Rapid-Android VERSION "${CZ_VERSION}")
message(STATUS "Project Version: ${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SHARED_DATADIR ${CMAKE_INSTALL_DATAROOTDIR}/rapid)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
    message(STATUS "RPATH set to ${CMAKE_INSTALL_RPATH}")
endif()

option(ENABLE_TEST "Enable unit tests" OFF)
option(ENABLE_PRECOMMIT "Enables automatic pre-commit install." OFF)
option(ENABLE_DESKTOP "This option enables DESKTOP compiling" ON)
option(ENABLE_ANDROID "This option enables the Android build" OFF)
add_feature_info(PreCommit ENABLE_PRECOMMIT "Automatic install of the Python pre-commit tool.")

if(ENABLE_PRECOMMIT)
    setup_precommit()
endif()

if(ENABLE_ANDROID AND NOT UNIX)
    MESSAGE(FATAL_ERROR "ENABLE_ANDROID is only supported on Linux")
endif()

if(ENABLE_ANDROID AND ENABLE_TESTING)
    MESSAGE(FATAL_ERROR "ENABLE_ANDROID and ENABLE_TESTING is not supported")
endif()

if(ENABLE_ANDROID AND ENABLE_DESKTOP)
    MESSAGE(FATAL_ERROR "ENABLE_ANDROID and ENABLE_DESKTOP is not supported")
endif()

if(ENABLE_ANDROID)
    if(ANDROID_NDK_ROOT)
        set(ANDROID_NDK_HOME ${ANDROID_NDK_ROOT})
        MESSAGE(STATUS "Use Android NDK ROOT ${ANDROID_NDK_ROOT}")
    else()
        set(ANDROID_NDK_HOME ${ANDROID_SDK_ROOT}/ndk/26.3.11579264)
        set(ENV{ANDROID_NDK_HOME} ${ANDROID_NDK_HOME})
        MESSAGE(STATUS "Android NDK root not set use default: ${ANDROID_NDK_HOME}")
    endif()

    set(ENV{ANDROID_NDK_HOME} ${ANDROID_NDK_HOME})
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Qml
    Gui
    Quick
    Network
    Location
    QuickControls2
)

if(QT_KNOWN_POLICY_QTP0001)
    qt_policy(SET QTP0001 NEW)
endif()

if(QT_KNOWN_POLICY_QTP0004)
    qt_policy(SET QTP0004 NEW)
endif()

setup_compile_flags()
# setup_iwyu()

if(ENABLE_DESKTOP AND NOT ENABLE_ANDROID AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    setup_sanitizers()
endif()

if(ENABLE_TEST)
    include(CTest)
    find_package(Qt6 REQUIRED Test)
    enable_testing()
endif(ENABLE_TEST)

add_subdirectory(rapid_android)
add_subdirectory(common)

feature_summary(WHAT ALL)
