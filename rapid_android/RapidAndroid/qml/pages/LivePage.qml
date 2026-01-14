// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Controls
import Rapid.LiveSession

Page {
    id: livePage
    anchors.fill: parent
    title: qsTr("Live")

    LiveSession {
        id: liveSession
        anchors.fill: parent
    }
}
