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
import QtQuick.Window 2.13
import "./../"
import "./../popup/"

ColumnLayout {
    id: accountVerwaltung
    spacing: 20

    ThemeManager { id: themeManagerInstance }
    ActionPopup { id: actionPopupInstance }
    ErrorPopup {id: errorPopupInstance }

    RowLayout {
        Text {
            text: qsTr("Group-Management")
            font.pixelSize: themeManagerInstance.fontSizeExtraBig
        }
        Image {
            id: createGroup
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            Layout.preferredHeight: 25
            Layout.preferredWidth: 25
            source: "qrc:/icons/add.plus.png"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (myEngine.currentUserGroup !== "User") {
                        actionPopupInstance.groupTypeModel = myEngine.defaultGroupTypes
                        actionPopupInstance.tryToOpen("group")
                        myEngine.setGroupTypeComboIdx(-1)
                    }
                    else
                        errorPopupInstance.tryToOpen("You are not allowed to create a new Group.")
                }
            }
        }
        Item { Layout.fillWidth: true }
        ComboBox {
            id: sortTypeCombo
            model: ListModel {
                id: model
                ListElement { text: "ID" }
                ListElement { text: "Name" }
                ListElement { text: "Type" }
                ListElement { text: "Users" }
            }
            onCurrentIndexChanged: myEngine.sortAlgorithm(1,sortTypeCombo.currentIndex,sortOrderCombo.currentIndex)
        }
        ComboBox {
            id: sortOrderCombo
            model: ListModel {
                id: model2
                ListElement { text: "Aufsteigend" }
                ListElement { text: "Absteigend" }
            }
            onCurrentIndexChanged: myEngine.sortAlgorithm(1,sortTypeCombo.currentIndex,sortOrderCombo.currentIndex)
        }
    }

    Rectangle {
        Layout.preferredHeight: 2
        Layout.fillWidth: true
        color: themeManagerInstance.background
    }

    ListView {
        id: groupListView
        Layout.fillWidth: true
        Layout.preferredHeight: 4*themeManagerInstance.groupMenuHeight
        currentIndex: myEngine.groupsIdx
        clip: true

        headerPositioning: ListView.OverlayHeader
        header: GroupListViewHeader {}

        delegate: GroupListViewDelegate {}
        ScrollBar.vertical: ScrollBar { policy: ScrollBar.AlwaysOn }
        model:  myEngine.groupsList
        Component.onCompleted: myEngine.sortAlgorithm(1,sortTypeCombo.currentIndex,sortOrderCombo.currentIndex)
    }

    RowLayout {
        Text {
            text: qsTr("User-Management")
            font.pixelSize: themeManagerInstance.fontSizeExtraBig
        }
        Image {
            id: createUser
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            Layout.preferredHeight: 25
            Layout.preferredWidth: 25
            source: "qrc:/icons/add.plus.png"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (myEngine.currentUserGroup === "User")
                        errorPopupInstance.tryToOpen("You are not allowed to create a new User.")
                    else {
                        actionPopupInstance.groupNameModel = myEngine.getGroupNames()
                        actionPopupInstance.tryToOpen("user")
                        myEngine.setGroupNameComboIdx(-1)
                    }
                }
            }
        }
        Item { Layout.fillWidth: true }
        ComboBox {
            id: sortTypeCombo2
            model: ListModel {
                id: model3
                ListElement { text: "ID" }
                ListElement { text: "FirstName" }
                ListElement { text: "LastName" }
                ListElement { text: "eMail" }
            }
            onCurrentIndexChanged: myEngine.sortAlgorithm(2,sortTypeCombo2.currentIndex,sortOrderCombo2.currentIndex)
        }
        ComboBox {
            id: sortOrderCombo2
            model: ListModel {
                id: model4
                ListElement { text: "Aufsteigend" }
                ListElement { text: "Absteigend" }
            }
            onCurrentIndexChanged: myEngine.sortAlgorithm(2,sortTypeCombo2.currentIndex,sortOrderCombo2.currentIndex)
        }

    }

    Rectangle {
        Layout.preferredHeight: 2
        Layout.fillWidth: true
        color: themeManagerInstance.background
    }

    ListView {
        id: userListView
        Layout.fillWidth: true
        Layout.fillHeight: true
        currentIndex: myEngine.usersIdx
        clip: true

        headerPositioning: ListView.OverlayHeader
        header: UserListViewHeader {}

        delegate: UserListViewDelegate {}
        ScrollBar.vertical: ScrollBar {}
        model: myEngine.groupsList[myEngine.groupsIdx].usersList
        Component.onCompleted: myEngine.sortAlgorithm(2,sortTypeCombo2.currentIndex,sortOrderCombo2.currentIndex)
    }
}


