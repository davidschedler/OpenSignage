import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13

import "./../popup/"

Rectangle {
    width: parent.width
    z: 3
    height: themeManagerInstance.groupMenuHeight
    color: themeManagerInstance.blue

    RowLayout {
        anchors.fill: parent
        anchors.margins: 5
        spacing: 0
        Text {
            text: "ID"
            Layout.fillHeight: true
            Layout.preferredWidth: 50
            Layout.minimumWidth: 50
            Layout.maximumWidth: 50
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
        Text {
            text: "Name"
            Layout.fillHeight: true
            Layout.fillWidth: true
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
        Text {
            text: "Type"
            Layout.fillHeight: true
            Layout.preferredWidth: 200
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
        Text {
            text: "Users"
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.preferredWidth: 100
            Layout.minimumWidth: 100
            Layout.maximumWidth: 100
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
        Text {
            text: "Playlists"
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.preferredWidth: 100
            Layout.minimumWidth: 100
            Layout.maximumWidth: 100
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
        Text {
            text: "Clients"
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.preferredWidth: 100
            Layout.minimumWidth: 100
            Layout.maximumWidth: 100
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
    }
}
