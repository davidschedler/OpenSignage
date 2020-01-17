import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13
import QtQuick.Window 2.13
import "./../"
import "./../popup/"

ColumnLayout {
    id: playlistVerwaltung
    spacing: 25

    ThemeManager { id: themeManagerInstance }

    ActionPopup {
        id: actionPopupInstance

    }

    RowLayout {
        Text {
            text: qsTr("Playlist-Management")
            //Layout.fillWidth: true
            font.pixelSize: themeManagerInstance.fontSizeExtraBig
        }
        Image {
            id: createPlaylist
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            Layout.preferredHeight: 25
            Layout.preferredWidth: 25
            source: "qrc:/icons/add.plus.png"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    myEngine.setPlaylistIdx(0)
                    myEngine.createClientModels("FREE");
                    myEngine.createClientModels("USED");
                    actionPopupInstance.tryToOpen("playlist")
                    actionPopupInstance.groupNameModel = myEngine.getGroupNames()
                    myEngine.setGroupNameComboIdx(0)

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
                ListElement { text: "GroupName" }
                ListElement { text: "Orientation" }
                ListElement { text: "Planed On" }
                ListElement { text: "Count" }
            }
            onCurrentIndexChanged: myEngine.sortAlgorithm(3,sortTypeCombo.currentIndex,sortOrderCombo.currentIndex)
        }
        ComboBox {
            id: sortOrderCombo
            model: ListModel {
                id: model2
                ListElement { text: "Aufsteigend" }
                ListElement { text: "Absteigend" }
            }
            onCurrentIndexChanged: myEngine.sortAlgorithm(3,sortTypeCombo.currentIndex,sortOrderCombo.currentIndex)
        }
    }

    Rectangle {
        Layout.preferredHeight: 2
        Layout.fillWidth: true
        color: themeManagerInstance.background
    }

    ListView {
        id: playlistListView
        Layout.fillWidth: true
        Layout.fillHeight: true
        currentIndex: myEngine.playlistIdx
        clip: true

        headerPositioning: ListView.OverlayHeader
        header: PlaylistenListViewHeader {}

        delegate: PlaylistenListViewDelegate {}
        ScrollBar.vertical: ScrollBar {}
        model: myEngine.playlistList
        Component.onCompleted: myEngine.sortAlgorithm(3,sortTypeCombo.currentIndex,sortOrderCombo.currentIndex)
    }
}
