// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Controls
import Rapid.Session

Page {
    id: sessionsPage
    anchors.fill: parent
    title: qsTr("Sessions")

    Session {
        id: session
        anchors.fill: parent
    }
}
