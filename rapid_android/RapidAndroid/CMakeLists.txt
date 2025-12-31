qt_add_qml_module(RapidAndroid
    URI "Rapid.Android"
    OUTPUT_DIRECTORY "Rapid/Android"
    IMPORT_PATH "Rapid/Android"
    STATIC
    QML_FILES
        qml/RapidAndroid.qml
        # pages
        qml/pages/LaptimerPage.qml
        qml/pages/LivePage.qml
        qml/pages/SessionsPage.qml
        qml/pages/TracksPage.qml
    RESOURCES
        img/Stopwatch.svg
        img/Live.svg
        img/Sessions.svg
        img/Laptimer.svg
        img/Tracks.svg
    SOURCES
        GlobalContext.cpp
        GlobalContext.hpp
        FileSettingsBackend.cpp
        FileSettingsBackend.hpp
        LoggingCategories.cpp
        LoggingCategories.hpp
)

add_library(RapidAndroid::Android ALIAS RapidAndroid)
add_library(RapidAndroid::AndroidPlugin ALIAS RapidAndroidplugin)

target_link_libraries(RapidAndroid
    PUBLIC
        Qt6::Quick
        RapidAndroid::Workflow
        RapidAndroid::ControlsPlugin
        RapidAndroid::LaptimerPlugin
        RapidAndroid::Session
        RapidAndroid::SessionPlugin
)

target_include_directories(RapidAndroid
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
