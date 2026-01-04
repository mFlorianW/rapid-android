// SPDX-FileCopyrightText: 2025 All contributors
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

    ListView {
        id: laptimeView
        anchors.fill: parent
        anchors.margins: 10
        model: GlobalContext.sessionAnalyzer.lapListModel

        spacing: 4

        delegate: Control {
            id: delegate
            width: laptimeView.width
            height: 56

            required property var laptime
            required property int index
            spacing: 4

            contentItem: ColumnLayout {
                id: contentLayout
                Layout.fillWidth: true

                RowLayout {
                    id: listContent
                    Layout.fillWidth: true
                    spacing: 0

                    Text {
                        text: "%1:".arg(delegate.index + 1)
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

                Rectangle {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter
                    Layout.preferredHeight: 1
                    color: "#e5e7eb"
                }
            }
        }
    }
}
