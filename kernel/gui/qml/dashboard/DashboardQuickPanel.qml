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

RowLayout {
    spacing: 25

    DashboardQuickItem {
        Layout.fillWidth: true
        Layout.fillHeight: true
        backgroundColor: themeManagerInstance.midground
        numberString: myEngine.calculateDashboardInfo(textString)
        textString: "Contents"
        icon: "qrc:/icons/content.png"
    }
    DashboardQuickItem {
        Layout.fillWidth: true
        Layout.fillHeight: true
        backgroundColor: themeManagerInstance.midground
        numberString: myEngine.calculateDashboardInfo(textString)
        textString: "Playlisten"
                icon: "qrc:/icons/playlist.png"
    }
    DashboardQuickItem {
        Layout.fillWidth: true
        Layout.fillHeight: true
        backgroundColor: themeManagerInstance.midground
        numberString: myEngine.calculateDashboardInfo(textString)
        textString: "Clients"
                icon: "qrc:/icons/clients.png"
    }
}
