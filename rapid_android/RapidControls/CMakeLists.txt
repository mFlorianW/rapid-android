qt_add_qml_module(RapidControls
    URI "Rapid.Controls"
    OUTPUT_DIRECTORY "Rapid/Controls"
    IMPORT_PATH "Rapid/Controls"
    STATIC
    QML_FILES
        qml/NavButton.qml
)

add_library(Rapid::Controls ALIAS RapidControls)
add_library(Rapid::ControlsPlugin ALIAS RapidControlsplugin)

target_link_libraries(RapidControls
    PUBLIC
        Qt6::Quick
)

target_include_directories(RapidControls
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

