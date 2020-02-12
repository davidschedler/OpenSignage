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

import "./../popup/"

Component {
    Item {
        id: groupListViewDelegate
        width: parent.width
        height: themeManagerInstance.groupMenuHeight

        Connections {
            target: myEngine
            onRefreshModelDataCount: {
                useringroups.text = myEngine.calculateListviewModelDataCount("UserInGroups",modelData.id);
                playlistingroups.text = myEngine.calculateListviewModelDataCount("PlaylistInGroups",modelData.id);
                hardwareingroups.text = myEngine.calculateListviewModelDataCount("HardwareInGroups",modelData.id);
            }
        }

        Rectangle {
            anchors.fill: parent
            color: groupListViewDelegate.ListView.isCurrentItem ?
                       "grey" : "lightgrey"
        }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 0
            Text {
                text: modelData.id
                Layout.fillHeight: true
                Layout.preferredWidth: 50
                Layout.minimumWidth: 50
                Layout.maximumWidth: 50
                verticalAlignment: Text.AlignVCenter
                clip: true

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                text: modelData.groupName
                Layout.fillHeight: true
                Layout.fillWidth: true
                verticalAlignment: Text.AlignVCenter
                clip: true

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                text: modelData.groupType
                Layout.fillHeight: true
                Layout.preferredWidth: 200
                verticalAlignment: Text.AlignVCenter
                clip: true

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                id: useringroups
                text: myEngine.calculateListviewModelDataCount("UserInGroups",modelData.id);

                Layout.fillHeight: true
                Layout.preferredWidth: 100
                Layout.minimumWidth: 100
                Layout.maximumWidth: 100
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                clip: true

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                id: playlistingroups
                text: myEngine.calculateListviewModelDataCount("PlaylistInGroups",modelData.id);

                Layout.fillHeight: true
                Layout.preferredWidth: 100
                Layout.minimumWidth: 100
                Layout.maximumWidth: 100
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                clip: true

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                id: hardwareingroups
                text: myEngine.calculateListviewModelDataCount("HardwareInGroups",modelData.id);

                Layout.fillHeight: true
                Layout.preferredWidth: 100
                Layout.minimumWidth: 100
                Layout.maximumWidth: 100
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                clip: true

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
                if (myEngine.groupsIdx !== index) {
                    myEngine.groupsIdx = index
                }
                if (mouse.button === Qt.RightButton) {
                    contextMenu.popup()
                }
            }
            Menu {
                id: contextMenu
                MenuItem {
                    text: "Edit - nice-to-have"
                }
                MenuItem {
                    text: (myEngine.currentUserGroup != "User") ? "Delete" : "Delete - keine Berechtigung"
                    onClicked: {
                        if (myEngine.currentUserGroup != "User") {
                            if (myEngine.isOwnGrp()) {
                                errorPopupInstance.tryToOpen("Die eigene Gruppe kann nicht gelöscht werden.")
                            }
                            else {
                                var canDelete =  myEngine.deleteGroup(0)
                                if (!canDelete)
                                    errorPopupInstance.tryToOpen("Der Gruppe sind noch User, Playlisten oder Hardware zugeordnet!")
                            }
                        }
                        else
                            errorPopupInstance.tryToOpen("Kann nicht gelöscht werden keine Berechtigung")
                    }
                }
            }
        }
    }
}
