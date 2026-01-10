// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Control {
    id: listDelegate
    implicitWidth: rowLayout.implicitWidth
    implicitHeight: rowLayout.implicitHeight

    property alias statusText: statusText.text
    property bool active: false

    RowLayout {
        id: rowLayout
        spacing: 2

        Rectangle {
            id: colorIndicator
            width: 8
            height: colorIndicator.width
            color: active ? "#22C55E" : "#9CA3AF"
            radius: 32
        }

        Text {
            id: statusText
            Layout.alignment: Qt.AlignVCenter
            font.pixelSize: 10
            color: active ? "#22C55E" : "#9CA3AF"
        }
    }
}
