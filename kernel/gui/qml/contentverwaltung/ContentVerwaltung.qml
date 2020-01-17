import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13
import "./../"
import "./../popup/"

ColumnLayout {
    id: accountVerwaltung
    spacing: 20

    ThemeManager { id: themeManagerInstance }

    ActionPopup {
        id: actionPopupInstance

    }

    RowLayout {
        Image {
            id: img
            visible: myEngine.showContentToAddPlanning
            Layout.preferredHeight: 25
            Layout.preferredWidth: 25
            source: "qrc:/icons/back.arrow.png"
            MouseArea {
                anchors.fill: parent
                onClicked: myEngine.setNextStackItem("Content_POP");
            }
        }
        Text {
            text: qsTr("Content-Management")
            //Layout.fillWidth: true
            font.pixelSize: themeManagerInstance.fontSizeExtraBig
        }
        Image {
            id: addContent
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            Layout.preferredHeight: 25
            Layout.preferredWidth: 25
            source: "qrc:/icons/add.plus.png"
            MouseArea {
                anchors.fill: parent
                onClicked: addContentTip.show("Noch nicht implementiert",3000)
            }
            ToolTip { id: addContentTip }
        }
                Item { Layout.fillWidth: true }
        ComboBox {
            id: sortTypeCombo
            model: ListModel {
                id: model
                ListElement { text: "ID" }
                ListElement { text: "Name" }
                 ListElement { text: "Type" }
            }
            onCurrentIndexChanged: myEngine.sortAlgorithm(0,sortTypeCombo.currentIndex,sortOrderCombo.currentIndex)
        }
        ComboBox {
            id: sortOrderCombo
            model: ListModel {
                id: model2
                ListElement { text: "Aufsteigend" }
                ListElement { text: "Absteigend" }
            }
            onCurrentIndexChanged: myEngine.sortAlgorithm(0,sortTypeCombo.currentIndex,sortOrderCombo.currentIndex)
        }
    }
    Rectangle {
        Layout.preferredHeight: 2
        Layout.fillWidth: true
        color: themeManagerInstance.background
    }

    RowLayout {
        TextField {
            id: seachField
            Layout.fillWidth: true
            onTextEdited: {
                myEngine.searchAlgorithm(0,searchCombo.currentIndex,text)
            }
        }
        ComboBox {
            id: searchCombo
                        currentIndex: 1
            model: ListModel {
                id: search
                ListElement { text: "ID" }
                ListElement { text: "Name" }
                ListElement { text: "Type" }
            }
        }
    }

    ListView {
        id: userListView
        Layout.fillWidth: true
        Layout.fillHeight: true
        currentIndex: myEngine.contentIdx
        clip: true

        headerPositioning: ListView.OverlayHeader
        header: ContentListViewHeader {}

        delegate: ContentListViewDelegate {}
        ScrollBar.vertical: ScrollBar {}
        model: myEngine.contentList
        Component.onCompleted: myEngine.searchAlgorithm(0,searchCombo.currentIndex,"")
    }

}
