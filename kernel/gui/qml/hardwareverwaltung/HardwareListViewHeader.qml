import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13

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
            text: "MAC"
            Layout.fillHeight: true
            Layout.preferredWidth: 200
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
        Text {
            text: "Location"
            Layout.fillHeight: true
            Layout.preferredWidth: 200
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
        Text {
            text: "Version"
            Layout.fillHeight: true
            Layout.preferredWidth: 200
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
        Text {
            text: "Group"
            Layout.fillHeight: true
            Layout.preferredWidth: 200
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
    }
}
