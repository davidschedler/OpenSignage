import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "qml/"
import "qml/navigation"
import "qml/popup"

Window {
    id: openSignage
    visible: true
    width: 1600
    height: 960
    title: qsTr("OpenSignage - CMS")

    ErrorPopup {
        id: error
        errorMsg: "Websocket kann keine Verbindung herstellen!"
        visibleButton: false
        modal: true
        closePolicy: Popup.NoAutoClose
    }

    Connections {
        target: myEngine
        onIsWebsocketConnectedChanged: { (isWebsocketConnected) ? error.close() : error.open()
        }
    }

    ThemeManager { id: themeManagerInstance }

    Rectangle {
        anchors.fill: parent
        color: themeManagerInstance.foreground
    }

    Rectangle {
        id: wrapper
        color: themeManagerInstance.foreground
        anchors.fill: parent
        visible: !myEngine.isAuthenticated
        LoginPage {
            id: loginPage
            height: 400
            width: 500
            anchors.centerIn: parent

        }
    }

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        visible: myEngine.isAuthenticated
        spacing: 0

        TopMenu {
            id: menuBarInstance
            Layout.fillWidth: true
            Layout.preferredHeight: 50
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            NavigationMenu {
                id: navigationMenuInstance
                Layout.preferredWidth: 300
                Layout.fillHeight: true
            }
            Loader {
                id: loader
                source: "qrc:/qml/dashboard/Dashboard.qml"
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: 25
            }
        }
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
