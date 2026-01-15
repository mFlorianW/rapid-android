// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Effects
import QtQuick.VectorImage
import Rapid.Controls
import Rapid.Android

Control {
    id: liveSession

    property var liveSessionMgmt: GlobalContext.liveSessionManagement
    property var deviceMgmt: GlobalContext.deviceManagement

    contentItem: Item {
        ColumnLayout {
            id: liveSessionLayout
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 12
            anchors.margins: 10

            ListDelegateBackground {
                Layout.preferredWidth: liveSessionLayout.width
                Layout.preferredHeight: 156
                Layout.alignment: Qt.AlignTop
                borderColor: "#0682C9"

                ColumnLayout {
                    anchors.margins: 10
                    spacing: 5
                    anchors.centerIn: parent

                    Text {
                        text: Qt.formatTime(liveSession.liveSessionMgmt.currentLaptime, "hh:mm:ss.zzz")
                        font.pixelSize: 52
                        color: "#0682C9"
                        font.bold: true
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Text {
                        text: qsTrId("Lap %1".arg(liveSession.liveSessionMgmt.lapCount + 1))
                        font.pixelSize: 16
                        opacity: 0.7
                        color: "#555555"
                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: 0
                    }
                }
            }

            RowLayout {
                id: infoRow
                Layout.preferredWidth: liveSessionLayout.width
                // Layout.alignment: Qt.AlignTop

                Rectangle {
                    color: "#F5F5F5"
                    radius: 8
                    Layout.preferredHeight: 110
                    Layout.preferredWidth: 100

                    ColumnLayout {
                        anchors.centerIn: parent
                        anchors.margins: 10

                        Image {
                            id: flagImage
                            source: "qrc:/qt/qml/Rapid/LiveSession/img/Flag.svg"
                            sourceSize.width: 18
                            sourceSize.height: 18
                            fillMode: VectorImage.PreserveAspectFit
                            Layout.alignment: Qt.AlignHCenter
                            layer.enabled: true
                            layer.smooth: true
                            layer.effect: MultiEffect {
                                brightness: 1.0
                                colorization: 1.0          // enable colorization
                                colorizationColor: "#0682C9" // set color to blue
                            }
                        }

                        Text {
                            text: liveSession.liveSessionMgmt.lapCount
                            font.pixelSize: 28
                            color: "#000000"
                            font.bold: true
                            Layout.alignment: Qt.AlignHCenter
                        }

                        Text {
                            text: qsTrId("Total Laps")
                            font.pixelSize: 14
                            opacity: 0.7
                            color: "#555555"
                            Layout.alignment: Qt.AlignHCenter
                        }
                    }
                }
            }
        }
    }

    Connections {
        target: liveSession.deviceMgmt
        function onActiveLaptimerChanged() {
            liveSession.liveSessionMgmt.setDeviceSettings(liveSession.deviceMgmt.activeLaptimer);
        }
    }

    Component.onCompleted: {
        liveSessionMgmt.setDeviceSettings(deviceMgmt.activeLaptimer);
    }
}
