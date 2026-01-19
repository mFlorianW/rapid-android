// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Controls

Control {
    id: listDelegate

    property alias borderColor: backgroundRect.border.color

    background: Rectangle {
        id: backgroundRect
        color: "#ffffff"
        border.color: "#E5E7EB"
        border.width: 1
        radius: 8
    }
}
