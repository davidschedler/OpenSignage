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
