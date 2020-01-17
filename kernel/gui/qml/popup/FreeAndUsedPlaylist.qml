import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

ListView {
    id: freeClientsListView
    Layout.fillWidth: true
    Layout.fillHeight: true
    model: freeClientsModel
    clip: true
    header: Rectangle {
        height: 30
        width: parent.width
        color: themeManagerInstance.blue
        Text {
            text: "Free Clients"
            anchors.centerIn: parent
            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
    }

    delegate: Component {
        id: contactDelegate
        Rectangle {
            width: parent.width
            height: 30
            color: ListView.isCurrentItem ? "grey" : "lightgrey"
            Column {
                id: column
                Text {
                    text: modelData
                    verticalAlignment: Text.AlignVCenter
                    clip: true
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log(freeClientsListView.currentIndex)
                    if (freeClientsListView.currentIndex !== index)
                        freeClientsListView.currentIndex = index
                }
            }
        }
    }
}
