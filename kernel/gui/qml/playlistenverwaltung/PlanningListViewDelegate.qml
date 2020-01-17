import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "./../popup/"

Component {
    Item {
        id: planningListViewDelegate
        width: parent.width
        height: themeManagerInstance.groupMenuHeight

        Rectangle {
            anchors.fill: parent
            color: planningListViewDelegate.ListView.isCurrentItem ?
                       "grey" : "lightgrey"
        }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 0
            RowLayout {
                Layout.preferredWidth: 100
                Layout.minimumWidth: 100
                Layout.maximumWidth: 100
                spacing: 0

                Image {
                    id: up
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.preferredHeight: 25
                    Layout.preferredWidth: 25
                    source: "qrc:/icons/arrow.toup.png"

                }
                Text {
                    text: modelData.id_order
                    Layout.fillHeight: true
                    verticalAlignment: Text.AlignVCenter
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    font.bold: themeManagerInstance.isBold
                    color: themeManagerInstance.backColer
                    font.pointSize: themeManagerInstance.fontSizeDefault
                }

                Image {
                    id: down
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.preferredHeight: 25
                    Layout.preferredWidth: 25
                    source: "qrc:/icons/arrow.todown.png"
                }
            }
            Item {
                Layout.preferredWidth: 25
                Layout.minimumWidth: 25
                Layout.maximumWidth: 25
            }
            Text {
                text: modelData.contentName
                Layout.fillHeight: true
                Layout.fillWidth: true
                verticalAlignment: Text.AlignVCenter

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                text: modelData.validPeriod
                Layout.fillHeight: true
                Layout.preferredWidth: 230
                verticalAlignment: Text.AlignVCenter

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                text: modelData.weekdays
                Layout.fillHeight: true
                Layout.preferredWidth: 200
                verticalAlignment: Text.AlignVCenter

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                text: modelData.validTime
                Layout.fillHeight: true
                Layout.preferredWidth: 200
                verticalAlignment: Text.AlignVCenter

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            Text {
                text: modelData.type
                Layout.fillHeight: true
                Layout.preferredWidth: 100
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
                if (myEngine.planningIdx !== index) {
                    myEngine.setPlanningIdx(index)
                }
                if (mouse.button === Qt.RightButton) {
                    contextMenu.popup()
                }
            }
            Menu {
                id: contextMenu
                MenuItem {
                    text: "Edit"
                    onClicked: {
                        actionPopupInstance.groupNameModel = myEngine.getGroupNames()
                        myEngine.setGroupNameComboIdx(myEngine.getGroupNameIdxByGroupName(modelData.groupName))
                        //actionPopupInstance._weekdays = ""
                        actionPopupInstance.tryToOpen("planning")
                        actionPopupInstance.fillPlanning("Update",modelData.contentName,modelData.validPeriod,modelData.weekdays,modelData.validTime,modelData.type,modelData.id,modelData.id_order)
                    }
                }
                MenuItem {
                    text: "Delete"
                    onClicked: {
                        myEngine.deletePlanning()
                    }
                }
            }
        }
        MouseArea {
            x: up.x
            y: up.y
            width: up.width
            height: up.height
            onClicked: {
                if (myEngine.planningIdx !== index) {
                    myEngine.setPlanningIdx(index)
                }
                myEngine.changePlanningPosition("UP");
            }
        }
        MouseArea {
            x: down.x
            y: down.y
            width: down.width
            height: down.height
            onClicked: {
                if (myEngine.planningIdx !== index) {
                    myEngine.setPlanningIdx(index)
                }
                myEngine.changePlanningPosition("DOWN");
            }
        }
    }
}
