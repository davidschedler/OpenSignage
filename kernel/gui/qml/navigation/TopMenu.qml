import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Rectangle {
    id: topMenu

    height: themeManagerInstance.navigationTopMenuHeight
    color: themeManagerInstance.background

    RowLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 20

        Text {
            text: qsTr("OpenSignage - CMS")
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
            font.bold: themeManagerInstance.isBold
        }
        Text {
            text: myEngine.ident
            Layout.fillWidth: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
            font.bold: themeManagerInstance.isBold
        }
        Image {
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            fillMode: Image.Pad
            source: "qrc:/icons/settings.png"
        }
    }
}
