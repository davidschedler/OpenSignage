import QtQuick 2.0

ListModel {
    id: myModel

    ListElement { title: "Dashboard"; source: "qrc:/qml/dashboard/Dashboard.qml"; img: "qrc:/icons/home.png"  }
    ListElement { title: "Account-Verwaltung"; source: "qrc:/qml/accountverwaltung/AccountVerwaltung.qml" ; img: "qrc:/icons/account.png" }
    ListElement { title: "Content-Verwaltung"; source: "qrc:/qml/contentverwaltung/ContentVerwaltung.qml" ; img: "qrc:/icons/content.png" }
    ListElement { title: "Playlist-Verwaltung"; source: "qrc:/qml/playlistenverwaltung/PlaylistenVerwaltung.qml"; img: "qrc:/icons/playlist.png" }
    ListElement { title: "Clients-Verwaltung"; source: "qrc:/qml/hardwareverwaltung/HardwareVerwaltung.qml"; img: "qrc:/icons/clients.png" }
}
