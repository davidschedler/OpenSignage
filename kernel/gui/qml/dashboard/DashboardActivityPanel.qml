import QtQuick 2.0

Rectangle {
    color: "lightgrey"

    Text {
        id: name
        anchors.centerIn: parent
        text: qsTr("Nice to have - Aktivitäts-History")
        font.bold: themeManagerInstance.isBold
        color: themeManagerInstance.backColer
        font.pixelSize: themeManagerInstance.fontSizeExtraExtraBig
    }
}
