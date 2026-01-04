// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import Rapid.Android

Dialog {
    id: laptimerConfigDialog
    anchors.centerIn: parent
    standardButtons: Dialog.Save | Dialog.Cancel
    Material.accent: "#0682C9"

    property var laptimer: null
    property alias state: laptimerInputDialogLayout.state
    required property var deviceManagement

    ColumnLayout {
        id: laptimerInputDialogLayout
        anchors.fill: parent
        spacing: 10

        TextField {
            id: deviceInputPopupDeviceName
            Layout.fillWidth: true
            placeholderText: "Name"
        }

        TextField {
            id: deviceInputPopupDeviceIp
            Layout.fillWidth: true
            placeholderText: "IP Address"
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            validator: RegularExpressionValidator {
                id: ipAddressRegExValidation
                regularExpression: /^(25[0-5]|2[0-4][0-9]|1?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|1?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|1?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|1?[0-9][0-9]?)$/
            }
        }

        TextField {
            id: deviceInputPopupDevicePort
            Layout.fillWidth: true
            placeholderText: "Device Port"
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            validator: IntValidator {
                id: portValidator
                bottom: 0
                top: 65535
            }
        }

        states: [
            State {
                name: "new"
                PropertyChanges {
                    laptimerConfigDialog {
                        title: qsTr("New Laptimer")
                    }
                    deviceInputPopupDeviceName {
                        text: "Rapid"
                    }
                    deviceInputPopupDeviceIp {
                        text: "192.168.1.1"
                    }
                    deviceInputPopupDevicePort {
                        text: "27018"
                    }
                }
            },
            State {
                name: "edit"
                PropertyChanges {
                    laptimerConfigDialog {
                        title: qsTr("Edit Laptimer")
                    }
                    deviceInputPopupDeviceName {
                        text: laptimerConfigDialog.laptimer.name
                    }
                    deviceInputPopupDeviceIp {
                        text: laptimerConfigDialog.laptimer.ip
                    }
                    deviceInputPopupDevicePort {
                        text: laptimerConfigDialog.laptimer.port
                    }
                }
            }
        ]
    }

    onAccepted: {
        var device = laptimerConfigDialog.deviceManagement.create(deviceInputPopupDeviceName.text, deviceInputPopupDeviceIp.text, deviceInputPopupDevicePort.text, false);
        if (laptimerInputDialogLayout.state === "new") {
            laptimerConfigDialog.deviceManagement.store(device);
        } else if (laptimerInputDialogLayout.state === "edit") {
            laptimerConfigDialog.deviceManagement.update(laptimerConfigDialog.laptimer, device);
        } else {
            console.log("Unknown state");
        }
    }
}
