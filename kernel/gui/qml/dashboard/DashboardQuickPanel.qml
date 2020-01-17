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
