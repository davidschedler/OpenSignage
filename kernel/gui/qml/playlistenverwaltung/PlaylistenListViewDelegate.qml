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
        id: playlistListViewDelegate

        signal pushPlanning

        width: parent.width
        height: themeManagerInstance.groupMenuHeight

        Connections {
            target: myEngine
            onRefreshModelDataCount: {
                harderinplaylistText.text = myEngine.calculateListviewModelDataCount("HardwareInPlaylists",modelData.id);
            }
        }

        ErrorPopup { id: errorPopupInstance }

        Rectangle {
            anchors.fill: parent
            color: playlistListViewDelegate.ListView.isCurrentItem ?
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

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                text: modelData.name
                Layout.fillHeight: true
                Layout.fillWidth: true
                verticalAlignment: Text.AlignVCenter

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                text: modelData.groupName
                Layout.fillHeight: true
                Layout.preferredWidth: 350
                verticalAlignment: Text.AlignVCenter

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                text: modelData.orientation
                Layout.fillHeight: true
                Layout.preferredWidth: 150
                verticalAlignment: Text.AlignVCenter

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                text: myEngine.calculateListviewModelDataCount("PlanningsInPlaylists",modelData.id);
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillHeight: true
                Layout.preferredWidth: 100
                Layout.minimumWidth: 100
                Layout.maximumWidth: 100
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                clip: true

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                id: harderinplaylistText
                text: myEngine.calculateListviewModelDataCount("HardwareInPlaylists",modelData.id);
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillHeight: true
                Layout.preferredWidth: 100
                Layout.minimumWidth: 100
                Layout.maximumWidth: 100
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
                if (myEngine.playlistIdx !== index) {
                    myEngine.setPlaylistIdx(index)
                }
                if (mouse.button === Qt.RightButton) {
                    contextMenu.popup()
                }
            }
            onDoubleClicked: {
                if (myEngine.playlistIdx !== index) {
                    myEngine.setPlaylistIdx(index)
                }
                myEngine.setNextStackItem("Planning");
            }

            Menu {
                id: contextMenu
                MenuItem {
                    text: "Edit"
                    onClicked: {
                        myEngine.createClientModels("FREE");
                        myEngine.createClientModels("USED");
                        actionPopupInstance.groupNameModel = myEngine.getGroupNames()
                        myEngine.setGroupNameComboIdx(myEngine.getGroupNameIdxByGroupName(modelData.groupName))
                        actionPopupInstance.fillPlaylist(modelData.id,modelData.name,modelData.orientation,modelData.groupName)
                        actionPopupInstance.tryToOpen("playlist")
                        actionPopupInstance.refreshClients(0)
                    }
                }
                MenuItem {
                    text: "Delete"
                    onClicked: {
                        var canDelete =  myEngine.deletePlaylist()
                        if (!canDelete)
                            errorPopupInstance.tryToOpen("Der Playliste sind noch Plannungen zugeordnet!")

                    }
                }
            }
        }
    }
}

