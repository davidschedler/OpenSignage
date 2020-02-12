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

import QtQuick 2.0

Component {
    Item {
        id: navigationMenuDelegate
        width: parent.width
        height: themeManagerInstance.navigationMenuHeight

        Rectangle {
            anchors.fill: parent
            color: navigationMenuDelegate.ListView.isCurrentItem ?
                       themeManagerInstance.blueHighlight : themeManagerInstance.blue
        }
        Rectangle {
            height: parent.height / 2 ; width: parent.height / 2
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.right
            visible: navigationMenuDelegate.ListView.isCurrentItem ? 1 : 0
            color: themeManagerInstance.foreground
            rotation: 45
        }
        Image {
            id: img
            anchors.top: parent.top
            anchors.left: parent.left
            sourceSize.height: parent.height - 20
            sourceSize.width: parent.height - 20

            anchors.topMargin: themeManagerInstance.navigationMenuMargin
            anchors.leftMargin: themeManagerInstance.navigationMenuMargin
            source: model.img
        }
        Text {
            text: title
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
        MouseArea{
            id: mouseArea
            anchors.fill: parent
            onClicked:
            {
                if (myEngine.navigationMenuIdx !== index) {
                    myEngine.setNavigationMenuIdx(index)

                    if (myEngine.navigationMenuIdx == 1) { //account
                        myEngine.getDatabaseData("GETDATA","GROUPS")
                        myEngine.getDatabaseData("GETDATA","USERS")
                    }
                    if (myEngine.navigationMenuIdx == 2) { //account
                        myEngine.getDatabaseData("GETDATA","CONTENT")
                    }
                    if (myEngine.navigationMenuIdx == 3) { //account
                        myEngine.getDatabaseData("GETDATA","PLAYLISTS")
                        myEngine.getDatabaseData("GETDATA","PLANNINGS")
                        myEngine.getDatabaseData("GETDATA","HARDWARE")
                    }
                    if (myEngine.navigationMenuIdx == 4) { //hardware
                        myEngine.getDatabaseData("GETDATA","PLAYLISTS")
                        myEngine.getDatabaseData("GETDATA","HARDWARE")

                    }
                    myEngine.setNextStackItem("Default")
                    myEngine.setGroupsIdx(0)
                    myEngine.setUsersIdx(0)
                    myEngine.setContentIdx(0)
                    myEngine.setPlaylistIdx(0)
                    myEngine.setPlanningIdx(0)
                    myEngine.setHardwareIdx(0)
                }
                loader.source = source
            }
        }
    }
}
