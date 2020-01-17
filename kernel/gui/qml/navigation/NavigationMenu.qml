import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13

Rectangle {
    id: wrapper
    color: themeManagerInstance.blue
    ListView {
        id: navigationMenu
        anchors.fill: parent
        interactive: false
        currentIndex: myEngine.navigationMenuIdx

        delegate: NavigationMenuDelegate {}
        model: NavigationMenuModel {}
    }
}
