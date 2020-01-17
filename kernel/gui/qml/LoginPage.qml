import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Item {
    Rectangle {
        anchors.fill: parent
        color: themeManagerInstance.midground
    }

    ColumnLayout {
        id: column
        anchors.fill: parent
        anchors.margins: 10
        spacing: 20

        Rectangle {
            id: header
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            color: themeManagerInstance.blue
            Text {
                id: headertext
                text: qsTr("Login")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.centerIn: parent
                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.fontColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
        }

        GridLayout {
            id: grid
            Layout.fillWidth: true
            Layout.fillHeight: true
            columns: 2

            Text {
                id: loginName
                text: qsTr("Login: ")
                verticalAlignment: Text.AlignVCenter
                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            TextField {
                id: loginNameEdit
                text: myEngine.ident
                Layout.fillWidth: true
                placeholderText: "max.mustermann@mail.de"
                onTextChanged: myEngine.setIdent(loginNameEdit.text)
            }
            Text {
                id: loginPwd
                text: qsTr("Password: ")
                verticalAlignment: Text.AlignVCenter
                font.bold: themeManagerInstance.isBold
                color: themeManagerInstance.backColer
                font.pointSize: themeManagerInstance.fontSizeDefault
            }
            TextField {
                id: loginPwdEdit
                text: myEngine.password
                Layout.fillWidth: true
                onTextChanged: myEngine.setPassword(loginPwdEdit.text)
                echoMode: TextInput.Password
            }
        }

        Text {
            id: loginMessage
            Layout.preferredHeight: 30
            Layout.fillWidth: true
            verticalAlignment: Text.AlignVCenter
            font.bold: themeManagerInstance.isBold
            color: themeManagerInstance.backColer
            font.pointSize: themeManagerInstance.fontSizeDefault
            text: myEngine.loginMessage
            horizontalAlignment: Text.AlignHCenter
        }

        Button {
            id: loginButton
            text: "Login"
            Layout.preferredHeight: 50
            Layout.preferredWidth: 200
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            onClicked: {
                if (loginNameEdit.text !== "" || loginPwdEdit.text !== "") {
                    var data = "ident=" + myEngine.ident + "|" + "password=" + myEngine.password
                    myEngine.getDatabaseData("GETDATA","AUTH",data)
                }

            }
        }
//        ColumnLayout {
//            Button {
//                text: "CMS-Administrator"
//                onClicked: {

//                        myEngine.setIdent("admin@gadmin.de")
//                        myEngine.setPassword("admin")
//                        var data = "ident=admin@gadmin.de" + "|" + "password=admin"
//                        myEngine.getDatabaseData("GETDATA","AUTH",data)

//                }
//            }
//            Button {
//                text: "Group-Administrator (Campus am Brunnenlech)"
//                onClicked: {

//                        myEngine.setIdent("max.baumgartner@hs-augsburg.de")
//                        myEngine.setPassword("passwort")
//                        var data = "ident=max.baumgartner@hs-augsburg.de" + "|" + "password=passwort"
//                        myEngine.getDatabaseData("GETDATA","AUTH",data)

//                }
//            }
//            Button {
//                text: "Group-Administrator (Campus am Brunnenlech)"
//                onClicked: {

//                        myEngine.setIdent("stefan.mayer@hs-augsburg.de")
//                        myEngine.setPassword("passwort")
//                        var data = "ident=stefan.mayer@hs-augsburg.de" + "|" + "password=passwort"
//                        myEngine.getDatabaseData("GETDATA","AUTH",data)

//                }
//            }
//            Button {
//                text: "Group-Administrator (Am Rotem Tor)"
//                onClicked: {

//                        myEngine.setIdent("sonja.krebs@hs-augsburg.de")
//                        myEngine.setPassword("passwort")
//                        var data = "ident=sonja.krebs@hs-augsburg.de" + "|" + "password=passwort"
//                        myEngine.getDatabaseData("GETDATA","AUTH",data)

//                }
//            }
//            Button {
//                text: "User- Content-Planer (Brunnenlech)"
//                onClicked: {

//                        myEngine.setIdent("mario.müller@hs-augsburg.de")
//                        myEngine.setPassword("passwort")
//                        var data = "ident=mario.müller@hs-augsburg.de" + "|" + "password=passwort"
//                        myEngine.getDatabaseData("GETDATA","AUTH",data)

//                }
//            }
//            Button {
//                text: "User- Content-Planer (Am Rotem Tor)"
//                onClicked: {

//                        myEngine.setIdent("karin.schmied@hs-augsburg.de")
//                        myEngine.setPassword("passwort")
//                        var data = "ident=karin.schmied@hs-augsburg.de" + "|" + "password=passwort"
//                        myEngine.getDatabaseData("GETDATA","AUTH",data)

//                }
//            }
//            Button {
//                text: "User- Content-Planer (Am Rotem Tor)"
//                onClicked: {

//                        myEngine.setIdent("mario.thil@hs-augsburg.de")
//                        myEngine.setPassword("passwort")
//                        var data = "ident=mario.thil@hs-augsburg.de" + "|" + "password=passwort"
//                        myEngine.getDatabaseData("GETDATA","AUTH",data)

//                }
//            }
//        }
    }
}
