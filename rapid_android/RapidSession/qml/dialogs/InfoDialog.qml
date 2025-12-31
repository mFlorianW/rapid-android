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
    standardButtons: Dialog.Ok
    Material.accent: "#0682C9"
    modal: true
    closePolicy: Dialog.NoAutoClose

    property alias text: infoText.text
    property alias additionalText: additionalInfoText.text
    property bool progressbar: false

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        Text {
            id: infoText
            font.pixelSize: 16
            Layout.fillWidth: true
            wrapMode: Text.Wrap
        }

        Text {
            id: additionalInfoText
            font.pixelSize: 14
            Layout.fillWidth: true
            wrapMode: Text.Wrap
        }

        ProgressBar {
            id: downloadProgressBar
            Layout.fillWidth: true
            indeterminate: true
            visible: laptimerConfigDialog.progressbar
        }
    }
}
