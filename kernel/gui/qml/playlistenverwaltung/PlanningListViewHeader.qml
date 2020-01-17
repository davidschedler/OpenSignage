import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

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
            text: "Position"
            horizontalAlignment: Text.AlignHCenter
            Layout.fillHeight: true
            Layout.preferredWidth: 100
            Layout.minimumWidth: 100
            Layout.maximumWidth: 100
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
        Item {
            Layout.preferredWidth: 25
            Layout.minimumWidth: 25
            Layout.maximumWidth: 25
        }
        Text {
            text: "Content-Name"
            Layout.fillHeight: true
            Layout.fillWidth: true
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
        Text {
            text: "Valid-Period"
            Layout.fillHeight: true
            Layout.preferredWidth: 230
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
        Text {
            text: "Weekdays"
            Layout.fillHeight: true
            Layout.preferredWidth: 200
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
        Text {
            text: "Valid-Time"
            Layout.fillHeight: true
            Layout.preferredWidth: 200
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
        Text {
            text: "Type"
            Layout.fillHeight: true
            Layout.preferredWidth: 100
            verticalAlignment: Text.AlignVCenter

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
        }
    }
}
