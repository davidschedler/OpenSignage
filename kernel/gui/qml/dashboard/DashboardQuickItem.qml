import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13

Rectangle {
    id: dashboardQuickItem

    property string backgroundColor: "green"
    property string icon: ""
    property string numberString: ""
    property string textString: ""

    color: themeManagerInstance.blue
    radius: 5

    RowLayout {
        anchors.fill: parent
        anchors.margins: 10

        Image {
            Layout.preferredHeight: 64
            Layout.preferredWidth: 64
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            sourceSize.width: 64
            sourceSize.height: 64
            fillMode: Image.Stretch
            source: icon
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Text {
                Layout.fillWidth: true
                Layout.fillHeight: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                text: numberString
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.fontColer
                font.pixelSize: themeManagerInstance.fontSizeExtraExtraBig
            }
            Text {
                Layout.fillWidth: true
                Layout.fillHeight: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                text: textString
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.fontColer
                font.pixelSize: themeManagerInstance.fontSizeExtraBig
            }
        }
    }

    MouseArea {
        onClicked: {

        }
    }
}
