// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import Rapid.Controls
import Rapid.Android

Control {
    id: liveSession

    property var liveSessionMgmt: GlobalContext.liveSessionManagement
    property var deviceMgmt: GlobalContext.deviceManagement

    ColumnLayout {
        id: liveSessionLayout
        anchors.fill: parent
        spacing: 12
        anchors.margins: 10

        ListDelegateBackground {
            Layout.preferredWidth: liveSessionLayout.width
            Layout.preferredHeight: 156
            Layout.alignment: Qt.AlignTop
            borderColor: "#0682C9"

            ColumnLayout {
                anchors.centerIn: parent
                anchors.margins: 10
                spacing: 5

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

            Text {
                text: Qt.formatTime(liveSession.liveSessionMgmt.currentLaptime, "hh:mm:ss.zzz")
                font.pixelSize: 36
                color: "#0682C9"
                font.bold: true
                Layout.alignment: Qt.AlignHCenter
            }

            Text {
                text: qsTrId("Lap %1 inprogress".arg(5))
                font.pixelSize: 12
                opacity: 0.7
                color: "#555555"
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 0
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
