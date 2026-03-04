// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import Rapid.Controls
import Rapid.Android
import Rapid.Session

Control {
    id: laptimer

    TabBar {
        id: tabbar
        width: parent.width

        Material.accent: "#0682C9"

        TabButton {
            text: qsTr("Local Sessions")
            icon.source: "qrc:/qt/qml/Rapid/Session/img/Local.svg"
        }
        TabButton {
            text: qsTr("Remote Sessions")
            icon.source: "qrc:/qt/qml/Rapid/Session/img/Remote.svg"
        }
    }

    StackLayout {
        id: stackLayout
        width: parent.width
        anchors.top: tabbar.bottom
        anchors.topMargin: 12
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        currentIndex: tabbar.currentIndex

        Item {
            id: localSessionTab
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: localLaptimerSessionList
                anchors.fill: parent
                anchors.margins: 10
                model: GlobalContext.localSessionManagement.model
                spacing: 12
                clip: true
                delegate: ListDelegate {
                    id: listDelegate
                    width: localLaptimerSessionList.width
                    required property var sessionInfo
                    titleName: listDelegate.sessionInfo.trackName
                    additionalTitleText: listDelegate.sessionInfo.date
                    detail1Text: qsTr("Laps: %1").arg(listDelegate.sessionInfo.laps)

                    buttonLeftText: qsTr("Analyze")
                    buttonLeftIcon: "qrc:/qt/qml/Rapid/Session/img/Analyze.svg"
                    buttonRightText: qsTr("Delete")
                    buttonRightIcon: "qrc:/qt/qml/Rapid/Session/img/Trash.svg"

                    onLeftButtonClicked: {
                        GlobalContext.localSessionManagement.load(listDelegate.sessionInfo);
                        infoDialog.title = qsTr("Loading Session");
                        infoDialog.open();
                    }

                    onRightButtonClicked: {
                        GlobalContext.localSessionManagement.remove(listDelegate.sessionInfo);
                    }
                }
            }

            RoundButton {
                id: sessionTabRefeshButton
                height: 60
                width: height
                icon.source: "qrc:/qt/qml/Rapid/Session/img/Refresh.svg"

                Material.roundedScale: Material.FullScale
                Material.foreground: "#FFFFFF"
                Material.background: "#0682C9"

                anchors.bottom: localSessionTab.bottom
                anchors.bottomMargin: 10
                anchors.right: localSessionTab.right
                anchors.rightMargin: 15

                onClicked: {
                    GlobalContext.localSessionManagement.refreshSessionInfos();
                }
            }

            StackLayout.onIsCurrentItemChanged: {
                if (StackLayout.isCurrentItem) {
                    sessionTabRefeshButton.clicked();
                }
            }

            LaptimeDialog {
                id: laptimeDialog
                title: qsTr("Session Analysis")
                height: laptimer.height * 0.8
                width: laptimer.width * 0.8
            }

            InfoDialog {
                id: infoDialog
                width: laptimer.width * 0.8
                progressbar: true
            }

            Connections {
                target: GlobalContext.localSessionManagement
                function onSessionLoaded(success, info, session) {
                    if (success) {
                        GlobalContext.sessionAnalyzer.analyzeSession(session);
                    }
                }
            }

            Connections {
                target: GlobalContext.sessionAnalyzer
                function onSessionAnalyzed() {
                    infoDialog.close();
                    laptimeDialog.open();
                }
            }
        }

        Item {
            id: laptimerSessionTab
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: laptimerSessionList
                anchors.fill: parent
                anchors.margins: 10
                model: GlobalContext.deviceSessionManagement.model
                spacing: 12
                clip: true
                delegate: ListDelegate {
                    id: listDeviceSessionDelegate
                    width: laptimerSessionList.width
                    required property var sessionInfo
                    titleName: listDeviceSessionDelegate.sessionInfo.trackName
                    additionalTitleText: listDeviceSessionDelegate.sessionInfo.date
                    detail1Text: qsTr("Laps: %1").arg(listDeviceSessionDelegate.sessionInfo.laps)

                    buttonLeftText: qsTr("Delete")
                    buttonLeftIcon: "qrc:/qt/qml/Rapid/Session/img/Trash.svg"
                    buttonRightText: qsTr("Download")
                    buttonRightIcon: "qrc:/qt/qml/Rapid/Session/img/Download.svg"

                    onLeftButtonClicked: {
                        var settings = GlobalContext.deviceManagement.activeLaptimer;
                        GlobalContext.deviceSessionManagement.deleteSession(settings, listDeviceSessionDelegate.sessionInfo.id);
                    }

                    onRightButtonClicked: {
                        var settings = GlobalContext.deviceManagement.activeLaptimer;
                        GlobalContext.deviceSessionManagement.downloadSession(settings, listDeviceSessionDelegate.sessionInfo.id);
                        downloadDialog.trackName = listDeviceSessionDelegate.sessionInfo.trackName;
                        downloadDialog.date = listDeviceSessionDelegate.sessionInfo.date;
                        downloadDialog.open();
                    }
                }
            }

            RoundButton {
                id: laptimerSessionTabRefeshButton
                height: 60
                width: height
                icon.source: "qrc:/qt/qml/Rapid/Session/img/Refresh.svg"

                Material.roundedScale: Material.FullScale
                Material.foreground: "#FFFFFF"
                Material.background: "#0682C9"

                anchors.bottom: laptimerSessionTab.bottom
                anchors.bottomMargin: 10
                anchors.right: laptimerSessionTab.right
                anchors.rightMargin: 15

                onClicked: {
                    var settings = GlobalContext.deviceManagement.activeLaptimer;
                    GlobalContext.deviceSessionManagement.synchronizeSessionInfos(settings);
                    refreshDialog.open();
                }
            }

            InfoDialog {
                id: downloadDialog
                width: laptimer.width * 0.8
                progressbar: true

                property string trackName: ""
                property string date: ""
                standardButtons: Dialog.NoButton

                title: qsTr("Downloading Session")
                text: qsTr("Downloading session for track: %1").arg(downloadDialog.trackName)
                additionalText: qsTr("Date: %1").arg(downloadDialog.date)
            }

            InfoDialog {
                id: refreshDialog
                width: laptimer.width * 0.8
                title: qsTr("Refreshing Sessions")
            }

            Connections {
                target: GlobalContext.deviceSessionManagement
                function onSessionDownloaded(sessionId, result, errorMessage) {
                    downloadDialog.close();
                }

                function onSessionInfosSynchronized(result, errorMessage) {
                    if (result === DeviceSessionManagement.Success) {
                        refreshDialog.close();
                    } else {
                        refreshDialog.text = qsTr("Error:");
                        refreshDialog.additionalText = errorMessage;
                    }
                }
            }

            StackLayout.onIsCurrentItemChanged: {
                if (StackLayout.isCurrentItem) {
                    laptimerSessionTabRefeshButton.clicked();
                }
            }
        }
    }
}
