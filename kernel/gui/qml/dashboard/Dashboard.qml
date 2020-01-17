import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13

import "./../"

ColumnLayout {
    id: dashboard
    spacing: 20

    ThemeManager { id: themeManagerInstance }

    Text {
        text: qsTr("Dashboard")
        font.pixelSize: themeManagerInstance.fontSizeExtraBig
    }

    Rectangle {
        Layout.preferredHeight: 2
        Layout.fillWidth: true
        color: themeManagerInstance.background
    }

    DashboardQuickPanel {
        Layout.preferredHeight: 100
        Layout.maximumHeight: 100
        Layout.fillWidth: true

        Layout.leftMargin: parent.width / 6
        Layout.rightMargin: parent.width / 6
    }

    DashboardActivityPanel {
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
}
