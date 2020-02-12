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

ListModel {
    id: myModel

    ListElement { title: "Dashboard"; source: "qrc:/qml/dashboard/Dashboard.qml"; img: "qrc:/icons/home.png"  }
    ListElement { title: "Account-Verwaltung"; source: "qrc:/qml/accountverwaltung/AccountVerwaltung.qml" ; img: "qrc:/icons/account.png" }
    ListElement { title: "Content-Verwaltung"; source: "qrc:/qml/contentverwaltung/ContentVerwaltung.qml" ; img: "qrc:/icons/content.png" }
    ListElement { title: "Playlist-Verwaltung"; source: "qrc:/qml/playlistenverwaltung/PlaylistenVerwaltung.qml"; img: "qrc:/icons/playlist.png" }
    ListElement { title: "Clients-Verwaltung"; source: "qrc:/qml/hardwareverwaltung/HardwareVerwaltung.qml"; img: "qrc:/icons/clients.png" }
}
