import QtQuick 2.0
import QtQuick.Controls 2.5

Button {
    id: mybutton
    text: myText
    property bool isActive: false
    property bool doReset: false
    property string myText: ""
    onIsActiveChanged: {
            background.color = (isActive === true) ? themeManagerInstance.blue : "lightgrey"
            label.color = (isActive === true) ? "white" : "black"
    }

    background: Rectangle {
        id: background
        implicitWidth: 75
        implicitHeight: 40
        opacity: enabled ? 1 : 0.3
        border.color: (parent.isActive === true) ? themeManagerInstance.blue : "lightgrey"
        border.width: 1
        radius: 2
    }
    contentItem: Text {
        id: label
        text: mybutton.text
        font: mybutton.font
        opacity: enabled ? 1.0 : 0.3
        color: (parent.isActive === true) ? "white" : "black"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
}
