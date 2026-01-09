// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import Rapid.Controls

ListDelegateBackground {
    id: delegateContent
    implicitHeight: 156

    property alias titleName: titleName.text
    property alias additionalTitleText: additionalTitleText.text
    property alias statusIndicator: statusIndicator.active

    property alias buttonLeftText: leftButton.text
    property alias buttonLeftIcon: leftButton.icon.source

    property alias buttonRightText: righttButton.text
    property alias buttonRightIcon: righttButton.icon.source

    property alias detail1Icon: detail1Icon.source
    property alias detail1Text: detail1Text.text

    property alias detail2Icon: detail2Icon.source
    property alias detail2Text: detail2Text.text

    property alias statusIndicatorText: statusIndicator.statusText
    property alias statusIndicatorActive: statusIndicator.active

    signal leftButtonClicked
    signal rightButtonClicked

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
                id: titleName
                Layout.alignment: Qt.AlignLeft
                Layout.leftMargin: 12
                font.pixelSize: 16
                font.bold: true
                color: "#000000"
                Layout.fillWidth: true
            }

            StatusIndicator {
                id: statusIndicator
                visible: statusIndicator.statusText !== ""
                Layout.alignment: Qt.AlignRight | Qt.AlignHCenter | Qt.AlignVCenter
                Layout.rightMargin: 12
            }
        }

        Text {
            id: additionalTitleText
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
                    id: detail1Icon
                    sourceSize.width: 28
                    sourceSize.height: 28
                    fillMode: Image.PreserveAspectFit
                    Layout.alignment: Qt.AlignVCenter
                }

                Text {
                    id: detail1Text
                    font.pixelSize: 12
                    color: "#6B7280"
                }
            }

            RowLayout {
                id: lastSyncStatus
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignRight
                spacing: 2

                Image {
                    id: detail2Icon
                    sourceSize.width: 18
                    sourceSize.height: 18
                    fillMode: Image.PreserveAspectFit
                    Layout.alignment: Qt.AlignVCenter
                }

                Text {
                    id: detail2Text
                    font.pixelSize: 12
                    color: "#6B7280"
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
                id: leftButton
                Layout.preferredHeight: 50
                Layout.minimumWidth: (columnLayout.width / 2) - 12
                Material.foreground: "#515A67"
                icon.width: 60
                icon.height: 60
                icon.color: "#000000"

                background: Rectangle {
                    color: "#F5F5F5"
                    radius: 4
                }
                spacing: 0
                onClicked: delegateContent.leftButtonClicked()
            }

            Button {
                id: righttButton
                Layout.preferredHeight: 50
                Layout.minimumWidth: (columnLayout.width / 2) - 12
                property var forgroundColor
                icon.width: 40
                icon.height: 40
                icon.color: "#000000"

                Material.foreground: forgroundColor

                background: Rectangle {
                    color: "#F5F5F5"
                    radius: 4
                }
                spacing: 0
                onClicked: delegateContent.rightButtonClicked()
            }
        }
    }
}
