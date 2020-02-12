/* Copyright (C) 2020  David Schedler (https://github.com/davidschedler/)
 *
 * This file is part of OpenSigange
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

