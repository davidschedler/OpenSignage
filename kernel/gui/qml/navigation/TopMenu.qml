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

import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Rectangle {
    id: topMenu

    height: themeManagerInstance.navigationTopMenuHeight
    color: themeManagerInstance.background

    RowLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 20

        Text {
            text: qsTr("OpenSignage - CMS")
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
            font.bold: themeManagerInstance.isBold
        }
        Text {
            text: myEngine.ident
            Layout.fillWidth: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            color: themeManagerInstance.fontColer
            font.pointSize: themeManagerInstance.fontSizeDefault
            font.bold: themeManagerInstance.isBold
        }
        Image {
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            fillMode: Image.Pad
            source: "qrc:/icons/settings.png"
        }
    }
}
