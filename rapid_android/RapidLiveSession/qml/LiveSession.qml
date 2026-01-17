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
                        text: TimeFormatter.formatTime(liveSession.liveSessionMgmt.currentLaptime)
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

            ListDelegateBackground {
                id: lastLaptimeContainer
                Layout.fillWidth: true
                Layout.preferredHeight: 120

                ColumnLayout {
                    anchors.margins: 10
                    width: lastLaptimeContainer.width
                    spacing: 5

                    Item {
                        id: lastLaptimeRow
                        Layout.fillWidth: true
                        Layout.preferredHeight: lastLaptimeLabel.implicitHeight + checkeredFlagImage.implicitHeight

                        Text {
                            id: lastLaptimeLabel
                            text: qsTrId("Last Laptime")
                            font.pixelSize: 14
                            anchors.left: parent.left
                            anchors.leftMargin: 15
                            anchors.top: parent.top
                            anchors.topMargin: 15
                            color: "#555555"
                        }

                        Image {
                            id: checkeredFlagImage
                            source: "qrc:/qt/qml/Rapid/LiveSession/img/CheckeredFlag.svg"
                            sourceSize.width: 18
                            sourceSize.height: 18
                            fillMode: VectorImage.PreserveAspectFit
                            anchors.right: lastLaptimeRow.right
                            anchors.rightMargin: 15
                            anchors.top: lastLaptimeRow.top
                            anchors.topMargin: 15
                            layer.enabled: true
                            layer.smooth: true
                            layer.effect: MultiEffect {
                                brightness: 1.0
                                colorization: 1.0
                                colorizationColor: "#0682C9"
                            }
                        }
                    }

                    Text {
                        text: TimeFormatter.formatTime(liveSession.liveSessionMgmt.lastLaptime)
                        font.pixelSize: 28
                        color: "#000000"
                        font.bold: true
                        Layout.leftMargin: 15
                        Layout.alignment: Qt.AlignLeft
                    }

                    Item {
                        id: lastLaptimeDiffRow
                        Layout.fillWidth: true
                        Layout.preferredHeight: lastLaptimeDiffLabel.implicitHeight
                        Layout.bottomMargin: 20

                        Text {
                            id: lastLaptimeDiffLabel
                            text: TimeFormatter.formatTime(liveSession.liveSessionMgmt.bestLaptimeDiff)
                            font.pixelSize: 16
                            color: "#555555"
                            anchors.right: lastLaptimeDiffRow.right
                            anchors.rightMargin: 15
                        }
                    }
                }
            }

            ListDelegateBackground {
                id: bestLaptimeContainer
                Layout.fillWidth: true
                Layout.preferredHeight: bestAvgLaptimeRow.implicitHeight

                RowLayout {
                    id: bestAvgLaptimeRow
                    anchors.fill: parent

                    ColumnLayout {
                        Layout.margins: 10
                        spacing: 5

                        Text {
                            text: qsTrId("Best Laptime")
                            font.pixelSize: 14
                            Layout.leftMargin: 15
                            Layout.topMargin: 10
                            color: "#555555"
                            Layout.alignment: Qt.AlignLeft
                        }

                        Text {
                            text: TimeFormatter.formatTime(liveSession.liveSessionMgmt.bestLaptime)
                            font.pixelSize: 28
                            Layout.leftMargin: 15
                            color: "#0682C9"
                            font.bold: true
                            Layout.alignment: Qt.AlignLeft
                        }
                        Text {
                            text: qsTrId("Lap: %1").arg(liveSession.liveSessionMgmt.bestLaptimeLap)
                            font.pixelSize: 12
                            Layout.leftMargin: 15
                            color: "#555555"
                            font.bold: true
                            Layout.alignment: Qt.AlignLeft
                            visible: liveSession.liveSessionMgmt.bestLaptimeLap > 0
                        }
                    }

                    ColumnLayout {
                        Layout.margins: 10
                        spacing: 5
                        Layout.alignment: Qt.AlignRight | Qt.AlignTop

                        Text {
                            text: qsTrId("Average Laptime")
                            font.pixelSize: 14
                            Layout.rightMargin: 15
                            Layout.topMargin: 10
                            color: "#000000"
                            Layout.alignment: Qt.AlignRight
                        }

                        Text {
                            text: TimeFormatter.formatTime(liveSession.liveSessionMgmt.averageLaptime)
                            font.pixelSize: 28
                            Layout.rightMargin: 15
                            color: "#0682C9"
                            font.bold: true
                            Layout.alignment: Qt.AlignRight
                        }
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
                                colorization: 1.0
                                colorizationColor: "#0682C9"
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
