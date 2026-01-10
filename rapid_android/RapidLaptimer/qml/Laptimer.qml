// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import Rapid.Controls
import Rapid.Laptimer

Control {
    id: laptimer

    required property var deviceManagement

    ColumnLayout {
        anchors.fill: parent
        spacing: 12

        Button {
            id: addDeviceButton
            text: "+ Add New Device"
            Layout.fillWidth: true
            Layout.preferredHeight: 62
            Layout.leftMargin: 10
            Layout.rightMargin: 10

            background: Rectangle {
                color: "#0682C9"
                radius: 4
            }

            font.pixelSize: 18
            font.bold: true
            Material.foreground: "#ffffff"

            onClicked: {
                laptimerConfigDialog.state = "new";
                laptimerConfigDialog.open();
            }
        }

        ListView {
            id: laptimerList
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            spacing: 12
            clip: true
            model: laptimer.deviceManagement.model

            delegate: ListDelegateBackground {
                id: listDelegate
                width: laptimerList.width
                height: 156
                required property var laptimer
                required property var index

                DelegateContent {
                    anchors.fill: listDelegate
                    laptimerName: listDelegate.laptimer.name
                    laptimerIpAddress: "%1:%2".arg(listDelegate.laptimer.ip).arg(listDelegate.laptimer.port)
                    active: listDelegate.laptimer.defaultDevice

                    onActivateClicked: {
                        laptimer.deviceManagement.enable(listDelegate.laptimer);
                    }

                    onConfigureClicked: {
                        laptimerConfigDialog.laptimer = listDelegate.laptimer;
                        contextMenu.open();
                    }
                }
            }
        }
    }

    ContextMenu {
        id: contextMenu
        model: ListModel {
            ListElement {
                entryText: qsTr("Edit Laptimer")
                iconSource: "qrc:/qt/qml/Rapid/Laptimer/img/Edit.svg"
                clickedAction: function () {
                    contextMenu.close();
                    laptimerConfigDialog.state = "edit";
                    laptimerConfigDialog.open();
                }
            }

            ListElement {
                entryText: qsTr("Delete Laptimer")
                iconSource: "qrc:/qt/qml/Rapid/Laptimer/img/Trash.svg"
                clickedAction: function () {
                    contextMenu.close();
                    laptimer.deviceManagement.remove(laptimerConfigDialog.laptimer);
                }
            }
        }
    }

    LaptimerConfigurationDialog {
        id: laptimerConfigDialog
        width: laptimer.width * 0.8

        deviceManagement: laptimer.deviceManagement
    }
}
