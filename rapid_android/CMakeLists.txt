find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml)

add_subdirectory(RapidAndroid)
add_subdirectory(RapidControls)
add_subdirectory(RapidLaptimer)

qt_add_executable(rapid_android)

target_sources(rapid_android PRIVATE
    src/rapid_android.cpp
)

target_link_libraries(rapid_android PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::QuickControls2
    RapidAndroid::Android
    RapidAndroid::AndroidPlugin
    RapidAndroid::Controls
    RapidAndroid::ControlsPlugin
)
