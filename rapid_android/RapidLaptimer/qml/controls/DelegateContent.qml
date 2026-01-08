// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import Rapid.Controls

Control {
    id: delegateContent

    property alias laptimerName: laptimerName.text
    property alias laptimerIpAddress: laptimerIpAddress.text
    property string gpsStatusText: "--"
    property string lastSyncStatusText: "--"
    property alias statusIndicator: statusIndicator.active
    property bool active: false

    signal configureClicked
    signal activateClicked

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 0

        RowLayout {
            id: titleRow
            Layout.fillWidth: true
            Layout.topMargin: 5
            spacing: 0

            Text {
                id: laptimerName
                Layout.alignment: Qt.AlignLeft
                Layout.leftMargin: 12
                font.pixelSize: 16
                font.bold: true
                color: "#000000"
                Layout.fillWidth: true
            }

            StatusIndicator {
                id: statusIndicator
                statusText: "Offline"
                Layout.alignment: Qt.AlignRight | Qt.AlignHCenter | Qt.AlignVCenter
                Layout.rightMargin: 12
            }
        }

        Text {
            id: laptimerIpAddress
            Layout.leftMargin: 12
            font.pixelSize: 12
            color: "#6B7280"
        }

        RowLayout {
            id: detailRow
            Layout.fillWidth: true
            Layout.leftMargin: 12
            spacing: 20

            RowLayout {
                id: gpsStatusRow
                Layout.fillWidth: true
                spacing: 0

                Image {
                    id: gpsIcon
                    source: "qrc:/qt/qml/Rapid/Laptimer/img/ConnectionVeryHigh.svg"
                    sourceSize.width: 28
                    sourceSize.height: 28
                    fillMode: Image.PreserveAspectFit
                    Layout.alignment: Qt.AlignVCenter
                }

                Text {
                    id: gpsStatus
                    font.pixelSize: 12
                    color: "#6B7280"
                    text: "GPS: %1".arg(delegateContent.gpsStatusText)
                }
            }

            RowLayout {
                id: lastSyncStatus
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignRight
                spacing: 2

                Image {
                    id: syncIcon
                    source: "qrc:/qt/qml/Rapid/Laptimer/img/ClockHourWatch.svg"
                    sourceSize.width: 18
                    sourceSize.height: 18
                    fillMode: Image.PreserveAspectFit
                    Layout.alignment: Qt.AlignVCenter
                }

                Text {
                    id: lastSyncStatusText
                    font.pixelSize: 12
                    color: "#6B7280"
                    text: "Last sync: %1".arg(delegateContent.lastSyncStatusText)
                }
            }
        }

        RowLayout {
            id: buttonRow
            Layout.fillWidth: true
            Layout.leftMargin: 12
            Layout.rightMargin: 12
            spacing: 5

            Button {
                id: configureButton
                text: "Configure"
                Layout.preferredHeight: 50
                Layout.minimumWidth: (columnLayout.width / 2) - 12
                Material.foreground: "#515A67"
                icon.height: 40
                icon.width: 40
                icon.color: "#000000"

                background: Rectangle {
                    color: "#F5F5F5"
                    radius: 4
                }
                icon.source: "qrc:/qt/qml/Rapid/Laptimer/img/Gear.svg"
                spacing: 0
                onClicked: delegateContent.configureClicked()
            }

            Button {
                id: activateButton
                text: "Activate"
                Layout.preferredHeight: 50
                Layout.minimumWidth: (columnLayout.width / 2) - 12
                property var forgroundColor
                icon.height: 40
                icon.width: 40

                Material.foreground: forgroundColor

                background: Rectangle {
                    id: activateButtonBackground
                    radius: 4
                }
                icon.source: "qrc:/qt/qml/Rapid/Laptimer/img/Play.svg"
                spacing: 0
                onClicked: delegateContent.activateClicked()
            }
        }
    }

    states: [
        State {
            name: "activeState"
            when: delegateContent.active === true
            PropertyChanges {
                target: activateButton
                text: qsTr("Active")
                forgroundColor: "#ffffff"
                icon.color: "#FFFFFF"
            }
            PropertyChanges {
                target: activateButtonBackground
                color: "#0682C9"
            }
        },
        State {
            name: "inactiveState"
            when: delegateContent.active === false
            PropertyChanges {
                target: activateButton
                text: qsTr("Activate")
                forgroundColor: "#515A67"
                icon.color: "#000000"
            }
            PropertyChanges {
                target: activateButtonBackground
                color: "#F5F5F5"
            }
        }
    ]
}
