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
import QtQuick.Window 2.13

Popup {
    id: popup
    padding: 10

    parent: Overlay.overlay

    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)

    property var groupNameModel
    property var groupTypeModel
    property var clientsModel
    property var freeClientsModel
    property var usedClientsModel

    onClosed: {
        _comboIDX = 0
        _popupType = ""
        loader.sourceComponent = undefined
    }


    function refreshClients(type) {
        //        myEngine.createClientModels("FREE");
        //        myEngine.createClientModels("USED");
        //        freeClientsModel = myEngine.freeClientsToPlanOn
        //        usedClientsModel = myEngine.usedClientsToPlanOn
        //        if (type === 1)
        //            clientsModel = myEngine.freeClientsToPlanOn()
        //        else
        //            clientsModel = myEngine.freeClientsToPlanOn(_planedon)
    }

    ErrorPopup {id: errorPopupInstance }

    function tryToOpen(type) {
        if (popup.opened) {
            popup.close()
        }
        if (type === "group") {
            height = 250
            width = 400
            _groupName = ""
            _gID = ""
            _comboIDX = -1
            loader.sourceComponent = group
        }
        if (type === "user") {
            height = 400
            width = 400
            _uID = ""
            _firstName = ""
            _lastname = ""
            _password = ""
            _groupName = ""
            _email = ""
            _groupType = ""
            loader.sourceComponent = user
        }
        if (type === "hardware") {
            height = 350
            width = 400
            loader.sourceComponent = hardware
        }
        if (type === "playlist") {
            height = 500
            width = 600
            loader.sourceComponent = playlist
        }
        if (type === "planning") {
            height = 500
            width = 600
            _weekdays = "XX"
            loader.sourceComponent = planning
        }
        popup.open()
        console.log(_popupType)
    }

    function fillHardware(mac,location,version,groupname,comboIDX) {
        _mac = mac
        _location = location
        _version = version
        _groupName = groupname
        _comboIDX = comboIDX
        _popupType = "Update"
    }
    function fillGroup(id,groupName,groupTypeIDx) {
        _groupName = groupName
        _gID = id
        _comboIDX = groupTypeIDx
        _popupType = "Update"
    }
    function fillUser(id,firstname,lastname,password,groupname,email,grouptype) {
        _uID = id
        _firstName = firstname
        _lastname = lastname
        _password = password
        _groupName = groupname
        _email = email
        _groupType = grouptype
        _popupType = "Update"
    }
    function fillPlaylist(id,name,orientation,group) {
        _playId = id
        _name = name
        _orientation = orientation
        _groupName = group
        //_planedon = planedon
        _popupType = "Update"
    }
    function fillPlanning(type,name,validperiod,weekdays,validtime,typePL,planningID,idOrder) {
        _playlistName = name
        _validperiod = validperiod
        _weekdays = weekdays
        _id_order = idOrder
        _validtime = validtime
        _type = type
        if (type === "Update")
            _popupType = "Update"
        if (type === "")
            _popupType = ""
        if (type === "Content")
            _popupType = "Content"

        if (typePL === "Standard")
            myEngine.setPlanningTypeComboIdx(0)
        else if (typePL === "Event")
            myEngine.setPlanningTypeComboIdx(1)
        else
            myEngine.setPlanningTypeComboIdx(-1)

        _planningID = planningID
        console.log("Beim fillen vom edit" + _popupType)
    }

    property int _id_order: -1
    property var _planningID: ""
    property int _pltypeIdx: -1
    property var _validperiod: ""
    property string _weekdays: ""
    property var _validtime: ""
    property var _type: ""

    property var _planedon
    property var _playlistName: ""

    property var _playId: ""
    property var _name: ""
    property var _orientation: ""


    property var _uID: ""
    property var _firstName: ""
    property var _lastname: ""
    property var _password: ""
    property var _email: ""
    property var _gID: ""
    property var _groupType: ""

    property var _mac: ""
    property var _location: ""
    property var _version: ""
    property var _groupName: ""

    property var _popupType: ""
    property int _comboIDX: 0

    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

    Loader {
        id: loader
        anchors.fill: parent

    }

    Component {
        id: group
        ColumnLayout {
            id: column
            spacing: 20

            Rectangle {
                id: header
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                color: themeManagerInstance.blue
                Text {
                    id: headertext
                    text: (_popupType == "") ? qsTr("Create Group") : qsTr("Update Group")
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
                    id: groupname
                    text: qsTr("Group-Name: ")
                    verticalAlignment: Text.AlignVCenter
                    font.bold: themeManagerInstance.isBold
                    color: themeManagerInstance.backColer
                    font.pointSize: themeManagerInstance.fontSizeDefault
                }
                TextField {
                    id: groupEdit
                    text: _groupName
                    Layout.fillWidth: true
                    validator: RegExpValidator { regExp: /[0-9A-Za-z_-\s\.]+/ }
                    placeholderText: "Grupenname"
                }
                Text {
                    id: grouptype
                    text: qsTr("Group-Type: ")
                    verticalAlignment: Text.AlignVCenter
                    font.bold: themeManagerInstance.isBold
                    color: themeManagerInstance.backColer
                    font.pointSize: themeManagerInstance.fontSizeDefault
                }
                ComboBox {
                    id: grouptypeCombo
                    Layout.fillWidth: true
                    model: {
                        if (myEngine.currentUserGroup !== "CMS-Administrator")
                            model: ["User"]
                        else
                        groupTypeModel
                    }
                    currentIndex: myEngine.groupTypeComboIdx
                }
            }
            RowLayout {
                id: actionrow
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredHeight: 50
                Layout.preferredWidth: 200

                Button {
                    id: createButton
                    text: (_popupType === "") ? "Create" : "Update"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    onClicked: {
                        if (groupEdit.text.match(/[0-9A-Za-z_-\s\.]+/) && grouptypeCombo.currentIndex >= 0) {
                            var data = "id_group=" + _gID + "|" + "createdby=" + myEngine.groupCreatedby() + "|" + "groupName=" + groupEdit.text + "|" + "groupTypeID=" + myEngine.getGroupTypeIdByGroupType(grouptypeCombo.currentText)
                            if (_popupType == "")
                                myEngine.createDatabaseEntry("INSERTENTRY","GROUP",data)
                            else
                                myEngine.updateDatabaseEntry("UPDATEENTRY","GROUP",data)
                            groupEdit.text = ""
                            grouptypeCombo.currentIndex = -1
                            _groupName = ""
                            _gID = ""
                            _popupType = ""
                            popup.close();
                        }
                        else
                            errorPopupInstance.tryToOpen("Bitte füllen Sie die Felder korrekt aus.")
                    }
                }
                Button {
                    id: cancelButton
                    text: "Cancel"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    onClicked: {
                        _groupName = ""
                        _popupType = ""
                        groupEdit.text = ""
                        grouptypeCombo.currentIndex = -1
                        _groupName = ""
                        _gID = ""
                        _popupType = ""
                        popup.close()
                    }
                }
            }
        }
    }

    Component {
        id: user
        ColumnLayout {
            id: column
            spacing: 20

            Rectangle {
                id: header
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                color: themeManagerInstance.blue
                Text {
                    id: headertext
                    text: (_popupType == "") ? qsTr("Create User") : qsTr("Update User")
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
                    id: firstname
                    text: qsTr("First-Name: ")
                    verticalAlignment: Text.AlignVCenter
                    font.bold: themeManagerInstance.isBold
                    color: themeManagerInstance.backColer
                    font.pointSize: themeManagerInstance.fontSizeDefault
                }
                TextField {
                    id: fistnameEdit
                    text: _firstName
                    Layout.fillWidth: true
                    readOnly: (_popupType == "Update" && myEngine.currentUserGroup == "User") ? true: false
                    validator: RegExpValidator { regExp: /[A-Za-z]+/ }
                    placeholderText: "Max"
                    ToolTip {
                        id: infoTip3
                    }
                    onFocusChanged: (focus == true && _popupType == "Update" && myEngine.currentUserGroup == "User") ? infoTip3.show("Kann nicht geändert werden.",2000) : ""
                }
                Text {
                    id: lastname
                    text: qsTr("Last-Name: ")
                    verticalAlignment: Text.AlignVCenter
                    font.bold: themeManagerInstance.isBold
                    color: themeManagerInstance.backColer
                    font.pointSize: themeManagerInstance.fontSizeDefault

                }
                TextField {
                    id: lastnameEdit
                    text: _lastname
                    Layout.fillWidth: true
                    readOnly: (_popupType == "Update" && myEngine.currentUserGroup == "User") ? true: false
                    validator: RegExpValidator { regExp: /[A-Za-z]+/ }
                    placeholderText: "Mustermann"
                    ToolTip {
                        id: infoTip2
                    }
                    onFocusChanged: (focus == true && _popupType == "Update" && myEngine.currentUserGroup == "User") ? infoTip2.show("Kann nicht geändert werden.",2000) : ""
                }
                Text {
                    id: password
                    text: qsTr("Password: ")
                    verticalAlignment: Text.AlignVCenter
                    font.bold: themeManagerInstance.isBold
                    color: themeManagerInstance.backColer
                    font.pointSize: themeManagerInstance.fontSizeDefault
                }
                TextField {
                    id: passwordEdit
                    text: _password
                    Layout.fillWidth: true
                    echoMode: TextInput.Password
                    onFocusChanged: {
                        if (focus)
                            if (_popupType == "Update") {
                                infoTip.show("Leer lassen, um Passwort nicht zu ändern",3000)
                                console.log("PASSWORT" + passwordEdit.text)
                            }
                    }
                    ToolTip {
                        id: infoTip
                    }
                    onTextChanged: {
                        console.log("PASSWORT TEXT CHANGED " + passwordEdit.text)
                    }
                }
                Text {
                    id: groupname
                    text: qsTr("GroupName: ")
                    verticalAlignment: Text.AlignVCenter
                    font.bold: themeManagerInstance.isBold
                    color: themeManagerInstance.backColer
                    font.pointSize: themeManagerInstance.fontSizeDefault
                }
                ComboBox {
                    id: groupnameCombo
                    Layout.fillWidth: true
                    model: groupNameModel
                    currentIndex: myEngine.groupNameComboIdx

                }
                Text {
                    id: mail
                    text: qsTr("e-Mail: ")
                    verticalAlignment: Text.AlignVCenter
                    font.bold: themeManagerInstance.isBold
                    color: themeManagerInstance.backColer
                    font.pointSize: themeManagerInstance.fontSizeDefault
                }
                TextField {
                    id: mailEdit
                    text: _email
                    Layout.fillWidth: true
                    validator: RegExpValidator { regExp: /^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,4}$/ }
                    placeholderText: "max.musterman@muster.de"
                }
            }
            RowLayout {
                id: actionrow
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredHeight: 50
                Layout.preferredWidth: 200

                Button {
                    id: createButton
                    text: (_popupType === "") ? "Create" : "Update"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    onClicked: {
                        if (fistnameEdit.text.match(/[A-Za-z]+/) && lastnameEdit.text .match(/[A-Za-z]+/) &&  groupnameCombo.currentIndex >= 0 && mailEdit.text.match(/^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,4}$/) ) {
                            if ((_popupType == "" && passwordEdit.text != "") || (_popupType == "Update")) {
                                var data = "id=" + _uID + "|" + "firstName=" + fistnameEdit.text + "|" + "lastName=" + lastnameEdit.text + "|" + "password=" + passwordEdit.text + "|" +
                                        "groupNameID=" + myEngine.getGroupIdByGroupName(groupnameCombo.currentText)  + "|" + "eMail=" + mailEdit.text
                                if (_popupType == "")
                                    myEngine.createDatabaseEntry("INSERTENTRY","USER",data)
                                else {
                                    myEngine.updateDatabaseEntry("UPDATEENTRY","USER",data)
                                    myEngine.deleteUser(1) //damit move in andere group geht !!!!!!!!!!!!!!!!!
                                }

                                _firstName = ""
                                //fistnameEdit.text = ""
                                _lastname = ""
                                //lastnameEdit.text = ""
                                _password = ""
                                //passwordEdit.text = ""
                                _email = ""
                                //mailEdit.text = ""
                                _popupType = ""
                                _groupName = ""
                                _groupType = ""
                                _popupType = ""
                                myEngine.setGroupNameComboIdx(-1)
                                myEngine.setGroupTypeComboIdx(-1)
                                popup.close();
                            }
                            else
                                errorPopupInstance.tryToOpen("Bitte füllen Sie die Felder korrekt aus.")
                        }
                        else
                            errorPopupInstance.tryToOpen("Bitte füllen Sie die Felder korrekt aus.")
                    }
                }
                Button {
                    id: cancelButton
                    text: "Cancel"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                    onClicked: {
                        _firstName = ""
                        //fistnameEdit.text = ""
                        _lastname = ""
                        //lastnameEdit.text = ""
                        _password = ""
                        //passwordEdit.text = ""
                        _email = ""
                        //mailEdit.text = ""
                        _popupType = ""
                        _groupName = ""
                        _groupType = ""
                        _popupType = ""
                        myEngine.setGroupNameComboIdx(-1)
                        myEngine.setGroupTypeComboIdx(-1)
                        popup.close()
                    }

                }
            }
        }

    }

    Component {
        id: hardware

        ColumnLayout {
            id: column
            spacing: 20

            Rectangle {
                id: header
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                color: themeManagerInstance.blue
                Text {
                    id: headertext
                    text: (_popupType == "") ? qsTr("Create Hardware") : qsTr("Update Hardware")
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
                    id: mac
                    text: qsTr("MAC: ")
                    verticalAlignment: Text.AlignVCenter
                    font.bold: themeManagerInstance.isBold
                    color: themeManagerInstance.backColer
                    font.pointSize: themeManagerInstance.fontSizeDefault

                }
                TextField {
                    id: macEdit
                    text: _mac
                    Layout.fillWidth: true
                    readOnly: (_popupType == "") ? false: true
                    validator: RegExpValidator { regExp: /^([0-9A-Fa-f]{2}[:]){5}([0-9A-Fa-f]{2})/ }
                    placeholderText: "XX:XX:XX:XX:XX:XX"

                }
                Text {
                    id: location
                    text: qsTr("Location: ")
                    verticalAlignment: Text.AlignVCenter
                    font.bold: themeManagerInstance.isBold
                    color: themeManagerInstance.backColer
                    font.pointSize: themeManagerInstance.fontSizeDefault
                }
                TextField {
                    id: locationEdit
                    text: _location
                    Layout.fillWidth: true
                    validator: RegExpValidator { regExp: /[0-9A-Za-z_-\s\.]+/ }
                    placeholderText: "Gebäude M1.2"
                }
                Text {
                    id: version
                    text: qsTr("Version: ")
                    verticalAlignment: Text.AlignVCenter
                    font.bold: themeManagerInstance.isBold
                    color: themeManagerInstance.backColer
                    font.pointSize: themeManagerInstance.fontSizeDefault
                }
                TextField {
                    id: versionEdit
                    text: _version
                    Layout.fillWidth: true
                    validator: RegExpValidator { regExp: /^([0-9]{1,2}[.][0-9]{1,2})/ }
                    placeholderText: "1.0"
                }
                Text {
                    id: groupname
                    text: qsTr("Group: ")
                    verticalAlignment: Text.AlignVCenter
                    font.bold: themeManagerInstance.isBold
                    color: themeManagerInstance.backColer
                    font.pointSize: themeManagerInstance.fontSizeDefault
                }
                ComboBox {
                    id: groupCombo
                    Layout.fillWidth: true
                    model: groupNameModel
                    currentIndex: _comboIDX

                }
            }
            RowLayout {
                id: actionrow
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredHeight: 50
                Layout.preferredWidth: 200

                Button {
                    id: createButton
                    text: (_popupType === "") ? "Create" : "Update"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    onClicked: {
                        if (macEdit.text.match(/^([0-9A-Fa-f]{2}[:]){5}([0-9A-Fa-f]{2})/) && locationEdit.text.match(/[0-9A-Za-z_-\s\.]+/) && versionEdit.text.match(/^([0-9]{1,2}[.][0-9]{1,2})/) && groupCombo.currentIndex >= 0) {
                            var data = "mac=" + macEdit.text + "|" + "location=" + locationEdit.text  + "|" +
                                    "version=" + versionEdit.text + "|" + "id=" + myEngine.getGroupIdByGroupName(groupCombo.currentText)
                            if (_popupType == "")
                                myEngine.createDatabaseEntry("INSERTENTRY","HARDWARE",data)
                            else
                                myEngine.updateDatabaseEntry("UPDATEENTRY","HARDWARE",data)
                            _mac = ""
                            _location = ""
                            _version = ""
                            _popupType = ""
                            _comboIDX = -1
                            myEngine.setGroupNameComboIdx(0)
                            popup.close();
                        }
                        else
                            errorPopupInstance.tryToOpen("Bitte füllen Sie die Felder korrekt aus.")
                    }
                }
                Button {
                    id: cancelButton
                    text: "Cancel"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    onClicked: {
                        _mac = ""
                        _location = ""
                        _version = ""
                        _comboIDX = -1
                        _popupType = ""
                        myEngine.setGroupNameComboIdx(0)
                        popup.close()
                    }
                }
            }
        }

    }




    Component {
        id: playlist
        ColumnLayout {
            id: column
            spacing: 20

            Rectangle {
                id: header
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                color: themeManagerInstance.blue
                Text {
                    id: headertext
                    text: (_popupType == "") ? qsTr("Create Playliste") : qsTr("Update Playliste")
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
                    id: playlistname
                    text: qsTr("Name: ")
                    verticalAlignment: Text.AlignVCenter
                    font.bold: themeManagerInstance.isBold
                    color: themeManagerInstance.backColer
                    font.pointSize: themeManagerInstance.fontSizeDefault
                }
                TextField {
                    id: playlistnameEdit
                    Layout.fillWidth: true
                    text: _name
                    validator: RegExpValidator { regExp: /[0-9A-Za-z_-\s\.]+/ }
                    placeholderText: "Playliste 1"
                }
                Text {
                    id: grouptype
                    text: qsTr("Group: ")
                    verticalAlignment: Text.AlignVCenter
                    font.bold: themeManagerInstance.isBold
                    color: themeManagerInstance.backColer
                    font.pointSize: themeManagerInstance.fontSizeDefault
                }
                ComboBox {
                    id: groupCombo
                    Layout.fillWidth: true
                    model: groupNameModel
                    currentIndex: myEngine.groupNameComboIdx
                }
                Text {
                    id: orientation
                    text: qsTr("Orientation: ")
                    verticalAlignment: Text.AlignVCenter
                    font.bold: themeManagerInstance.isBold
                    color: themeManagerInstance.backColer
                    font.pointSize: themeManagerInstance.fontSizeDefault
                }
                ComboBox {
                    id: orientationCombo
                    Layout.fillWidth: true
                    model: ListModel {
                        id: orientationModel
                        ListElement { text: "PANORAMA" }
                        ListElement { text: "PORTRAIT" }
                    }
                    currentIndex: (_orientation == "PANORAMA") ? 0 : (_orientation == "PORTRAIT") ? 1 : -1
                }


                RowLayout {
                    id: planon
                    Layout.columnSpan: 2
                    Layout.margins: 10
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 15



                    ListView {
                        id: freeClientsListView
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        property var checkmodel
                        model:  (myEngine.freeClientsToPlanOn) ? myEngine.freeClientsToPlanOn : ""
                        clip: true
                        headerPositioning: ListView.OverlayHeader
                        header: Rectangle {
                            height: 30
                            z: 3
                            width: parent.width
                            color: themeManagerInstance.blue
                            Text {
                                text: "Free Clients"
                                anchors.centerIn: parent
                                font.bold: themeManagerInstance.isBold
                                color: themeManagerInstance.fontColer
                                font.pointSize: themeManagerInstance.fontSizeDefault
                            }
                        }

                        delegate: Component {
                            id: contactDelegate
                            Rectangle {
                                width: parent.width
                                height: 30
                                color: ListView.isCurrentItem ? "grey" : "lightgrey"
                                Column {
                                    id: column
                                    Text {
                                        text: modelData
                                        verticalAlignment: Text.AlignVCenter
                                        clip: true
                                    }
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        //console.log(freeClientsListView.currentIndex)
                                        if (freeClientsListView.currentIndex !== index)
                                            freeClientsListView.currentIndex = index
                                    }
                                }
                            }
                        }
                    }

                    ColumnLayout {
                        Layout.preferredWidth: 50
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter

                        Image {
                            id: toright
                            source: "qrc:/icons/arrow.toright.png"
                            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                            Layout.preferredHeight: 50
                            Layout.preferredWidth: 50
                            MouseArea { id: toUsed; anchors.fill: parent; onClicked: myEngine.moveClientModel("FREEtoUSED",freeClientsListView.currentIndex) }
                        }
                        Image {
                            id: toleft
                            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                            Layout.preferredHeight: 50
                            Layout.preferredWidth: 50
                            source: "qrc:/icons/arrow.toleft.png"
                            MouseArea { id: tofree; anchors.fill: parent; onClicked: myEngine.moveClientModel("USEDtoFREE",usedClientsListView.currentIndex) }
                        }

                    }
                    ListView {
                        id: usedClientsListView
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        property var checkmodel
                        model: (myEngine.usedClientsToPlanOn) ? myEngine.usedClientsToPlanOn :  ""
                        clip: true
                        headerPositioning: ListView.OverlayHeader
                        header: Rectangle {
                            height: 30
                            z: 3
                            width: parent.width
                            color: themeManagerInstance.blue
                            Text {
                                text: "Used Clients"
                                anchors.centerIn: parent
                                font.bold: themeManagerInstance.isBold
                                color: themeManagerInstance.fontColer
                                font.pointSize: themeManagerInstance.fontSizeDefault
                            }
                        }

                        delegate: Component {
                            id: contactDelegate2
                            Rectangle {
                                width: parent.width
                                height: 30
                                color: ListView.isCurrentItem ? "grey" : "lightgrey"
                                Column {
                                    id: column
                                    Text {
                                        text: modelData
                                        verticalAlignment: Text.AlignVCenter
                                        clip: true
                                    }
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        //console.log(usedClientsListView.currentIndex)
                                        if (usedClientsListView.currentIndex !== index)
                                            usedClientsListView.currentIndex = index
                                    }
                                }
                            }
                        }
                    }
                }


            }
            RowLayout {
                id: actionrow
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredHeight: 50
                Layout.preferredWidth: 200

                Button {
                    id: createButton
                    text: (_popupType === "") ? "Create" : "Update"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    onClicked: {
                        var data = "name=" + playlistnameEdit.text + "|" + "orientation=" + (orientationCombo.currentIndex + 1)  + "|" +
                                "id_group=" + myEngine.getGroupIdByGroupName(groupCombo.currentText) + "|" + "id_playlist=" + _playId   + "|" +
                                "planedon=" + myEngine.usedClientsToPlanOn;
                        if (playlistnameEdit.text.match(/[0-9A-Za-z_-\s\.]+/) && orientationCombo.currentIndex >= 0 && groupCombo.currentIndex >= 0) {
                            myEngine.writeClientModel()
                            if (_popupType == "")
                                myEngine.createDatabaseEntry("INSERTENTRY","PLAYLIST",data)
                            else {
                                myEngine.updateDatabaseEntry("UPDATEENTRY","PLAYLIST",data)
                                //myEngine.deletePlaylist(1) nicht notwendig da kein move !!!!!!!!!!!!!!!!!!!!!!!!!!!
                            }

                            _playId = ""
                            _name = ""
                            _popupType = ""
                            _orientation =""
                            myEngine.setGroupNameComboIdx(0)
                            popup.close();
                        }
                        else
                            errorPopupInstance.tryToOpen("Bitte füllen Sie die Felder aus. Nur Used Playlist darf leer sein.")
                    }
                }
                Button {
                    id: cancelButton
                    text: "Cancel"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    onClicked: {
                        _playId = ""
                        _name = ""
                        _orientation =""
                        _popupType = ""
                        myEngine.setGroupNameComboIdx(0)
                        popup.close()
                    }
                }
            }
        }

    }

    Component {
        id: planning
        ColumnLayout {
            id: column
            spacing: 20

            //property alias contentFound3: grid.contentFound2

            Rectangle {
                id: header
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                color: themeManagerInstance.blue
                Text {
                    id: headertext
                    text: (_popupType == "" || _popupType == "Content") ? qsTr("Create Planning") : qsTr("Update Planning")
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

                //property alias contentFound2: myROW.contentFound

                Text {
                    id: fromPLaylist
                    text: qsTr("Playlist-Name: ")
                    verticalAlignment: Text.AlignVCenter
                    font.bold: themeManagerInstance.isBold
                    color: themeManagerInstance.backColer
                    font.pointSize: themeManagerInstance.fontSizeDefault
                }
                ComboBox {
                    id: fromPlaylistEdit
                    Layout.fillWidth: true
                    model: myEngine.getPlaylistNames()
                    currentIndex: (myEngine.playlistIdx)
                    //enabled: (_popupType == "") ? false : (_popupType == "Update") ? true : (_popupType == "Content") ? true : false
                }
                Text {
                    id: planningName
                    text: qsTr("Content-Name: ")
                    verticalAlignment: Text.AlignVCenter
                    font.bold: themeManagerInstance.isBold
                    color: themeManagerInstance.backColer
                    font.pointSize: themeManagerInstance.fontSizeDefault
                }
                //                RowLayout {
                //                    id: myROW

                //                    Layout.fillWidth: true
                //                    property bool contentFound: false

                TextField {
                    id: planningEdit
                    Layout.fillWidth: true
                    text: _playlistName
                    //                        onTextChanged: myROW.contentFound = myEngine.findContentByName(text);
                    //                        onTextEdited: myROW.contentFound = myEngine.findContentByName(text);
                    enabled: false
                    //                    }
                    //                    Button {
                    //                        Layout.preferredWidth: planningEdit.height
                    //                        Layout.preferredHeight: planningEdit.height
                    //                        background: Rectangle { color: (myROW.contentFound) ? "green" : "red" }
                    //                        onClicked: {
                    //                            myEngine.setNextStackItem("Content_PUSH");
                    //                            myEngine.setShowContentToAddPlanning(true);
                    //                            //myEngine.setSearchContent(planningEdit.text)
                    //                            popup.close()
                    //                        }
                    //                    }
                }


                Text {
                    id: validPeriod
                    text: qsTr("Valid-Period: ")
                    verticalAlignment: Text.AlignVCenter
                    font.bold: themeManagerInstance.isBold
                    color: themeManagerInstance.backColer
                    font.pointSize: themeManagerInstance.fontSizeDefault
                }

                RowLayout {
                    //_validperiod
                    Layout.fillWidth: width
                    TextField {
                        id: validPeriodEditstart
                        property var split: _validperiod.split("-")
                        onSplitChanged: text = (split[0]) ? split[0] : ""
                        Layout.fillWidth: true
                        text: split[0]
                        validator: RegExpValidator { regExp: /^([0-9]{2}[.][0-9]{2}[.][0-9]{4})$/ }
                        placeholderText: "01.01.2019"
                    }
                    Text {
                        id: spacerperiod
                        text: "-"
                    }
                    TextField {
                        id: validPeriodEditend
                        property var split: _validperiod.split("-")
                        onSplitChanged: text = (split[1]) ? split[1] : ""
                        Layout.fillWidth: true
                        text: split[1]
                        validator: RegExpValidator { regExp: /^([0-9]{2}[.][0-9]{2}[.][0-9]{4})$/ }

                        placeholderText: "31.12.2019"
                    }
                }




                //                TextField {
                //                    id: validPeriodEdit
                //                    Layout.fillWidth: true
                //                    text: _validperiod
                //                }
                Text {
                    id: weekdays
                    text: qsTr("Weekdays: ")
                    verticalAlignment: Text.AlignVCenter
                    font.bold: themeManagerInstance.isBold
                    color: themeManagerInstance.backColer
                    font.pointSize: themeManagerInstance.fontSizeDefault
                }
                RowLayout {
                    //_validperiod
                    id: myrow
                    Layout.fillWidth: true
                    //property var wday:  (_weekdays.split(",")) ? _weekdays.split(",") : "XX";

                    MyButton {
                        id: mo
                        text: "Mo"
                        Layout.fillWidth: true
                        isActive: _weekdays.includes("Mo")
                        onClicked: isActive = !isActive
                    }
                    MyButton {
                        id: di
                        text: "Di"
                        Layout.fillWidth: true
                        isActive: _weekdays.includes("Di")
                        onClicked: isActive = !isActive
                    }
                    MyButton {
                        id: mi
                        text: "Mi"
                        Layout.fillWidth: true
                        isActive: _weekdays.includes("Mi")
                        onClicked: isActive = !isActive
                    }
                    MyButton {
                        id: don
                        text: "Do"
                        Layout.fillWidth: true
                        isActive: _weekdays.includes("Do")
                        onClicked: isActive = !isActive
                    }
                    MyButton {
                        id: fr
                        text: "Fr"
                        Layout.fillWidth: true
                        isActive: _weekdays.includes("Fr")
                        onClicked: isActive = !isActive
                    }
                    MyButton {
                        id: sa
                        text: "Sa"
                        Layout.fillWidth: true
                        isActive: _weekdays.includes("Sa")
                        onClicked: isActive = !isActive
                    }
                    MyButton {
                        id: so
                        text: "So"
                        Layout.fillWidth: true
                        isActive: _weekdays.includes("So")
                        onClicked: isActive = !isActive
                    }
                }
                Text {
                    id: validTime
                    text: qsTr("Valid-Time: ")
                    verticalAlignment: Text.AlignVCenter
                    font.bold: themeManagerInstance.isBold
                    color: themeManagerInstance.backColer
                    font.pointSize: themeManagerInstance.fontSizeDefault
                }
                RowLayout {
                    //_validperiod
                    Layout.fillWidth: width
                    TextField {
                        id: validTimeEditstart
                        property var split: _validtime.split("-")
                        onSplitChanged: text = (split[0]) ? split[0] : ""
                        Layout.fillWidth: true
                        text: split[0]
                        validator: RegExpValidator { regExp: /^([0-9]{2}:[0-9]{2}:[0-9]{2})$/ }
                        placeholderText: "00:00:00"
                    }
                    Text {
                        id: spacerperiod2
                        text: "-"
                    }
                    TextField {
                        id: validTimeEditend
                        property var split: _validtime.split("-")
                        onSplitChanged: text = (split[1]) ? split[1] : ""
                        Layout.fillWidth: true
                        text: split[1]
                        validator: RegExpValidator { regExp: /^([0-9]{2}:[0-9]{2}:[0-9]{2})$/ }
                        placeholderText: "23:59:59"
                    }
                }
                Text {
                    id: planningtype
                    text: qsTr("Planning-Type: ")
                    verticalAlignment: Text.AlignVCenter
                    font.bold: themeManagerInstance.isBold
                    color: themeManagerInstance.backColer
                    font.pointSize: themeManagerInstance.fontSizeDefault
                }
                ComboBox {
                    id: planningTypeCombo
                    Layout.fillWidth: true
                    currentIndex: (myEngine.planningTypeComboIdx)
                    model: ListModel {
                        id: model
                        ListElement { text: "Standard" }
                        ListElement { text: "Event" }
                    }
                    onCurrentIndexChanged: myEngine.setPlanningTypeComboIdx(currentIndex)
                }
                Text {
                    id: duration
                    text: qsTr("Duration: ")
                    verticalAlignment: Text.AlignVCenter
                    font.bold: themeManagerInstance.isBold
                    color: themeManagerInstance.backColer
                    font.pointSize: themeManagerInstance.fontSizeDefault
                }

                TextField {
                    id: durationEdit
                    Layout.fillWidth: true
                    text: myEngine.getDurationFromContent(myEngine.getContentIdByContentName(planningEdit.text),_planningID,myEngine.getPlaylistIdWithPlaylistName(fromPlaylistEdit.currentText))
                    //enabled: false
                }
            }

            RowLayout {
                id: actionrow
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredHeight: 50
                Layout.preferredWidth: 200

                //property bool contentFound4: column.contentFound3

                Button {
                    id: createButton
                    text: (_popupType === "" || _popupType == "Content") ? "Create" : "Update"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    onClicked: {
                        //                        console.log("!!!!!!!FOUND !" + parent.contentFound4)
                        //                        console.log("!!!!!!!fromPlaylistEdit.currentIndex !" + fromPlaylistEdit.currentIndex)
                        //                        console.log("!!!!!!!validPeriodEditstart.text !" + validPeriodEditstart.text)
                        //                        console.log("!!!!!!!validPeriodEditend.text !" + validPeriodEditend.text)
                        //                        console.log("!!!!!!!validTimeEditstart.text !" + validTimeEditstart.text)
                        //                        console.log("!!!!!!!validTimeEditend.text !" + validTimeEditend.text)
                        //                        console.log("!!!!!!!planningName.text !" + planningName.text)
                        //                                                console.log("!!!!!!!planningTypeCombo.currentIndex !" + planningTypeCombo.currentIndex)
                        if (durationEdit.text != "-1" && fromPlaylistEdit.currentIndex >= 0 && validPeriodEditstart.text.match(/^([0-9]{2}[.][0-9]{2}[.][0-9]{4})$/) && validPeriodEditend.text.match(/^([0-9]{2}[.][0-9]{2}[.][0-9]{4})$/) && validTimeEditstart.text.match(/^([0-9]{2}:[0-9]{2}:[0-9]{2})$/) && validTimeEditend.text.match(/^([0-9]{2}:[0-9]{2}:[0-9]{2})$/) && planningName.text != "" && planningTypeCombo.currentIndex >= 0 && ( mo.isActive || di.isActive || mi.isActive || don.isActive || fr.isActive || sa.isActive || so.isActive )) {
                            var days = myEngine.generateWeekDays(mo.isActive,di.isActive,mi.isActive,don.isActive,fr.isActive,sa.isActive,so.isActive)
                            var data = /* "name=" + planningEdit.text + "|" + */ "validPeriod=" + validPeriodEditstart.text + "-" + validPeriodEditend.text  + "|" +
                                    "weekdays=" + myEngine.generateWeekDays(mo.isActive,di.isActive,mi.isActive,don.isActive,fr.isActive,sa.isActive,so.isActive) +
                                    "|" + "validTime=" + validTimeEditstart.text  + "-" + validTimeEditend.text  + "|" +
                                    "type=" + (planningTypeCombo.currentText)  + "|" +
                                    //"id_playlist=" + myEngine.getPlaylistIdWithIdx(myEngine.playlistIdx)
                                    "id_playlist=" + myEngine.getPlaylistIdWithPlaylistName(fromPlaylistEdit.currentText)+ "|" +
                                    "id_content=" + myEngine.getContentIdByContentName(planningEdit.text)+ "|" +
                                    "id_planning=" + _planningID + "|" +
                                    "duration=" + durationEdit.text
                            if (_popupType == "" || _popupType == "Content") {
                                data += "|" + "id_order=" + myEngine.nextPlanningOrder();
                                myEngine.createDatabaseEntry("INSERTENTRY","PLANNING",data)
                            }
                            else {
                                data += "|" + "id_order=" + _id_order
                                myEngine.updateDatabaseEntry("UPDATEENTRY","PLANNING",data)
                                myEngine.deletePlanning(1) // damit move geht !!!!!!!!!!!!
                            }
                            //planningTypeCombo.currentIndex = 0
                            _popupType = ""
                            _validtime = ""
                            _validperiod = ""
                            // planningTypeCombo.currentIndex = -1
                            _weekdays = "YY"
                            _playlistName =""
                            _planningID=""
                            //console.log("BEIM BUTTON" + _weekdays)

                            myEngine.setPlanningTypeComboIdx(-1)
                            popup.close();
                        }
                        else
                            errorPopupInstance.tryToOpen("Bitte füllen Sie die Felder korrekt aus.")





                    }
                }
                Button {
                    id: cancelButton
                    text: "Cancel"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    onClicked: {
                        //planningTypeCombo.currentIndex = 0
                        _popupType = ""
                        //planningTypeCombo.currentIndex = -1
                        _validtime = ""
                        _validperiod = ""
                        _planningID=""
                        _weekdays = "YY"
                        myEngine.setPlanningTypeComboIdx(-1)
                        _playlistName =""
                        console.log("BEIM CANCEL" + _weekdays)
                        popup.close()
                    }
                }
            }
        }

    }

}
