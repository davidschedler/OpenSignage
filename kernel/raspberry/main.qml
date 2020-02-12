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

import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.4


Window {
    id: openSignageMainWindow
    visible: true
    width: Screen.width
    height: Screen.height
    flags: Qt.FramelessWindowHint
    title: qsTr("OpenSignage - Raspberry")

    Connections {
        target: myManager
        onCurrentPictureChanged: {
            showCurrentPicture.source = "file://" + currentPicture
        }
    }
    Connections {
        target: myManager
        onCurrentVideoChanged: {
            movieIsPlaying.text = currentVideo
        }
    }

    Image {
        id: showCurrentPicture
        anchors.fill: parent
        source: ""
    }

//    Text {
//        id: movieIsPlaying
//        x: 100
//        y: 100
//        width: 500
//        color: "#3dff16"
//        text: ""
//        font.pointSize: 22
//    }

//    Button {
//        id: button
//        z: 5
//        x: 150
//        y: 150
//        width: 100
//        height: 80
//        text: qsTr("Button")
//        onClicked: myManager.requestPlaylistFromMac();
//    }

//    Button {
//        id: button2
//        x: 159
//        y: 400
//        width: 100
//        height: 142
//        text: qsTr("Button")
//        onClicked: myManager.checkThis();
//    }
}
