import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13
import QtQuick.Window 2.13

Popup {
    id: popup
    padding: 75

    parent: Overlay.overlay

    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)

    property string errorMsg: ""
    property bool visibleButton: true

    function tryToOpen(msg) {
        if (popup.opened)
            popup.close()
        popup.open()
        errorMsg = msg
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 25

        Text {
            id: errorMessage
            text: errorMsg
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true

            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.backColer
            font.pointSize: themeManagerInstance.fontSizeDefault

        }

        Button {
            id: okButton
            text: "Ok"
            visible: visibleButton
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            onClicked: popup.close()
        }
    }
}

