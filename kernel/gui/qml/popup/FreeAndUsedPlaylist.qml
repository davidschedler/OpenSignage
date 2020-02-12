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
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

ListView {
    id: freeClientsListView
    Layout.fillWidth: true
    Layout.fillHeight: true
    model: freeClientsModel
    clip: true
    header: Rectangle {
        height: 30
        width: parent.width
        color: themeManagerInstance.blue
        Text {
            text: "Free Clients"
            anchors.centerIn: parent
            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
    }

    delegate: Component {
        id: contactDelegate
        Rectangle {
            width: parent.width
            height: 30
            color: ListView.isCurrentItem ? "grey" : "lightgrey"
            Column {
                id: column
                Text {
                    text: modelData
                    verticalAlignment: Text.AlignVCenter
                    clip: true
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log(freeClientsListView.currentIndex)
                    if (freeClientsListView.currentIndex !== index)
                        freeClientsListView.currentIndex = index
                }
            }
        }
    }
}
