import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13

import "./../popup/"

Component {
    Item {
        id: usersListViewDelegate
        width: parent.width
        height: themeManagerInstance.groupMenuHeight

        Rectangle {
            anchors.fill: parent
            color: usersListViewDelegate.ListView.isCurrentItem ?
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
                text: modelData.firstName
                Layout.fillHeight: true
                Layout.preferredWidth: 350
                verticalAlignment: Text.AlignVCenter
                clip: true

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                text: modelData.lastName
                Layout.fillHeight: true
                Layout.preferredWidth: 350
                verticalAlignment: Text.AlignVCenter
                clip: true

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                text: modelData.eMail
                Layout.fillHeight: true
                Layout.fillWidth: true
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
                if (myEngine.usersIdx !== index) {
                    myEngine.setUsersIdx(index)
                }
                if (mouse.button === Qt.RightButton) {
                    contextMenu.popup()
                }
            }
            Menu {
                id: contextMenu
                MenuItem {
                    text: (myEngine.currentUserGroup != "User") ? "Edit" : (myEngine.ident === modelData.eMail) ? "Edit" : "Edit - keine Berechtigung"
                    onClicked: {
                        if (myEngine.currentUserGroup == "CMS-Administrator" || myEngine.currentUserGroup == "Group-Administrator" || myEngine.ident === modelData.eMail) {
                            actionPopupInstance.groupNameModel = myEngine.getGroupNames()
                            actionPopupInstance.groupTypeModel = myEngine.defaultGroupTypes
                            myEngine.setGroupNameComboIdx(myEngine.getGroupNameIdxByGroupName(modelData.groupName))
                            myEngine.setGroupTypeComboIdx(myEngine.getGroupTypeIdxByGroupType(modelData.groupType))
                            actionPopupInstance.tryToOpen("user")
                            actionPopupInstance.fillUser(modelData.id,modelData.firstName,modelData.lastName,"",modelData.groupName,modelData.eMail,modelData.groupType)

                        }
                        else
                            errorPopupInstance.tryToOpen("Kann nicht editiert werden keine Berechtigung")
                    }
                }
                MenuItem {
                    text: (myEngine.currentUserGroup != "User") ? "Delete" : "Delete - keine Berechtigung"
                    onClicked: {
                        if (myEngine.currentUserGroup != "User") {
                            myEngine.deleteUser()
                        }
                        else
                            errorPopupInstance.tryToOpen("Kann nicht gelöscht werden keine Berechtigung")
                    }
                }
            }
        }
    }
}
