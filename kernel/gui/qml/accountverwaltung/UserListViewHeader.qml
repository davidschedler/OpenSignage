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
            text: "First-Name"
            Layout.fillHeight: true
                Layout.preferredWidth: 350
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
        Text {
            text: "Last-Name"
            Layout.fillHeight: true
                Layout.preferredWidth: 350
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
        Text {
            text: "e-Mail"
            Layout.fillHeight: true
                Layout.fillWidth: true
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
    }
}
