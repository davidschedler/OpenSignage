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
            Layout.preferredWidth: 30
            Layout.minimumWidth: 30
            Layout.maximumWidth: 30
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
        Text {
            text: "File-Name"
            Layout.fillHeight: true
            Layout.preferredWidth: 450
            Layout.minimumWidth: 450
            Layout.maximumWidth: 450
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
        Text {
            text: "Resolution"
            Layout.fillHeight: true
            Layout.preferredWidth: 120
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
        Text {
            text: "Size"
            Layout.fillHeight: true
            Layout.preferredWidth: 120
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
        Text {
            text: "Time"
            Layout.fillHeight: true
            Layout.preferredWidth: 120
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
        Text {
            text: "TYPE"
            Layout.fillHeight: true
            Layout.preferredWidth: 50
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
    }
}
