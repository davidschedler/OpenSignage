/* Copyright (C) 2020  David Schedler (https://github.com/davidschedler/)
 *
 * This file is part of OpenSigange
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13
import QtQuick.Window 2.13

Popup {
    id: popup
    padding: 75

    parent: Overlay.overlay

    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)

    property string errorMsg: ""
    property bool visibleButton: true

    function tryToOpen(msg) {
        if (popup.opened)
            popup.close()
        popup.open()
        errorMsg = msg
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 25

        Text {
            id: errorMessage
            text: errorMsg
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.backColer
            font.pointSize: themeManagerInstance.fontSizeDefault

        }

        Button {
            id: okButton
            text: "Ok"
            visible: visibleButton
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            onClicked: popup.close()
        }
    }
}

