// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import Rapid.Android
import Rapid.Session
import Rapid.Controls

Dialog {
    id: laptimeDialog
    anchors.centerIn: parent
    standardButtons: Dialog.Ok

    Material.accent: "#0682C9"

    modal: true
    closePolicy: Dialog.NoAutoClose

    Text {
        id: overviewLabel
        text: qsTr("Overview")
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 16
        anchors.left: parent.left
        anchors.leftMargin: 10
        font.bold: true
    }

    RowLayout {
        id: bestLapTimeRow
        anchors.top: overviewLabel.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10

        Text {
            id: bestLapTimeLabel
            text: qsTr("Best Lap Time:")
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 14
        }

        Text {
            id: bestLapTimeLabelValue
            text: TimeFormatter.formatTime(GlobalContext.sessionAnalyzer.bestLapTime)
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
            font.pixelSize: 14
            Layout.fillWidth: true
            font.bold: true
        }
    }

    RowLayout {
        id: topSpeedRow
        anchors.top: bestLapTimeRow.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10

        Text {
            id: topSpeedLabel
            text: qsTr("Top Speed:")
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 14
        }

        Text {
            id: topSpeedLabelValue
            text: GlobalContext.sessionAnalyzer.topSpeed * 3.6 + " km/h"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
            font.pixelSize: 14
            Layout.fillWidth: true
            font.bold: true
        }
    }

    Rectangle {
        id: analyzeSeparator
        anchors.top: topSpeedRow.bottom
        anchors.topMargin: 12
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        height: 1
        color: "#e5e7eb"
    }

    Text {
        id: lapTimesLabel
        text: qsTr("Lap Times")
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 16
        anchors.top: analyzeSeparator.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 10
        font.bold: true
    }

    ListView {
        id: laptimeView
        anchors.top: lapTimesLabel.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 10
        model: GlobalContext.sessionAnalyzer.lapListModel
        clip: true

        spacing: 4

        delegate: Control {
            id: delegate
            width: laptimeView.width
            height: 56

            required property var laptime
            required property int index
            required property var sectorTimes
            spacing: 4

            contentItem: ColumnLayout {
                id: contentLayout
                Layout.fillWidth: true

                RowLayout {
                    id: listContent
                    Layout.fillWidth: true
                    spacing: 12

                    Image {
                        id: openIndicator
                        source: sectorTimes.visible ? "qrc:/qt/qml/Rapid/Session/img/UpArrow.svg" : "qrc:/qt/qml/Rapid/Session/img/DownArrow.svg"
                        sourceSize.width: 8
                        sourceSize.height: 8
                    }

                    Text {
                        text: delegate.index + 1
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 14
                    }

                    Text {
                        id: laptimeText
                        Layout.fillWidth: true
                        text: delegate.laptime
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                        font.pixelSize: 14
                    }
                }

                ColumnLayout {
                    id: sectorTimes
                    Layout.fillWidth: true
                    Layout.topMargin: 4
                    visible: false

                    Text {
                        id: sectorTimesLabel
                        text: qsTr("Sector Times")
                        font.pixelSize: 12
                        // color: "#6B7280"
                    }

                    ListView {
                        id: sectorTimesList
                        Layout.fillWidth: true
                        implicitHeight: sectorTimesList.contentHeight
                        model: delegate.sectorTimes
                        clip: true

                        delegate: RowLayout {
                            id: sectorTimeRow
                            width: sectorTimesList.width
                            spacing: 4

                            required property int index
                            required property string modelData

                            Text {
                                text: sectorTimeRow.index + 1
                                verticalAlignment: Text.AlignVCenter
                                font.pixelSize: 12
                            }

                            Text {
                                Layout.fillWidth: true
                                text: sectorTimeRow.modelData
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignRight
                                font.pixelSize: 12
                            }
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter
                    Layout.preferredHeight: 1
                    color: "#e5e7eb"
                }

                TapHandler {
                    id: tapHandler
                    acceptedButtons: Qt.LeftButton
                    gesturePolicy: TapHandler.ReleaseWithinBounds
                    onTapped: {
                        sectorTimes.visible = !sectorTimes.visible;
                        delegate.height = delegate.height + (sectorTimes.visible ? sectorTimes.implicitHeight : -sectorTimes.implicitHeight);
                    }
                }
            }
        }
    }
}
