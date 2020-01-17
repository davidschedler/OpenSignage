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
