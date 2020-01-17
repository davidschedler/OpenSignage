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

    ActionPopup {
        id: actionPopupInstance

    }
    ErrorPopup {
        id: errorPopupInstance
    }

    RowLayout {
        Text {
            text: qsTr("Hardware-Management")
            //Layout.fillWidth: true
            font.pixelSize: themeManagerInstance.fontSizeExtraBig
        }
        Image {
            id: addHardware
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            Layout.preferredHeight: 25
            Layout.preferredWidth: 25
            source: "qrc:/icons/add.plus.png"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (myEngine.currentUserGroup === "CMS-Administrator" || myEngine.currentUserGroup === "Group-Administrator") {
                        actionPopupInstance.tryToOpen("hardware")
                        actionPopupInstance.groupNameModel = myEngine.getGroupNames()
                        myEngine.setGroupNameComboIdx(-1)
                        actionPopupInstance._comboIDX = -1
                    }
                    else
                        errorPopupInstance.tryToOpen("You are not allowed to create new Clients.")

                }
            }
        }
        Item { Layout.fillWidth: true }
        ComboBox {
            id: sortTypeCombo
            model: ListModel {
                id: model
                ListElement { text: "MAC" }
                ListElement { text: "Location" }
                ListElement { text: "Version" }
                ListElement { text: "Group" }
            }
            onCurrentIndexChanged: myEngine.sortAlgorithm(5,sortTypeCombo.currentIndex,sortOrderCombo.currentIndex)
        }
        ComboBox {
            id: sortOrderCombo
            model: ListModel {
                id: model2
                ListElement { text: "Aufsteigend" }
                ListElement { text: "Absteigend" }
            }
            onCurrentIndexChanged: myEngine.sortAlgorithm(5,sortTypeCombo.currentIndex,sortOrderCombo.currentIndex)
        }
    }

    Rectangle {
        Layout.preferredHeight: 2
        Layout.fillWidth: true
        color: themeManagerInstance.background
    }

    ListView {
        id: hardwareListView
        Layout.fillWidth: true
        Layout.fillHeight: true
        currentIndex: myEngine.hardwareIdx
        clip: true

        headerPositioning: ListView.OverlayHeader
        header: HardwareListViewHeader {}

        delegate: HardwareListViewDelegate {}
        ScrollBar.vertical: ScrollBar {}
        model: myEngine.hardwareList
        Component.onCompleted: myEngine.sortAlgorithm(5,sortTypeCombo.currentIndex,sortOrderCombo.currentIndex)

    }
}
