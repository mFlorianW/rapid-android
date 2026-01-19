// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Ctrl
import QtQuick.Controls.Material
import QtQuick.Effects
import Rapid.Android
import Rapid.Controls

Ctrl.ApplicationWindow {
    id: window
    visible: true
    title: qsTr("Rapid Android")
    width: 360
    height: 720

    background: Rectangle {
        color: "#FAFAFA"
    }

    header: Ctrl.Control {
        id: header
        height: 60

        Material.elevation: 6

        background: Rectangle {
            color: "#0682C9"
        }

        RowLayout {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 12
            spacing: 12

            Image {
                id: titleImage
                source: "qrc:/qt/qml/Rapid/Android/img/Stopwatch.svg"
                sourceSize.width: 24
                sourceSize.height: 24
                layer.enabled: true
                layer.effect: MultiEffect {
                    brightness: 1.0
                    colorization: 1.0
                    colorizationColor: "#ffffff"
                }
            }

            Text {
                id: navBarText
                text: "<b>Rapid Laptimer</b>"
                font.pointSize: 14
                color: "#ffffff"
            }
        }
    }

    Loader {
        id: pageLoader
        anchors.fill: parent
        sourceComponent: livePage
    }

    footer: Ctrl.ToolBar {
        id: bottomNavbar
        height: 56

        Material.elevation: 6

        background: Rectangle {
            color: "#ffffff"
            border.color: "#E5E7EB"
            border.width: 1
        }

        // property string activePage: pageLoader.sourceComponent.

        RowLayout {
            anchors.fill: parent

            NavButton {
                id: liveNavButton
                width: parent.width / 0.25
                Layout.alignment: Qt.AlignHCenter
                Layout.fillHeight: true

                iconSource: "qrc:/qt/qml/Rapid/Android/img/Live.svg"
                text: qsTr("Live")
                onClicked: {
                    pageLoader.sourceComponent = livePage;
                }
            }

            NavButton {
                id: sessionsNavButton
                width: parent.width / 0.25
                Layout.alignment: Qt.AlignHCenter
                Layout.fillHeight: true

                iconSource: "qrc:/qt/qml/Rapid/Android/img/Sessions.svg"
                text: qsTr("Sessions")
                onClicked: {
                    pageLoader.sourceComponent = sessionsPage;
                }
            }

            NavButton {
                id: tracksNavButton
                width: parent.width / 0.25
                Layout.alignment: Qt.AlignHCenter
                Layout.fillHeight: true

                iconSource: "qrc:/qt/qml/Rapid/Android/img/RaceTrack.svg"
                text: qsTr("Tracks")
                onClicked: {
                    pageLoader.sourceComponent = tracksPage;
                }
            }

            NavButton {
                id: laptimerNavButton
                width: parent.width / 0.25
                Layout.alignment: Qt.AlignHCenter
                Layout.fillHeight: true
                selected: true

                iconSource: "qrc:/qt/qml/Rapid/Android/img/Laptimer.svg"
                text: qsTr("Laptimer")
                onClicked: {
                    pageLoader.sourceComponent = laptimerPage;
                }
            }
        }

        states: [
            State {
                name: "live"
                when: pageLoader.sourceComponent === livePage
                PropertyChanges {
                    target: laptimerNavButton
                    selected: false
                }
                PropertyChanges {
                    target: liveNavButton
                    selected: true
                }
                PropertyChanges {
                    target: sessionsNavButton
                    selected: false
                }
                PropertyChanges {
                    target: tracksNavButton
                    selected: false
                }
            },
            State {
                name: "sessions"
                when: pageLoader.sourceComponent === sessionsPage
                PropertyChanges {
                    target: laptimerNavButton
                    selected: false
                }
                PropertyChanges {
                    target: liveNavButton
                    selected: false
                }
                PropertyChanges {
                    target: sessionsNavButton
                    selected: true
                }
                PropertyChanges {
                    target: tracksNavButton
                    selected: false
                }
            },
            State {
                name: "tracks"
                when: pageLoader.sourceComponent === tracksPage
                PropertyChanges {
                    target: laptimerNavButton
                    selected: false
                }
                PropertyChanges {
                    target: liveNavButton
                    selected: false
                }
                PropertyChanges {
                    target: sessionsNavButton
                    selected: false
                }
                PropertyChanges {
                    target: tracksNavButton
                    selected: true
                }
            },
            State {
                name: "laptimer"
                when: pageLoader.sourceComponent === laptimerPage
                PropertyChanges {
                    target: laptimerNavButton
                    selected: true
                }
                PropertyChanges {
                    target: liveNavButton
                    selected: false
                }
                PropertyChanges {
                    target: sessionsNavButton
                    selected: false
                }
                PropertyChanges {
                    target: tracksNavButton
                    selected: false
                }
            }
        ]
    }

    Component {
        id: livePage
        LivePage {}
    }

    Component {
        id: sessionsPage
        SessionsPage {}
    }

    Component {
        id: tracksPage
        TracksPage {}
    }

    Component {
        id: laptimerPage
        LaptimerPage {}
    }
}
