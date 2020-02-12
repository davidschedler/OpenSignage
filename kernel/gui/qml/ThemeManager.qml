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

Item {
    id: themeManager

    // Color for Layers
    property string background: "#323232"
    property string midground: "#eaeaea"
    property string foreground: "#ffffff"

    // Color for Highlights
    property string blue: "#2a3f54"
    property string blueHighlight: "#354a5f"

    // Coler & Size for Fonts
    property string fontColer: "#ffffff"
    property string backColer: "#000000"
    property int fontSizeDefault: 12
    property int fontSizeMiddle: 14
    property int fontSizeBig: 16
    property int fontSizeExtraBig: 24
    property int fontSizeExtraExtraBig: 30
    property bool isBold: false

    // Default colors
    property string black: "black"
    property string white: "white"
    property string green: "#1ed760"

    property int navigationMenuHeight: 50
    property int groupMenuHeight: 50
    property int navigationMenuMargin: 10
    property int navigationTopMenuHeight: 75
}
