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

Rectangle {
    id: dashboardQuickItem

    property string backgroundColor: "green"
    property string icon: ""
    property string numberString: ""
    property string textString: ""

    color: themeManagerInstance.blue
    radius: 5

    RowLayout {
        anchors.fill: parent
        anchors.margins: 10

        Image {
            Layout.preferredHeight: 64
            Layout.preferredWidth: 64
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            sourceSize.width: 64
            sourceSize.height: 64
            fillMode: Image.Stretch
            source: icon
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Text {
                Layout.fillWidth: true
                Layout.fillHeight: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                text: numberString
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.fontColer
                font.pixelSize: themeManagerInstance.fontSizeExtraExtraBig
            }
            Text {
                Layout.fillWidth: true
                Layout.fillHeight: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                text: textString
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.fontColer
                font.pixelSize: themeManagerInstance.fontSizeExtraBig
            }
        }
    }

    MouseArea {
        onClicked: {

        }
    }
}
