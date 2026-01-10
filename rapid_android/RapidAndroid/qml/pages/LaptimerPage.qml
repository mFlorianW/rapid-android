// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Controls
import Rapid.Laptimer
import Rapid.Android

Page {
    id: laptimerPage
    anchors.fill: parent
    title: qsTr("Laptimer")

    Laptimer {
        id: laptimer
        anchors.fill: parent
        deviceManagement: GlobalContext.deviceManagement
    }
}
