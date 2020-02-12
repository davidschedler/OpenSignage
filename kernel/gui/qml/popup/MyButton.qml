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
