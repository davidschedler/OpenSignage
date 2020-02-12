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

import "./../popup/"

Component {
    Item {
        id: contentListViewDelegate
        width: parent.width
        height: themeManagerInstance.groupMenuHeight

        ListViewMenuPopup { id: listviewMenuPopupInstance }

        Rectangle {
            anchors.fill: parent
            color: contentListViewDelegate.ListView.isCurrentItem ?
                       "grey" : "lightgrey"
        }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 0
            Text {
                text: modelData.id
                Layout.fillHeight: true
                Layout.preferredWidth: 30
                Layout.minimumWidth: 30
                Layout.maximumWidth: 30
                verticalAlignment: Text.AlignVCenter

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                text: modelData.name
                Layout.fillHeight: true
                Layout.preferredWidth: 450
                Layout.minimumWidth: 450
                Layout.maximumWidth: 450
                verticalAlignment: Text.AlignVCenter
                clip: true

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                text: modelData.reso
                Layout.fillHeight: true
                Layout.preferredWidth: 120
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                clip: true

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                text: modelData.size
                Layout.fillHeight: true
                Layout.preferredWidth: 120
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                clip: true

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                text: modelData.time
                Layout.fillHeight: true
                Layout.preferredWidth: 120
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                clip: true

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                text: modelData.fileType
                Layout.fillHeight: true
                Layout.preferredWidth: 50
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
                if (mouse.button === Qt.LeftButton) {
                    if (myEngine.contentIdx !== index) {
                        myEngine.contentIdx = index
                    }
                }
                if (mouse.button === Qt.RightButton) {
                    contextMenu.popup()
                }
            }
            onDoubleClicked: {} //delete aktuell nicht gewünscht

            Menu {
                id: contextMenu
                MenuItem {
                    text: "Add"
                    onClicked: {
                        actionPopupInstance.tryToOpen("planning")
                        actionPopupInstance.fillPlanning("Content",modelData.name)
                        actionPopupInstance.groupNameModel = myEngine.getGroupNames()
                    }
                }
            }
        }
    }
}

