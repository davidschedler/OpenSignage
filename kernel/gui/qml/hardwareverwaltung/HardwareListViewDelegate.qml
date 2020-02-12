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

import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13

Component {
    Item {
        id: hardwareListViewDelegate
        width: parent.width
        height: themeManagerInstance.groupMenuHeight

        Rectangle {
            anchors.fill: parent
            color: hardwareListViewDelegate.ListView.isCurrentItem ?
                       "grey" : "lightgrey"
        }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 0
            Text {
                //id: mac
                text: modelData.mac
                Layout.fillHeight: true
                Layout.preferredWidth: 200
                verticalAlignment: Text.AlignVCenter

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                // id: location
                text: modelData.location
                Layout.fillHeight: true
                Layout.preferredWidth: 200
                verticalAlignment: Text.AlignVCenter

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                // id: version
                text: modelData.version
                Layout.fillHeight: true
                Layout.preferredWidth: 200
                verticalAlignment: Text.AlignVCenter

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                // id: group
                text: modelData.groupName
                Layout.fillHeight: true
                Layout.preferredWidth: 200
                verticalAlignment: Text.AlignVCenter

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
        }
        MouseArea{
            id: mouseArea
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked:
            {
                if (myEngine.hardwareIdx !== index) {
                    myEngine.setHardwareIdx(index)
                }
                if (mouse.button === Qt.RightButton) {
                    contextMenu.popup()
                }
            }
            Menu {
                id: contextMenu
                MenuItem {
                    text: (myEngine.currentUserGroup != "User") ? "Edit" : "Edit - keine Berechtigung"
                    onClicked: {
                        if (myEngine.currentUserGroup != "User") {
                            actionPopupInstance.groupNameModel = myEngine.getGroupNames()
                            actionPopupInstance.fillHardware(modelData.mac,modelData.location,modelData.version,modelData.groupName,myEngine.getGroupNameIdxByGroupName(modelData.groupName))
                            actionPopupInstance.tryToOpen("hardware")
                        }
                        else
                            errorPopupInstance.tryToOpen("Kann nicht editiert werden keine Berechtigung")
                    }
                }
                MenuItem {
                    text: (myEngine.currentUserGroup != "User") ? "Delete" : "Delete - keine Berechtigung"
                    onClicked:
                        if (myEngine.isHardwareInUse()) {
                            errorPopupInstance.tryToOpen("Hardware ist noch in einer Playliste eingehängt.")
                        }
                        else {
                            if (myEngine.currentUserGroup != "User")
                                myEngine.deleteHardware()
                            else
                                errorPopupInstance.tryToOpen("Kann nicht gelöscht werden keine Berechtigung")
                        }
                }
            }
        }
    }
}
