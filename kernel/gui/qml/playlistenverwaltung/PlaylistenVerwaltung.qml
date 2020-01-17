import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13
import QtQuick.Window 2.13
import "./../"
import "./../popup/"
import "./../contentverwaltung/"

ColumnLayout {
    id: accountVerwaltung
    spacing: 20

    Connections {
        id: chooser
        target: myEngine
        onNextStackItemChanged: {
            if (nextStackItem == "Playlist") {
                stackview.pop("PlanningListView.qml")
            }
            if (nextStackItem == "Planning") {
                stackview.push("PlanningListView.qml")
            }
            if (nextStackItem == "Content_PUSH") {
                stackview.push("qrc:/qml/contentverwaltung/ContentVerwaltung.qml")
            }
            if (nextStackItem == "Content_POP") {
                stackview.pop("qrc:/qml/contentverwaltung/ContentVerwaltung.qml")
            }
            if (nextStackItem == "Default") {
                stackview.pop("qrc:/qml/contentverwaltung/ContentVerwaltung.qml")
                stackview.pop("PlanningListView.qml")
            }
        }
    }
   StackView {
        id: stackview
        Layout.fillHeight: true
        Layout.fillWidth: true
        initialItem: PlaylistenListView {}

        pushEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to:1
                duration: 100
            }
        }
        pushExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to:0
                duration:100
            }
        }
        popEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to:1
                duration: 100
            }
        }
        popExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to:0
                duration: 100
            }
        }
    }
}

