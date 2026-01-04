// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import QtQuick.Controls

Control {
    id: menuButton

    background: Rectangle {
        color: "#ffffff"
    }

    property alias text: menuButtonText.text
    property alias iconSource: menuButtonImage.source
    property bool selected: false

    signal clicked

    ColumnLayout {
        id: columnLayout
        anchors.centerIn: parent
        spacing: 2

        Image {
            id: menuButtonImage
            Layout.alignment: Qt.AlignHCenter
            sourceSize.width: 24
            sourceSize.height: 24

            ColorOverlay {
                anchors.fill: menuButtonImage
                source: menuButtonImage
                color: menuButton.selected ? "#0682C9" : "#6B7280"
            }
        }

        Text {
            id: menuButtonText
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Live")
            font.pointSize: 10
            color: menuButton.selected ? "#0682C9" : "#6B7280"
        }

        TapHandler {
            id: tapHandler
            gesturePolicy: TapHandler.ReleaseWithinBounds
            onTapped: menuButton.clicked()
        }
    }
}
