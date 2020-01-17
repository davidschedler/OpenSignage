#include "include/engine.h"

/**
 * @brief Engine::Engine Default-Konstruktor, welcher alle Initialen Variablen für die Gui setzt
 * @param parent
 */
Engine::Engine(QObject *parent) :
    QObject(parent),
    m_webSocketClient(QUrl(QStringLiteral("ws://localhost:1234"))),
    m_ident(""),
    m_isAuthenticated(false),
    m_navigationMenuIdx(0),
    m_hardwareIdx(-1),
    m_groupsIdx(-1),
    m_usersIdx(-1),
    m_contentIdx(-1),
    m_playlistIdx(-1),
    m_planningIdx(-1),
    m_groupNameComboIdx(-1),
    m_groupTypeComboIdx(-1),
    m_showContentToAddPlanning(false),
    m_freeClientsComboIdx(-1),
    m_planningTypeComboIdx(-1)
{
    setLoginMessage("please login");
    connect(&m_webSocketClient, &WebSocketClient::incommingMessage, this, &Engine::incommingData);
    connect(&m_webSocketClient, &WebSocketClient::disconnected, this, &Engine::websocketDisconnected);
    connect(&m_webSocketClient, &WebSocketClient::connected, this, &Engine::websocketConnected);
}

/**
 * @brief Engine::setIsAuthenticated Nach erfolgreicher Authentifikation hole alle Daten vom CMS
 * @param isAuthenticated true = hole Daten von CMS, false = nichts machen
 */
void Engine::setIsAuthenticated(bool isAuthenticated)
{
    if (m_isAuthenticated == isAuthenticated)
        return;

    if (isAuthenticated) { // initial getdata for all
        getDatabaseData("GETDATA","GROUPS");
        getDatabaseData("GETDATA","USERS");
        getDatabaseData("GETDATA","CONTENT");
        getDatabaseData("GETDATA","HARDWARE");
        getDatabaseData("GETDATA","PLAYLISTS");
        getDatabaseData("GETDATA","PLANNINGS");
    }

    m_isAuthenticated = isAuthenticated;
    emit isAuthenticatedChanged(m_isAuthenticated);
}

/**
 * @brief Engine::setNavigationMenuIdx Setze den Navigationsindex (Haupt-Navigation)
 * @param navigationMenuIdx Navigationsindex
 */
void Engine::setNavigationMenuIdx(int navigationMenuIdx)
{
    if (m_navigationMenuIdx == navigationMenuIdx)
        return;

    m_navigationMenuIdx = navigationMenuIdx;
    emit navigationMenuIdxChanged(m_navigationMenuIdx);
}

/**
 * @brief Engine::setGroupsList Setze komplette Gruppenliste
 * @param groups neue Gruppenliste
 */
void Engine::setGroupsList(QList<QObject *> groups)
{
    if (m_groupsList == groups)
        return;

    m_groupsList = groups;
    emit groupsListChanged(m_groupsList);
}

/**
 * @brief Engine::setHardwareList Setze neue Hardwareliste
 * @param hardwareList neue Hardwareliste
 */
void Engine::setHardwareList(QList<QObject *> hardwareList)
{
    if (m_hardwareList == hardwareList)
        return;

    m_hardwareList = hardwareList;
    emit hardwareListChanged(m_hardwareList);
}

/**
 * @brief Engine::incommingData Eingehende binäre Nachricht vom Websocket. Zuständig für das Parsen der Nachricht inkl. erstellen der notwendigen Objecte
 * Content, Groups, Users, Hardware, Playlists, Planings. Falls Objecte bereits vorhanden werden diese aktualisiert.
 * @param message Binäre-Websocket-Nachricht welche einem Json-Dokument entspricht
 */
void Engine::incommingData(QByteArray message)
{
    qDebug() << "Message received:" << message;
    QJsonParseError *err = new QJsonParseError();
    QJsonDocument jDoc = QJsonDocument::fromJson(message, err);

    if (err->error != 0)
    {
        qDebug() << err->errorString();
    }
    if (jDoc.isNull())
    {
        qDebug() << "Invalid json document, exiting...";
        return;
    }
    else if (jDoc.isEmpty())
    {
        qDebug() << "Empty json document, exiting...";
        return;
    }
    else if (jDoc.isObject())
    {
        QJsonObject mainObj = jDoc.object();
        QVariantMap mainMap = mainObj.toVariantMap();

        if (!mainObj.contains("COMMAND"))
        {
            qDebug() << "Json does not contains command...";
            return;
        }

        if (mainObj.contains("AUTH")) {
            QVariantList dataList = mainMap.value("AUTH").toList();
            QString groupType;
            bool isAuth = false;
            for (int i = 0; i < dataList.count(); i++){
                QVariantMap dataMap = dataList.at(i).toMap();
                if (dataMap.contains("auth")){
                    if (dataMap.value("auth").toString() == "success") {
                        isAuth = true;
                    }
                }
                if (dataMap.contains("groupType")){
                    groupType = dataMap.value("groupType").toString();
                    setCurrentUserGroup(groupType);
                }
            }
            if (isAuth) {
                emit setIsAuthenticated(true);
                setLoginMessage("login successfully");
            }
            else {
                emit setIsAuthenticated(false);
                setLoginMessage("login failed, pls try again");

            }
        }

        if (mainObj.contains("GROUPS")) {
            QVariantList dataList = mainMap.value("GROUPS").toList();
            QString groupName, groupType;
            int id = -1, id_grouptype = -1;

            Groups *newGroup = nullptr;

            for (int i = 0; i < dataList.count(); i++){
                QVariantMap dataMap = dataList.at(i).toMap();
                if (dataMap.contains("ID")){
                    id = dataMap.value("ID").toInt();
                }
                if (dataMap.contains("ID_Type")){
                    id_grouptype = dataMap.value("ID_Type").toInt();
                }
                if (dataMap.contains("GroupName")){
                    groupName = dataMap.value("GroupName").toString();
                }
                if (dataMap.contains("GroupType")){
                    groupType = dataMap.value("GroupType").toString();
                }
                if (id != -1 && !groupName.isEmpty() && !groupType.isEmpty()) {

                    Groups *searchGroup = getGroupById(id);
                    if (searchGroup == nullptr) {
                        newGroup = new Groups(id,groupName,groupType,id_grouptype,this);
                        m_groupsList.append(newGroup);
                        emit groupsListChanged(m_groupsList);
                    }
                    else {
                        searchGroup->setGroupName(groupName);
                        searchGroup->setGroupType(groupType);
                        searchGroup->setId_grouptype(id_grouptype);
                    }
                }
            }
        }
        if (mainObj.contains("USERS")) {
            QVariantList dataList = mainMap.value("USERS").toList();
            QString firstName, lastName, eMail, groupName, groupType;
            int id = -1;

            Users *newUser = nullptr;

            for (int i = 0; i < dataList.count(); i++){
                QVariantMap dataMap = dataList.at(i).toMap();
                if (dataMap.contains("ID")){
                    id = dataMap.value("ID").toInt();
                }
                if (dataMap.contains("FirstName")){
                    firstName = dataMap.value("FirstName").toString();
                }
                if (dataMap.contains("LastName")){
                    lastName = dataMap.value("LastName").toString();
                }
                if (dataMap.contains("eMail")){
                    eMail = dataMap.value("eMail").toString();
                }
                if (dataMap.contains("GroupName")){
                    groupName = dataMap.value("GroupName").toString();
                }
                if (dataMap.contains("GroupType")){
                    groupType = dataMap.value("GroupType").toString();
                }
                if (dataMap.contains("eMail")){
                    eMail = dataMap.value("eMail").toString();
                }

                if (id != -1 && !firstName.isEmpty() && !lastName.isEmpty() && !eMail.isEmpty()
                        && !groupName.isEmpty() && !groupType.isEmpty()) {

                    Groups *searchGroup = getGroupByName(groupName);
                    Users *searchUser = nullptr;

                    if (searchGroup != nullptr)
                        searchUser = searchGroup->getUserById(id);
                    else
                        return;

                    if (searchUser == nullptr) {
                        newUser = new Users(id,firstName,lastName,groupName,groupType,eMail,this);
                        searchGroup->addUser(newUser);
                    }
                    else {
                        searchUser->setFirstName(firstName);
                        searchUser->setLastName(lastName);
                        searchUser->setGroupName(groupName);
                        searchUser->setGroupType(groupType);
                        searchUser->setEMail(eMail);
                    }
                }
            }
            emit refreshModelDataCount();
        }
        if (mainObj.contains("HARDWARE")) {
            QVariantList dataList = mainMap.value("HARDWARE").toList();
            QString mac, location, version, groupName, groupType, planedon;

            Hardware *newHardware = nullptr;

            for (int i = 0; i < dataList.count(); i++){
                QVariantMap dataMap = dataList.at(i).toMap();
                if (dataMap.contains("MAC")){
                    mac = dataMap.value("MAC").toString();
                }
                if (dataMap.contains("Location")){
                    location = dataMap.value("Location").toString();
                }
                if (dataMap.contains("Version")){
                    version = dataMap.value("Version").toString();
                }
                if (dataMap.contains("GroupName")){
                    groupName = dataMap.value("GroupName").toString();
                }
                if (dataMap.contains("GroupType")){
                    groupType = dataMap.value("GroupType").toString();
                }
                if (dataMap.contains("PlanedOn")){
                    planedon = dataMap.value("PlanedOn").toString();
                }

                if (!mac.isEmpty() && !location.isEmpty() && !version.isEmpty()
                        && !groupName.isEmpty() && !groupType.isEmpty() && !planedon.isEmpty()) {

                    Hardware *searchHardware = getHardwareByMac(mac);
                    if (searchHardware == nullptr) {
                        newHardware = new Hardware(mac,groupName,groupType,location,version,planedon.toInt(),this);
                        m_hardwareList.append(newHardware);
                        emit hardwareListChanged(m_hardwareList);
                    }
                    else {
                        searchHardware->setGroupName(groupName);
                        searchHardware->setGroupType(groupType);
                        searchHardware->setVersion(version);
                        searchHardware->setLocation(location);
                        searchHardware->setPlanedonPlaylist(planedon.toInt());
                    }
                }
            }
        }
        if (mainObj.contains("CONTENT")) {
            QVariantList dataList = mainMap.value("CONTENT").toList();
            QString name, md5, path, type, reso, size, time;
            int id = -1;

            Content *newContent = nullptr;

            for (int i = 0; i < dataList.count(); i++){
                QVariantMap dataMap = dataList.at(i).toMap();
                if (dataMap.contains("ID")){
                    id = dataMap.value("ID").toInt();
                }
                if (dataMap.contains("Name")){
                    name = dataMap.value("Name").toString();
                }
                if (dataMap.contains("MD5")){
                    md5 = dataMap.value("MD5").toString();
                }
                if (dataMap.contains("FilePath")){
                    path = dataMap.value("FilePath").toString();
                    type = path.mid(path.lastIndexOf(".")+1);

                }
                if (dataMap.contains("Reso")){
                    reso = dataMap.value("Reso").toString();
                }
                if (dataMap.contains("Size")){
                    size = dataMap.value("Size").toString();
                }
                if (dataMap.contains("Time")){
                    time = dataMap.value("Time").toString();
                }

                if (id != -1 && !name.isEmpty() && !md5.isEmpty() && !path.isEmpty() && !type.isEmpty() &&
                        !reso.isEmpty() && !size.isEmpty() && !time.isEmpty()) {

                    Content *searchContent = getContentById(id);

                    if (searchContent == nullptr) {
                        newContent = new Content(id,name,md5,path,type,reso,size,time,this);
                        m_contentTypes.append(type);
                        emit contentTypesChanged(m_contentTypes);
                        m_contentList.append(newContent);
                        m_contentListBACKUP.append(newContent); // halte saubere backup datei vor
                        emit contentListChanged(m_contentList);
                    }
                    else {
                        searchContent->setMd5(md5);
                        searchContent->setName(name);
                        searchContent->setFilePath(path);
                        searchContent->setFileType(type);
                        searchContent->setReso(reso);
                        searchContent->setSize(size);
                        searchContent->setTime(time);
                    }
                }
            }
        }
        if (mainObj.contains("PLAYLISTS")) {
            QVariantList dataList = mainMap.value("PLAYLISTS").toList();
            QString name, groupname, orientation, planedon;
            int id = -1;

            Playlist *newPlaylist = nullptr;

            for (int i = 0; i < dataList.count(); i++){
                QVariantMap dataMap = dataList.at(i).toMap();
                if (dataMap.contains("ID")){
                    id = dataMap.value("ID").toInt();
                }
                if (dataMap.contains("Name")){
                    name = dataMap.value("Name").toString();
                }
                if (dataMap.contains("Orientation")){
                    orientation = dataMap.value("Orientation").toString();
                }
                if (dataMap.contains("GroupName")){
                    groupname = dataMap.value("GroupName").toString();
                }
                if (dataMap.contains("PlanedOn")){
                    planedon = dataMap.value("PlanedOn").toString();
                }

                QStringList splitPlanedOn = planedon.split(",");
                foreach (QString mac, splitPlanedOn) {
                    for (int k = 0 ; k < m_hardwareList.count(); k++) {
                        Hardware *hardware = (Hardware*) m_hardwareList.at(k);
                        if (hardware->mac() == mac) {
                            hardware->setPlanedonPlaylist(id);
                            QString data = "mac=" + hardware->mac() + "|" + "location=" + hardware->location() + "|" +  "version=" + hardware->version() + "|" +  "id=" + getGroupIdByGroupName(hardware->groupName()) + "|" +  "planedon=" + QString::number(hardware->planedonPlaylist());
                            updateDatabaseEntry("UPDATEENTRY","HARDWARE",data);
                        }
                    }
                }
                hardwareListChanged(m_hardwareList);

                if (id != -1 && !name.isEmpty() && !groupname.isEmpty() && !orientation.isEmpty()) {

                    Playlist *searchPlaylist = getPlaylistById(id);
                    if (searchPlaylist == nullptr) {
                        newPlaylist = new Playlist(id,name,orientation,groupname,planedon,this);
                        m_playlistList.append(newPlaylist);
                        emit playlistListChanged(m_playlistList);
                    }
                    else {
                        searchPlaylist->setName(name);
                        searchPlaylist->setGroupName(groupname);
                        searchPlaylist->setOrientation(orientation);
                        searchPlaylist->setPlanedon(planedon);
                    }
                }
            }
            emit refreshModelDataCount();
        }
        if (mainObj.contains("PLANNINGS")) {
            QVariantList dataList = mainMap.value("PLANNINGS").toList();
            QString validPeriod, weekdays, validTime, type, name;
            int id_playlist = -1, id_planning = -1, id_content = -1, id_order = -1, duration = -1;

            Planning *newPlanning = nullptr;

            for (int i = 0; i < dataList.count(); i++){
                QVariantMap dataMap = dataList.at(i).toMap();
                if (dataMap.contains("ID_Playlist")){
                    id_playlist = dataMap.value("ID_Playlist").toInt();
                }
                if (dataMap.contains("ID_Planning")){
                    id_planning = dataMap.value("ID_Planning").toInt();
                }
                if (dataMap.contains("ID_Content")){
                    id_content = dataMap.value("ID_Content").toInt();
                }
                if (dataMap.contains("Valid_Period")){
                    validPeriod = dataMap.value("Valid_Period").toString();
                }
                if (dataMap.contains("Weekdays")){
                    weekdays = dataMap.value("Weekdays").toString();
                }
                if (dataMap.contains("Valid_Time")){
                    validTime = dataMap.value("Valid_Time").toString();
                }
                if (dataMap.contains("Type")){
                    type = dataMap.value("Type").toString();
                }
                if (dataMap.contains("ContentName")){
                    name = dataMap.value("ContentName").toString();
                }
                if (dataMap.contains("ID_Order")){
                    id_order = dataMap.value("ID_Order").toInt();
                }
                if (dataMap.contains("Duration")){
                    duration = dataMap.value("Duration").toInt();
                }

                if (duration != -1 && id_playlist != -1 && id_planning != -1 && id_content != -1 && id_order != -1 && !validPeriod.isEmpty()
                        && !weekdays.isEmpty() && !validTime.isEmpty() && !type.isEmpty() && !name.isEmpty()) {

                    Playlist *searchPlaylist = getPlaylistById(id_playlist);
                    Planning *searchPlanning = nullptr;

                    if (searchPlaylist != nullptr)
                        searchPlanning = searchPlaylist->getPlanningById(id_planning);
                    else
                        return;

                    if (searchPlanning == nullptr) {
                        newPlanning = new Planning(id_planning,id_content,name,validPeriod,weekdays,validTime,type,id_order,duration,this);
                        searchPlaylist->addPlanning(newPlanning);
                    }
                    else {
                        searchPlanning->setContentName(name);
                        searchPlanning->setWeekdays(weekdays);
                        searchPlanning->setValidPeriod(validPeriod);
                        searchPlanning->setValidTime(validTime);
                        searchPlanning->setType(type);
                        searchPlanning->setId_order(id_order);
                        searchPlanning->setDuration(duration);
                    }
                }
            }
        }
    }
}

/**
 * @brief Engine::setHardwareIdx Setze Hardwareindex
 * @param hardwareIdx Hardwareindex
 */
void Engine::setHardwareIdx(int hardwareIdx)
{
    if (m_hardwareIdx == hardwareIdx)
        return;

    m_hardwareIdx = hardwareIdx;
    emit hardwareIdxChanged(m_hardwareIdx);
}

/**
 * @brief Engine::setGroupsIdx Setze Gruppenindex
 * @param groupsIdx
 */
void Engine::setGroupsIdx(int groupsIdx)
{
    if (m_groupsIdx == groupsIdx)
        return;

    m_groupsIdx = groupsIdx;
    emit groupsIdxChanged(m_groupsIdx);
}

/**
 * @brief Engine::setUsersIdx Setze Benutzerindex
 * @param usersIdx Benutzerindex
 */
void Engine::setUsersIdx(int usersIdx)
{
    if (m_usersIdx == usersIdx)
        return;

    m_usersIdx = usersIdx;
    emit usersIdxChanged(m_usersIdx);
}

/**
 * @brief Engine::setContentList Setze Contentindex
 * @param contentList Benutzerindex
 */
void Engine::setContentList(QList<QObject *> contentList)
{
    if (m_contentList == contentList)
        return;

    m_contentList = contentList;
    emit contentListChanged(m_contentList);
}

/**
 * @brief Engine::setPlaylistList Setze eine neue Playlist
 * @param playlistList neue Playliste
 */
void Engine::setPlaylistList(QList<QObject *> playlistList)
{
    if (m_playlistList == playlistList)
        return;

    m_playlistList = playlistList;
    emit playlistListChanged(m_playlistList);
}

/**
 * @brief Engine::setPlaylistIdx Setze Playlistenindex
 * @param playlistIdx Playlistenindex
 */
void Engine::setPlaylistIdx(int playlistIdx)
{
    if (m_playlistIdx == playlistIdx)
        return;

    m_playlistIdx = playlistIdx;
    emit playlistIdxChanged(m_playlistIdx);
}

/**
 * @brief Engine::setPlanningIdx Setze Planungsindex
 * @param planningIdx Planungsindex
 */
void Engine::setPlanningIdx(int planningIdx)
{
    if (m_planningIdx == planningIdx)
        return;

    m_planningIdx = planningIdx;
    emit planningIdxChanged(m_planningIdx);
}

/**
 * @brief Engine::setGroupNameComboIdx Setze Index der Combobox von Gruppen-Namen
 * @param groupNameComboIdx neuer Index
 */
void Engine::setGroupNameComboIdx(int groupNameComboIdx)
{
    //    if (m_groupNameComboIdx == groupNameComboIdx)
    //        return;

    m_groupNameComboIdx = groupNameComboIdx;
    emit groupNameComboIdxChanged(m_groupNameComboIdx);
}

/**
 * @brief Engine::setGroupTypeComboIdx Setze Index der Combobox von Gruppen-Typen
 * @param groupTypeComboIdx neuer Index
 */
void Engine::setGroupTypeComboIdx(int groupTypeComboIdx)
{
    //    if (m_groupTypeComboIdx == groupTypeComboIdx)
    //        return;

    m_groupTypeComboIdx = groupTypeComboIdx;
    emit groupTypeComboIdxChanged(m_groupTypeComboIdx);
}

/**
 * @brief Engine::setCurrentUserGroup Setze Auswahl der zur Verfügung stehenden GruppenTypen
 * @param currentUserGroup Zur Verfügung stehenden GruppenTypen
 */
void Engine::setCurrentUserGroup(QString currentUserGroup)
{
    if (m_currentUserGroup == currentUserGroup)
        return;

    if (currentUserGroup == "CMS-Administrator") {
        m_defaultGroupTypes.append("CMS-Administrator");
        m_defaultGroupTypes.append("Group-Administrator");
        m_defaultGroupTypes.append("User");
    }
    if (currentUserGroup == "Group-Administrator") {
        m_defaultGroupTypes.append("Group-Administrator");
        m_defaultGroupTypes.append("User");
    }
    if (currentUserGroup == "User") {
        m_defaultGroupTypes.append("User");
    }

    m_currentUserGroup = currentUserGroup;
    emit defaultGroupTypesChanged(m_defaultGroupTypes);
    emit currentUserGroupChanged(m_currentUserGroup);
}

/**
 * @brief Engine::setDefaultGroupTypes Setze die standard GruppenTypen
 * @param defaultGroupTypes standard Gruppentypen
 */
void Engine::setDefaultGroupTypes(QStringList defaultGroupTypes)
{
    if (m_defaultGroupTypes == defaultGroupTypes)
        return;

    m_defaultGroupTypes = defaultGroupTypes;
    emit defaultGroupTypesChanged(m_defaultGroupTypes);
}

/**
 * @brief Engine::setContentIdx Setze den Contentindex
 * @param contentIdx Contentindex
 */
void Engine::setContentIdx(int contentIdx)
{
    if (m_contentIdx == contentIdx)
        return;

    m_contentIdx = contentIdx;
    emit contentIdxChanged(m_contentIdx);
}

/**
 * @brief Engine::setNextStackItem Setze StackItem für die GUI (Playlist => Planings)
 * @param nextStackItem Item, welches in der GUI angezeigt werden soll
 */
void Engine::setNextStackItem(QString nextStackItem)
{
    if (m_nextStackItem == nextStackItem)
        return;

    m_nextStackItem = nextStackItem;
    emit nextStackItemChanged(m_nextStackItem);
}

/**
 * @brief Engine::setContentTypes Setze die ContentTypen
 * @param contentTypes ContentTypen
 */
void Engine::setContentTypes(QStringList contentTypes)
{
    if (m_contentTypes == contentTypes)
        return;

    m_contentTypes = contentTypes;
    emit contentTypesChanged(m_contentTypes);
}

/**
 * @brief Engine::setShowContentToAddPlanning Zeige Content an um Planungen durch zu führen
 * @param showContentToAddPlanning Zeige Popup von Content an
 */
void Engine::setShowContentToAddPlanning(int showContentToAddPlanning)
{
    if (m_showContentToAddPlanning == showContentToAddPlanning)
        return;

    m_showContentToAddPlanning = showContentToAddPlanning;
    emit showContentToAddPlanningChanged(m_showContentToAddPlanning);
}

/**
 * @brief Engine::setSearchContent Setze GUI-Eingabe für Content-Suche
 * @param searchContent Eingegebener Such-String
 */
void Engine::setSearchContent(QString searchContent)
{
    if (m_searchContent == searchContent)
        return;

    m_searchContent = searchContent;
    emit searchContentChanged(m_searchContent);
}

/**
 * @brief Engine::setFreeClientsComboIdx Setze den Index der Combobox zur Auswahl der noch nicht eingehängten Hardware
 * @param freeClientsComboIdx neuer Index
 */
void Engine::setFreeClientsComboIdx(int freeClientsComboIdx)
{
    //        if (m_freeClientsComboIdx == freeClientsComboIdx)
    //            return;

    m_freeClientsComboIdx = freeClientsComboIdx;
    emit freeClientsComboIdxChanged(m_freeClientsComboIdx);
}

/**
 * @brief Engine::setPlanningTypeComboIdx Setze neuen ComboBox Index für den Planungs-Typ
 * @param planningTypeComboIdx neuer Index
 */
void Engine::setPlanningTypeComboIdx(int planningTypeComboIdx)
{
    if (m_planningTypeComboIdx == planningTypeComboIdx)
        return;

    m_planningTypeComboIdx = planningTypeComboIdx;
    emit planningTypeComboIdxChanged(m_planningTypeComboIdx);
}

/**
 * @brief Engine::setLoginMessage Setzte Nachricht für die Login-Maske
 * @param loginMessage Neue Login Nachricht
 */
void Engine::setLoginMessage(QString loginMessage)
{
    if (m_loginMessage == loginMessage)
        return;

    m_loginMessage = loginMessage;
    emit loginMessageChanged(m_loginMessage);
}

/**
 * @brief Engine::setFreeClientsToPlanOn Setze neue Liste von freien Clients die noch noch keine Playliste besitzen
 * @param freeClientsToPlanOn List die noch zur Planung zur Verfügung stehenden Clients beinhaltet
 */
void Engine::setFreeClientsToPlanOn(QStringList freeClientsToPlanOn)
{
    if (m_freeClientsToPlanOn == freeClientsToPlanOn)
        return;

    m_freeClientsToPlanOn = freeClientsToPlanOn;
    emit freeClientsToPlanOnChanged(m_freeClientsToPlanOn);
}

/**
 * @brief Engine::setUsedClientsToPlanOn Setze neue Liste von verplanten Clients die bereits eine Playliste besitzen
 * @param usedClientsToPlanOn Liste die nicht mehr zur Planung zur Verfügung stehende Clients beinhaltet
 */
void Engine::setUsedClientsToPlanOn(QStringList usedClientsToPlanOn)
{
    if (m_usedClientsToPlanOn == usedClientsToPlanOn)
        return;

    m_usedClientsToPlanOn = usedClientsToPlanOn;
    emit usedClientsToPlanOnChanged(m_usedClientsToPlanOn);
}

/**
 * @brief Engine::websocketDisconnected Websocket ist nicht Verbunden (Popup anzeigen in der Gui)
 */
void Engine::websocketDisconnected()
{
    setIsWebsocketConnected(false);
}

/**
 * @brief Engine::websocketConnected Websocket ist verbunden (Popup nicht anzeigen in der Gui)
 */
void Engine::websocketConnected()
{
    setIsWebsocketConnected(true);
}

/**
 * @brief Engine::setIsWebsocketConnected Status der WebsocketVerbindung
 * @param isWebsocketConnected true = hat eine Verbindung, false = hat keine Verbindung
 */
void Engine::setIsWebsocketConnected(bool isWebsocketConnected)
{
    if (m_isWebsocketConnected == isWebsocketConnected)
        return;

    m_isWebsocketConnected = isWebsocketConnected;
    emit isWebsocketConnectedChanged(m_isWebsocketConnected);
}

/**
 * @brief Engine::getDatabaseData Generiere die zu versendende Nachricht und schicke sie über den Websocket an das CMS
 * @param command Kommando => Anweisung für das CMS
 * @param type Typ => Auf welches Ziel bezieht sich das Kommando
 * @param data Data => Die neuen Daten vom Benutzer
 */
void Engine::getDatabaseData(QString command, QString type, QString data)
{
    m_webSocketClient.getDatabaseData(generateByteMessage(command,type,data));
}

/**
 * @brief Engine::createDatabaseEntry Generiere die zu versendende Nachrickt und schicke sie über den Websocket an das CMS
 * dabei muss um die Syncronität zur DB beizubehalten die Daten vom CMS neu bezogen werden
 * @param command Kommando => Anweisung für das CMS
 * @param type Typ => Auf welches Ziel bezieht sich das Kommando
 * @param data Data => Die neuen Daten vom Benutzer
 */
void Engine::createDatabaseEntry(QString command, QString type, QString data)
{

    m_webSocketClient.getDatabaseData(generateByteMessage(command,type,data));

    //resync
    if ((type == "GROUPS") || (type == "GROUP"))
        getDatabaseData("GETDATA","GROUPS");
    else if ((type == "USERS") || (type == "USER"))
        getDatabaseData("GETDATA","USERS");
    else if ((type == "CONTENT"))
        getDatabaseData("GETDATA","CONTENT");
    else if ((type == "PLAYLISTS") || (type == "PLAYLIST")){
        getDatabaseData("GETDATA","PLAYLISTS");
        getDatabaseData("GETDATA","HARDWARE");
    }
    else if ((type == "PLANNINGS") || (type == "PLANNING"))
        getDatabaseData("GETDATA","PLANNINGS");
    else if (type == "HARDWARE") {
        getDatabaseData("GETDATA","PLAYLISTS");
        getDatabaseData("GETDATA","HARDWARE");
    }
}

/**
 * @brief Engine::updateDatabaseEntry Generiere die zu versendende Nachrickt und schicke sie über den Websocket an das CMS
 * dabei muss um die Syncronität zur DB beizubehalten die Daten vom CMS neu bezogen werden
 * @param command Kommando => Anweisung für das CMS
 * @param type Typ => Auf welches Ziel bezieht sich das Kommando
 * @param data Data => Die neuen Daten vom Benutzer
 */
void Engine::updateDatabaseEntry(QString command, QString type, QString data)
{
    m_webSocketClient.getDatabaseData(generateByteMessage(command,type,data));

    QTimer::singleShot(1000, [type,this]() {
        //resync
        if ((type == "GROUPS") || (type == "GROUP"))
            getDatabaseData("GETDATA","GROUPS");
        else if ((type == "USERS") || (type == "USER"))
            getDatabaseData("GETDATA","USERS");
        else if ((type == "CONTENT"))
            getDatabaseData("GETDATA","CONTENT");
        else if ((type == "PLAYLISTS") || (type == "PLAYLIST")){
            //getDatabaseData("GETDATA","HARDWARE");
            getDatabaseData("GETDATA","PLAYLISTS");
        }
        else if ((type == "PLANNINGS") || (type == "PLANNING"))
            getDatabaseData("GETDATA","PLANNINGS");
        else if (type == "HARDWARE") {
            getDatabaseData("GETDATA","HARDWARE");
            //getDatabaseData("GETDATA","PLAYLISTS");
        }
    } );
}

/**
 * @brief Engine::getGroupNames Hole alle GruppenNamen, welche zur Verfügung stehen
 * @return onFail = nullptr, onSuccess = Groups*
 */
QStringList Engine::getGroupNames()
{
    Groups *groups = nullptr;
    QStringList ret;
    for (int i = 0 ; i < m_groupsList.size() ; i++){
        groups = (Groups*) m_groupsList.at(i);
        ret.append(groups->groupName());
    }
    return ret;
}

/**
 * @brief Engine::getGroupTypes Hole aktuelle GruppenTypen, welche zur Verfügung stehen
 * @return onFail = "" , onSuccess = GroupTypes
 */
QStringList Engine::getGroupTypes()
{
    Groups *groups = nullptr;
    QStringList ret;
    for (int i = 0 ; i < m_groupsList.size() ; i++){
        groups = (Groups*) m_groupsList.at(i);
        ret.append(groups->groupType());
    }
    return ret;
}

/**
 * @brief Engine::getGroupIdByGroupName Hole Gruppen-ID anhand des Gruppen-Namens
 * @param groupName Gruppen-Name
 * @return onFail = "", onSucces = Group-ID als String
 */
QString Engine::getGroupIdByGroupName(QString groupName)
{
    QString ret;
    for (int i = 0 ; i < m_groupsList.size() ; i++){
        Groups *temp = (Groups*) m_groupsList.at(i);
        if (temp->groupName() == groupName) {
            ret = QString::number(temp->id()); // ist oki da nur ganze zahlen
            break;
        }
    }
    return ret;
}

/**
 * @brief Engine::getGroupTypeIdByGroupType Hole GruppenTye-ID anhand des Namens des Gruppen-Typs
 * @param groupType Name des Gruppentyps
 * @return ID des Gruppentyps
 */
QString Engine::getGroupTypeIdByGroupType(QString groupType)
{
    QString ret;
    if (groupType == "CMS-Administrator")
        ret = "1";
    if (groupType == "Group-Administrator")
        ret = "2";
    if (groupType == "User")
        ret = "3";

    return ret;
}

/**
 * @brief Engine::getGroupNameIdxByGroupName Hole den GruppenNamen-Index anhand des GruppenNamen
 * @param groupName Gruppenname
 * @return onFail = -1, onSuccess = GruppenName-Index
 */
int Engine::getGroupNameIdxByGroupName(QString groupName)
{
    int ret = -1;
    for (int i = 0 ; i < m_groupsList.size() ; i++){
        Groups *temp = (Groups*) m_groupsList.at(i);
        if (temp->groupName() == groupName) {
            ret = i;
            break;
        }
    }
    return ret;
}

/**
 * @brief Engine::getGroupTypeIdxByGroupType Hole den GruppenTyp-Index anhand des GruppenTyps
 * @param groupType Name von Gruppentyp
 * @return onFail = -1, onSucces = Gruppentyp-Index
 */
int Engine::getGroupTypeIdxByGroupType(QString groupType)
{
    int ret = -1;
    for (int i = 0 ; i < m_groupsList.size() ; i++){
        Groups *temp = (Groups*) m_groupsList.at(i);
        if (temp->groupType() == groupType) {
            ret = i;
            break;
        }
    }
    return ret;
}

/**
 * @brief Engine::getPlaylistIdWithIdx Hole den Playlisten-ID anhand des GUI-Indexes
 * @param idx Index der Gui
 * @return ID der Playliste
 */
int Engine::getPlaylistIdWithIdx(int idx)
{
    int ret = -1;
    if (idx < 0 || idx > m_playlistList.count())
        return ret;
    Playlist *temp = (Playlist*) m_playlistList.at(idx);
    if (temp != nullptr)
        ret = temp->id();
    return ret;

}

/**
 * @brief Engine::getPlaylistIdWithPlaylistName Hole Playlist-iD anhand des Playlisten-Namens
 * @param name Name der Playliste
 * @return ID der Playliste
 */
int Engine::getPlaylistIdWithPlaylistName(QString name)
{
    int ret = -1;
    for (int i = 0 ; i < m_playlistList.size() ; i++){
        Playlist *temp = (Playlist*) m_playlistList.at(i);
        if (temp->name() == name) {
            ret = temp->id(); // ist oki da nur ganze zahlen
            break;
        }
    }
    return ret;
}

/**
 * @brief Engine::deleteHardware Lösche die aktuell in der GUI ausgewählte Hardware und teile es dem CMS mit
 */
void Engine::deleteHardware()
{
    if ( (hardwareIdx() >= 0) && (hardwareIdx() < m_hardwareList.count())) {
        Hardware *deleteHardware = (Hardware*) m_hardwareList.at(hardwareIdx());
        QString mac = "mac=" + deleteHardware->mac();

        deleteHardware->deleteLater();
        m_hardwareList.removeAt(hardwareIdx());
        setHardwareIdx(-1);
        emit hardwareListChanged(m_hardwareList);

        m_webSocketClient.getDatabaseData(generateByteMessage("DELETEENTRY","HARDWARE",mac));
    }
}

/**
 * @brief Engine::deleteUser Lösche den in der GUI ausgewählten User und teile es dem CMS mit.
 * @param type Type = 0 (CMS mitteilen) Type = 1 (CMS nicht mitteilen)
 */
void Engine::deleteUser(int type)
{
    if ( (groupsIdx() >= 0) && (groupsIdx() < m_groupsList.count() ) ) {
        Groups *tempGroups = (Groups*) m_groupsList.at(groupsIdx());
        if ((usersIdx() >= 0 ) && (usersIdx() < tempGroups->usersList().count())) {
            Users *deleteUser = (Users*) tempGroups->usersList().at(m_usersIdx);
            QString id = "id=" + QString::number(deleteUser->id());

            tempGroups->deleteUser(m_usersIdx);
            deleteUser->deleteLater();
            setUsersIdx(-1);
            tempGroups->usersListChanged(tempGroups->usersList());
            emit refreshModelDataCount();

            if (type == 0) //hard delete (inside db) => soft delete only in m_userlist
                m_webSocketClient.getDatabaseData(generateByteMessage("DELETEENTRY","USER",id));
        }
    }
}

/**
 * @brief Engine::deleteGroup Lösche die in der GUI ausgewählte Gruppe und teile es dem CMS mit.
 * @param type Type = 0 (CMS mitteilen) Type = 1 (CMS nicht mitteilen)
 * @return true = Wenn Gruppe keine Abhängigkeiten mehr besitzt, false = WEnn Gruppe noch Abhängigkeiten besitzt
 */
bool Engine::deleteGroup(int type)
{
    bool ret = checkDeleteGroup();

    if (ret) {
        Groups *temp = (Groups*) m_groupsList.at(groupsIdx());
        QString id = "id=" + QString::number(temp->id());
        temp->deleteLater();
        m_groupsList.removeAt(groupsIdx());
        setGroupsIdx(-1);
        emit groupsListChanged(m_groupsList);

        if (type == 0) //hard delete (inside db) => soft delete only in m_userlist
            m_webSocketClient.getDatabaseData(generateByteMessage("DELETEENTRY","GROUP",id));
    }
    return ret;
}

/**
 * @brief Engine::deletePlanning Lösche die in der GUI ausgewählten Planung
 * @param type Type = 0 (CMS mitteilen) Type = 1 (CMS nicht mitteilen)
 */
void Engine::deletePlanning(int type)
{
    if ( (playlistIdx() >= 0) && (playlistIdx() < m_playlistList.count() ) ) {
        Playlist *tempPlaylist = (Playlist*) m_playlistList.at(playlistIdx());
        if ((planningIdx() >= 0 ) && (planningIdx() < tempPlaylist->planningList().count())) {
            Planning *deletePlanning = (Planning*) tempPlaylist->planningList().at(m_planningIdx);
            QString id = "id=" + QString::number(deletePlanning->id());

            tempPlaylist->deletePlanning(m_planningIdx);
            deletePlanning->deleteLater();
            setPlanningIdx(-1);
            tempPlaylist->planningListChanged(tempPlaylist->planningList());

            if (type == 0) //hard delete (inside db) => soft delete only in m_userlist
                m_webSocketClient.getDatabaseData(generateByteMessage("DELETEENTRY","PLANNING",id));
        }
    }
}

/**
 * @brief Engine::calculateDashboardInfo Berechne die in der QuickInfo (Dashboard) angezeigten Informationen
 * @param target Welche Berechnung soll angestellt werden (Content,Playlisten,Clients)
 * @return Target gefunden = Ergebnis der Berechnung, Target nicht gefunden -1
 */
int Engine::calculateDashboardInfo(QString target)
{
    if (target == "Contents") {
        return m_contentList.count();
    }
    else if (target == "Playlisten") {
        return m_playlistList.count();
    }
    else if (target == "Clients") {
        return m_hardwareList.count();
    }

    return -1;
}

/**
 * @brief Engine::checkDeleteGroup Überprüfe, ob eine Gruppe Abhängigkeiten besitzt (Hardware,User,Playlisten)
 * @return true = Kann gelöscht werden, false = Kann nicht gelöscht werden.
 */
bool Engine::checkDeleteGroup()
{
    bool hardware = true;
    bool user = true;
    bool playlist = true;

    //initial check for operation
    if ( (m_groupsList.isEmpty()) || (groupsIdx() == -1) || (groupsIdx() >= m_groupsList.count()))
        return false;

    //target group to delete
    Groups *temp = (Groups*) m_groupsList.at(groupsIdx());
    QString grpToDelete = temp->groupName();

    //check hardware
    if (m_hardwareList.isEmpty())
        hardware = hardware && true;
    for (int i = 0 ; i < m_hardwareList.count(); i++) {
        Hardware *temp = (Hardware*) m_hardwareList.at(i);
        if (temp->groupName() == grpToDelete)
            hardware = hardware && false; //nicht löschen da noch hardware mit der grp da
        else
            hardware = hardware && true; // noch hardware da;
    }

    //check users
    QList<QObject*> userToCheck = temp->usersList();
    if (userToCheck.isEmpty())
        user = user && true;
    else
        user = user && false;

    //check playlist
    if (m_playlistList.isEmpty())
        playlist = playlist && true;
    for (int i = 0 ; i < m_playlistList.count() ; i++) {
        Playlist *temp = (Playlist*) m_playlistList.at(i);
        if (temp->groupName() == grpToDelete)
            playlist = playlist && false;
        else
            playlist = playlist && true;
    } //plannings muss nicht geprüft werden, da sie von playlist abhängig sind
    bool ret = (playlist && user && hardware);
    return ret;
}

/**
 * @brief Engine::deletePlaylist Lösche die in der GUI ausgewählten Playliste
 * @param type Type = 0 (CMS mitteilen) Type = 1 (CMS nicht mitteilen)
 * @return true = wurde gelöscht, false = wurde nicht gelöscht
 */
bool Engine::deletePlaylist(int type)
{
    bool ret = false;
    if ( (playlistIdx() >= 0) && (playlistIdx() < m_playlistList.count() ) ) {
        Playlist *tempPlaylist = (Playlist*) m_playlistList.at(playlistIdx());
        QString id = "id=" + QString::number(tempPlaylist->id());



        if (!tempPlaylist->planningList().isEmpty())
            return false;

        for (int i = 0 ; i < m_hardwareList.count() ; i ++ ) {
            Hardware *hardware = (Hardware*) m_hardwareList.at(i);
            if (hardware->planedonPlaylist() == tempPlaylist->id()) {
                hardware->setPlanedonPlaylist(0);
                QString data = "mac=" + hardware->mac() + "|" + "location=" + hardware->location() + "|" +  "version=" + hardware->version() + "|" +  "id=" + getGroupIdByGroupName(hardware->groupName()) + "|" +  "planedon=" + QString::number(hardware->planedonPlaylist());
                updateDatabaseEntry("UPDATEENTRY","HARDWARE",data);
            }
        }

        tempPlaylist->deleteLater();
        m_playlistList.removeAt(playlistIdx());
        setPlaylistIdx(-1);
        emit playlistListChanged(m_playlistList);

        if (type == 0) //hard delete (inside db) => soft delete only in m_userlist
            m_webSocketClient.getDatabaseData(generateByteMessage("DELETEENTRY","PLAYLIST",id));
        ret = true;
    }
    return ret;
}

/**
 * @brief Engine::sortAlgorithm Auswahlfunktion der möglichen Sortieralgorithmen
 * @param sortTarget Ziel der Sortierung (Content,Groups,Users,Playlisten,Planungen,Clients)
 * @param sortType Attribut das zu Sortieren ist
 * @param sortOrder Aufsteigend/Absteigend
 */
void Engine::sortAlgorithm(int sortTarget, int sortType, int sortOrder)
{
    if ((sortType < 0) || (sortType < 0) || (sortOrder < 0))
        return;

    switch(sortTarget) {
    case SortTarget::CONTENT:
        sortContent(sortType,sortOrder);
        break;
    case SortTarget::GROUPS:
        sortGroups(sortType,sortOrder);
        break;
    case SortTarget::USERS:
        sortUsers(sortType,sortOrder);
        break;
    case SortTarget::PLAYLISTS:
        sortPlaylists(sortType,sortOrder);
        break;
    case SortTarget::PLANNINGS:
        sortPlanning(sortType,sortOrder);
        break;
    case SortTarget::HARDWARE:
        sortHardware(sortType,sortOrder);
        break;
    default:
        break;
    }
}

/**
 * @brief Engine::searchAlgorithm Sortiere die Content Suche nach einen Attribut
 * @param sortTarget Suchfeld der Contentsuche
 * @param sortType (ID,NAME,TYPE)
 * @param searchtString gesuchte Datei
 */
void Engine::searchAlgorithm(int sortTarget, int sortType, QString searchtString)
{
    m_contentList = m_contentListBACKUP;    //rest for search
    emit contentListChanged(m_contentList);

    if (searchtString == "") {
        return;
    }

    if (sortTarget == 0) { // content
        if (m_contentList.isEmpty())
            return;

        QList<Content*> foundList;
        for (int i = 0 ; i < m_contentList.count() ; i ++) {
            Content *temp = (Content*) m_contentList.at(i);

            if (sortType == SortContent::CID) {
                if (temp->id() == searchtString.toInt())
                    foundList.append(temp);
            }
            else if (sortType == SortContent::CNAME) {
                if (temp->name().toLower().contains(searchtString.toLower()))
                    foundList.append(temp);
            }
            else if (sortType == SortContent::CTYPE) {
                if (temp->fileType().toLower().contains(searchtString.toLower()))
                    foundList.append(temp);
            }
        }

        m_contentList.clear();
        for (int i = 0 ; i < foundList.count() ; i ++) {
            m_contentList.append(foundList.at(i));
        }

        emit contentListChanged(m_contentList);
    }
}

/**
 * @brief Engine::findContentByName Suche Content anhand seines Namens
 * @param name Content-Name
 * @return true = gefunden, false = nicht gefunden
 */
bool Engine::findContentByName(QString name)
{
    bool ret = false;
    for (int i = 0 ; i < m_contentList.size() ; i++){
        Content *temp = (Content*) m_contentList.at(i);
        if (temp->name() == name) {
            ret = true;
            break;
        }
    }
    return ret;
}

/**
 * @brief Engine::getPlaylistNames Hole alle Namen der aktuell zugänglichen Playlisten
 * @return Liste aller Namen der Playlisten
 */
QStringList Engine::getPlaylistNames()
{
    QStringList ret;
    if (m_playlistList.isEmpty())
        return QStringList();

    for (int i = 0 ; i < m_playlistList.size() ; i++){
        Playlist *temp = (Playlist*) m_playlistList.at(i);
        if (temp->name() != "") {
            ret.append(temp->name());
        }
    }
    return ret;
}

/**
 * @brief Engine::getContentIdByContentName Hole die ID des Contents anhand der Namens
 * @param name Content-Name
 * @return Content-ID
 */
int Engine::getContentIdByContentName(QString name)
{
    int ret = -1;
    for (int i = 0 ; i < m_contentList.size() ; i++){
        Content *temp = (Content*) m_contentList.at(i);
        if (temp->name() == name) {
            ret = temp->id();
            break;
        }
    }
    return ret;
}

/**
 * @brief Engine::calculateListviewModelDataCount Berechnungs-Funktion für die Account-Übersicht
 * @param targetName Das zu berechnende Ziel
 * @param targetID Die zu berechnende ID
 * @return -1 Kein Taget gefunden, ansonsten das Ergebnis
 */
int Engine::calculateListviewModelDataCount(QString targetName, int targetID)
{
    int ret = -1;
    if (targetName == "UserInGroups") {
        if (m_groupsList.isEmpty())
            return 0;
        for (int i = 0 ; i < m_groupsList.size() ; i++){
            Groups *temp = (Groups*) m_groupsList.at(i);
            if (temp->id() == targetID) {
                ret = temp->usersList().count();
                return ret;
            }
        }
    }
    if (targetName == "PlanningsInPlaylists") {
        if (m_playlistList.isEmpty())
            return 0;
        for (int i = 0 ; i < m_playlistList.size() ; i++){
            Playlist *temp = (Playlist*) m_playlistList.at(i);
            if (temp->id() == targetID) {
                ret = temp->planningList().count();
                return ret;
            }
        }
    }
    if (targetName == "PlaylistInGroups") {
        if (playlistList().isEmpty())
            return 0;
        if (m_groupsList.isEmpty())
            return 0;
        int count = 0;
        for (int i = 0 ; i < m_groupsList.count() ; i++) {
            Groups *group  = (Groups*) m_groupsList.at(i);
            if (group->id() == targetID) {
                for (int j = 0 ; j < m_playlistList.count() ; j++) {
                    Playlist *play = (Playlist*) m_playlistList.at(j);
                    if (play->groupName() == group->groupName())
                        count++;
                }
            }
        }
        ret = count;
    }
    if (targetName == "HardwareInGroups") {
        if (m_hardwareList.isEmpty())
            return 0;
        if (m_groupsList.isEmpty())
            return 0;
        int count = 0;
        for (int i = 0 ; i < m_groupsList.count() ; i++) {
            Groups *group  = (Groups*) m_groupsList.at(i);
            if (group->id() == targetID) {
                for (int j = 0 ; j < m_hardwareList.count() ; j++) {
                    Hardware *hardware = (Hardware*) m_hardwareList.at(j);
                    if (hardware->groupName() == group->groupName())
                        count++;
                }
            }
        }
        ret = count;
    }
    if (targetName == "HardwareInPlaylists") {
        if (m_playlistList.isEmpty())
            return 0;
        int count = 0;
        for (int i = 0 ; i < m_playlistList.count() ; i++) {
            Playlist *play  = (Playlist*) m_playlistList.at(i);

            if (play->id() == targetID) {
                QStringList split = play->planedon().split(",");
                foreach (QString temp, split) {
                    if (temp != "")
                        count++;
                }
                break;
            }
        }
        ret = count;
    }
    return ret;
}

/**
 * @brief Engine::groupCreatedby Hole Information ein, welche Gruppe von welcher erstellt wurde
 * @return ID der Gruppe die eine Gruppe erstellt hat.
 */
QString Engine::groupCreatedby()
{
    QString ret = "";
    for (int i = 0 ; i < m_groupsList.count() ; i++) {
        Groups *tempGroup = (Groups*) m_groupsList.at(i);
        for (int j = 0 ; j < tempGroup->usersList().count() ; j++) {
            Users *tempUser = (Users*) tempGroup->usersList().at(j);
            if (m_ident == (tempUser->eMail())) {
                ret = QString::number(tempGroup->id());
                break;
            }
        }
    }
    return ret;
}

/**
 * @brief Engine::isOwnGrp Prüfe ob eine Gruppe die eigene ist (Diese kann nicht gelöscht werden)
 * @return true = eigene Gruppe, false = fremde Gruppe
 */
bool Engine::isOwnGrp()
{
    bool ret = false;
    if (groupsIdx() >= 0 && groupsIdx() < m_groupsList.count()) {

        QString myGroup;
        for (int i = 0 ; i < m_groupsList.count() ; i++) {
            Groups *tempGroup = (Groups*) m_groupsList.at(i);
            for (int j = 0 ; j < tempGroup->usersList().count() ; j++) {
                Users *tempUser = (Users*) tempGroup->usersList().at(j);
                if (m_ident == (tempUser->eMail())) {
                    myGroup = QString::number(tempGroup->id());
                    break;
                }
            }
        }
        Groups *tempGroup = (Groups*) m_groupsList.at(groupsIdx());
        if (myGroup == QString::number(tempGroup->id())) {
            ret = true;
        }
    }
    return ret;
}

/**
 * @brief Engine::isHardwareInUse Prüfe ob eine Hardware in einer Playliste eingehängt ist
 * @return true = Hardware noch in einer Playliste eingehängt, false = Hardware nicht mehr in Benutzung
 */
bool Engine::isHardwareInUse()
{
    bool ret = false;
    if (hardwareIdx() >= 0 && hardwareIdx() < m_hardwareList.count()) {
        Hardware *hardware = (Hardware*) m_hardwareList.at(hardwareIdx());
        for ( int i = 0 ; i < m_playlistList.count() ; i++) {
            Playlist *play = (Playlist*) m_playlistList.at(i);
            if (hardware->planedonPlaylist() == play->id()) {
                ret = true;
                break;
            }
        }
    }
    return ret;
}

/**
 * @brief Engine::createClientModels Prüfe für die Playlisten-Ansicht welche Hardware noch nicht bzw. bereits eine Playliste besitzen
 * @param type Hole die Hardware die frei oder die bereits in Benutzung ist.
 */
void Engine::createClientModels(QString type)
{
    //noch verfügbar
    if (type == "FREE") {
        if (m_hardwareList.isEmpty())
            return;
        else {
            m_freeClientsToPlanOn.clear();
            for (int i = 0 ; i < m_hardwareList.count(); i ++){
                Hardware *hardware = (Hardware*) m_hardwareList.at(i);
                if (hardware->planedonPlaylist() == 0) {
                    m_freeClientsToPlanOn.append(hardware->mac());
                }
            }
        }
        emit freeClientsToPlanOnChanged(m_freeClientsToPlanOn);
    }

    //used in der playliste
    if (type == "USED") {
        m_usedClientsToPlanOn.clear();
        if (playlistIdx() >= 0 && playlistIdx() < m_playlistList.count()) {
            Playlist *playlist = (Playlist*) m_playlistList.at(playlistIdx());

            QStringList splitPlanedOn = playlist->planedon().split(",");

            foreach (QString split, splitPlanedOn) {
                m_usedClientsToPlanOn.append(split);
            }
        }
        emit usedClientsToPlanOnChanged(m_usedClientsToPlanOn);
    }
}

/**
 * @brief Engine::moveClientModel Hilfs-Funktion zum verschieben einer Hardware (frei/in Benutzung). Dabei wird die Information mit dem CMS syncronisiert
 * @param type (FREEtoUSED / USEDtoFREE)
 * @param idx Index der zu verschiebenden Hardware
 */
void Engine::moveClientModel(QString type, int idx)
{
    if (type == "FREEtoUSED") {
        if (idx >= 0 && idx < freeClientsToPlanOn().count()) {
            QString moveClient = freeClientsToPlanOn().at(idx);
            m_freeClientsToPlanOn.removeAt(idx);

            QStringList newUsed = m_usedClientsToPlanOn;
            newUsed.append(moveClient);
            setUsedClientsToPlanOn(newUsed);

            emit freeClientsToPlanOnChanged(m_freeClientsToPlanOn);
            emit usedClientsToPlanOnChanged(m_usedClientsToPlanOn);
        }
    }
    if (type == "USEDtoFREE") {
        if (idx >= 0 && idx < usedClientsToPlanOn().count()) {
            QString moveClient = usedClientsToPlanOn().at(idx);
            m_usedClientsToPlanOn.removeAt(idx);

            QStringList newFree = m_freeClientsToPlanOn;
            newFree.append(moveClient);
            m_movedClientsForDbSync.append(moveClient); //neu
            setFreeClientsToPlanOn(newFree);

            emit freeClientsToPlanOnChanged(m_freeClientsToPlanOn);
            emit usedClientsToPlanOnChanged(m_usedClientsToPlanOn);
        }
    }
}

/**
 * @brief Engine::writeClientModel, Syncronisiere die Hardware mit dem CMS
 */
void Engine::writeClientModel()
{
    if (m_hardwareList.isEmpty())
        return;
    else {
        for (int i = 0 ; i < m_hardwareList.count() ; i++) {
            Hardware *temp = (Hardware*) m_hardwareList.at(i);
            foreach (QString client, m_movedClientsForDbSync) {
                if (temp->mac() == client) {
                    temp->setPlanedonPlaylist(0);
                    QString data = "mac=" + temp->mac() + "|" + "location=" + temp->location() + "|" +  "version=" + temp->version() + "|" +  "id=" + getGroupIdByGroupName(temp->groupName()) + "|" +  "planedon=" + QString::number(temp->planedonPlaylist());
                    updateDatabaseEntry("UPDATEENTRY","HARDWARE",data);
                }
            }
        }
        m_movedClientsForDbSync.clear();
    }
}

/**
 * @brief Engine::generateWeekDays Erstele einen Wochentag String für die Gültigkeit
 * @param mo true = gültig, false = ungültig
 * @param di true = gültig, false = ungültig
 * @param mi true = gültig, false = ungültig
 * @param don true = gültig, false = ungültig
 * @param fr true = gültig, false = ungültig
 * @param sa true = gültig, false = ungültig
 * @param so true = gültig, false = ungültig
 * @return Erstellter Wochentag-String
 */
QString Engine::generateWeekDays(bool mo, bool di, bool mi, bool don, bool fr, bool sa, bool so)
{
    QStringList ret;
    if (mo)
        ret.append("Mo");
    if (di)
        ret.append("Di");
    if (mi)
        ret.append("Mi");
    if (don)
        ret.append("Do");
    if (fr)
        ret.append("Fr");
    if (sa)
        ret.append("Sa");
    if (so)
        ret.append("So");

    QString final = ret.join(",");
    return final;
}

/**
 * @brief Engine::clientsToPlanOnCount Hole Anzahl der freien bzw. bereits genutzten Hardwares
 * @param target FREE / USED
 * @return Anzahl der gewünschten Ziel-Hardware
 */
int Engine::clientsToPlanOnCount(QString target)
{
    int ret = 0;
    if (target == "FREE") {
        ret = m_freeClientsToPlanOn.count();
    }
    if (target == "USED")
        ret = m_usedClientsToPlanOn.count();

    return ret;
}

/**
 * @brief Engine::nextPlanningOrder Prüfe welche Planung als nächstes genutzt werden soll
 * @return nächste Planung
 */
int Engine::nextPlanningOrder()
{
    int ret = -1;
    if (playlistIdx() >= 0 && playlistIdx() < playlistList().count()) {
        Playlist *play = (Playlist*) m_playlistList.at(playlistIdx());
        ret = play->planningList().count() + 1;
    }
    return ret;
}

/**
 * @brief Engine::changePlanningPosition Vertausche Reihenfolge zweier Planungen
 * @param destination (UP/DOWN)
 */
void Engine::changePlanningPosition(QString destination)
{
    if (destination == "UP") {
        if (planningIdx() > 0 && playlistIdx() >= 0 && playlistIdx() < m_playlistList.count()) {
            Playlist *play = (Playlist*) m_playlistList.at(playlistIdx());
            play->swapPlanning(planningIdx(),planningIdx()-1);

            QPair<QString, QString> updatePair = play->swapPlanningInformation(planningIdx(),planningIdx()-1);
            updatePair.first = updatePair.first + "|" + "id_playlist=" + QString::number(play->id());
            updatePair.second = updatePair.second + "|" + "id_playlist=" + QString::number(play->id());

            setPlanningIdx(m_planningIdx-1);

            updateDatabaseEntry("UPDATEENTRY","PLANNING",updatePair.first);
            updateDatabaseEntry("UPDATEENTRY","PLANNING",updatePair.second);
        }
    }
    if (destination == "DOWN") {
        if (playlistIdx() >= 0 && playlistIdx() < m_playlistList.count()) {
            Playlist *play = (Playlist*) m_playlistList.at(playlistIdx());
            if (planningIdx() < play->planningList().count()-1) {
                play->swapPlanning(planningIdx(),planningIdx()+1);

                QPair<QString, QString> updatePair = play->swapPlanningInformation(planningIdx(),planningIdx()+1);
                updatePair.first = updatePair.first + "|" + "id_playlist=" + QString::number(play->id());
                updatePair.second = updatePair.second + "|" + "id_playlist=" + QString::number(play->id());

                setPlanningIdx(m_planningIdx+1);

                updateDatabaseEntry("UPDATEENTRY","PLANNING",updatePair.first);
                updateDatabaseEntry("UPDATEENTRY","PLANNING",updatePair.second);
            }
        }
    }
}

/**
 * @brief Engine::getDurationFromContent Hole die Dauer eines Contents
 * @param id_content ID des Contents
 * @param id_planning ID der Planung
 * @param id_playlist ID der Playliste
 * @return Keine Dauer vorhanden = -1, ansonsten die Dauer des Contents
 */
int Engine::getDurationFromContent(int id_content, int id_planning, int id_playlist)
{
    int ret = -1;
    for (int i = 0 ; i < m_contentList.count() ; i++) {
        Content *checkDuration = (Content*) m_contentList.at(i);
        if (checkDuration->id() == id_content) {
            ret = checkDuration->getTimeAsDurationInMS();
            if (ret == -1) {
                for (int j = 0 ; j < m_playlistList.count() ; j ++) {
                    Playlist *play = (Playlist*) m_playlistList.at(j);
                    if (play->id() == id_playlist) {
                        for (int k = 0 ; k < play->planningList().count() ; k ++) {
                            Planning *plan = (Planning*) play->planningList().at(k);
                            if (plan->id() == id_planning)
                                ret = plan->duration();
                        }
                    }
                }
            }
            break;
        }
    }
    return ret;
}

/**
 * @brief Engine::generateByteMessage Generiere ein ByteArray mit Informationen die die Struktur eines JsonDokuments besitzen.
 * @param command (Zeil für ein Plugin)
 * @param type (Anweisung für ein Plugin)
 * @param data (Daten für ein Plugin)
 * @return Erstelle Byte-Nachricht
 */
QByteArray Engine::generateByteMessage(QString command, QString type, QString data)
{
    QJsonObject jsonMainObj;
    jsonMainObj.insert("COMMAND", QJsonValue::fromVariant(command));
    jsonMainObj.insert("TYPE", QJsonValue::fromVariant(type));
    jsonMainObj.insert("IDENT", QJsonValue::fromVariant(m_ident));

    if (data != "") {
        // data example ident=david.schedler|password=123456
        QJsonObject jsonDataObj;
        QStringList splittedData = data.split("|");
        QString singleData;

        foreach (singleData, splittedData) {
            QStringList singleSplitted = singleData.split("=");
            jsonDataObj.insert(singleSplitted.first(),QJsonValue::fromVariant(singleSplitted.last()));
        }
        jsonMainObj.insert("DATA",jsonDataObj);
    }

    QJsonDocument jDoc(jsonMainObj);
    QByteArray message = QString(jDoc.toJson(QJsonDocument::Compact)).toUtf8().data();
    qDebug() << message;

    return message;
}

/**
 * @brief Engine::getGroupById Hole ein Gruppen-Object anhand seiner ID
 * @param id Gesuchte ID
 * @return Gruppen-Objekt mit der gewünschten ID, wenn nicht gefunden nullptr
 */
Groups *Engine::getGroupById(int id)
{
    Groups *ret = nullptr;
    for (int i = 0 ; i < m_groupsList.size() ; i++){
        Groups *temp = (Groups*) m_groupsList.at(i);
        if (temp->id() == id) {
            ret = temp;
            break;
        }
    }
    return ret;
}

/**
 * @brief Engine::getGroupByName Hole ein GruppenObject anhand seines Namens
 * @param name Gesuchter Name
 * @return Gruppen-Objekt mit der gewünschten Name, wenn nicht gefunden nullptr
 */
Groups *Engine::getGroupByName(QString name)
{
    Groups *ret = nullptr;
    for (int i = 0 ; i < m_groupsList.size() ; i++){
        Groups *temp = (Groups*) m_groupsList.at(i);
        if (temp->groupName() == name) {
            ret = temp;
            break;
        }
    }
    return ret;
}

/**
 * @brief Engine::getHardwareByMac Hole ein Hardware-Object anhand seiner MAC
 * @param mac Gesuchte MAC
 * @return Hardware-Objekt mit der gewünschten MAC, wenn nicht gefunden nullptr
 */
Hardware *Engine::getHardwareByMac(QString mac)
{
    Hardware *ret = nullptr;
    for (int i = 0 ; i < m_hardwareList.size() ; i++){
        Hardware *temp = (Hardware*) m_hardwareList.at(i);
        if (temp->mac() == mac) {
            ret = temp;
            break;
        }
    }
    return ret;
}

/**
 * @brief Engine::getContentById Hole ein Content-Object anhand seiner ID
 * @param id Gesuchte ID
 * @return Content-Object mit der gewünschten ID, wenn nicht gedfunden nullptr
 */
Content *Engine::getContentById(int id)
{
    Content *ret = nullptr;
    for (int i = 0 ; i < m_contentList.size() ; i++){
        Content *temp = (Content*) m_contentList.at(i);
        if (temp->id() == id) {
            ret = temp;
            break;
        }
    }
    return ret;
}

/**
 * @brief Engine::getPlaylistById Hole ein Playlisten-Object anhand seiner ID
 * @param id Gesuchte ID
 * @return Playlisten-Object mit der gewünschten ID, wenn nicht gedfunden nullptr
 */
Playlist *Engine::getPlaylistById(int id)
{
    Playlist *ret = nullptr;
    for (int i = 0 ; i < m_playlistList.size() ; i++){
        Playlist *temp = (Playlist*) m_playlistList.at(i);
        if (temp->id() == id) {
            ret = temp;
            break;
        }
    }
    return ret;
}

/**
 * @brief Engine::sortContent Sortiere die ContentListe anhand eines Types
 * @param sortType Typ = Attribut anhand welches sortiert werden soll
 * @param sortOrder Aufsteigend/Absteigend
 */
void Engine::sortContent(int sortType, int sortOrder)
{
    if (m_contentList.isEmpty())
        return;

    QList<Content*> sortList;
    for (int i = 0 ; i < m_contentList.count() ; i ++) {
        Content *temp = (Content*) m_contentList.at(i);
        sortList.append(temp);
    }

    std::sort(sortList.begin(), sortList.end(),
              [sortType,sortOrder](const Content* a, const Content* b) -> bool
    {
        switch(sortType) {
        case SortContent::CID:
        {
            if (sortOrder == SortOrder::UP)
                return a->id() < b->id();
            if (sortOrder == SortOrder::DOWN)
                return a->id() > b->id();
        }
            break;
        case SortContent::CNAME:
        {
            if (sortOrder == SortOrder::UP)
                return a->name() < b->name();
            if (sortOrder == SortOrder::DOWN)
                return a->name() > b->name();
        }
            break;
        case SortContent::CTYPE:
        {
            if (sortOrder == SortOrder::UP)
                return a->fileType() < b->fileType();
            if (sortOrder == SortOrder::DOWN)
                return a->fileType() > b->fileType();
        }
            break;
        default:
            break;
        }
        return false;
    });

    m_contentList.clear();
    for (int i = 0 ; i < sortList.count() ; i ++) {
        m_contentList.append(sortList.at(i));
    }

    emit contentListChanged(m_contentList);
}

/**
 * @brief Engine::sortGroups Sortiere die Gruppen-Liste anhand eines Types
 * @param sortType Typ = Attribut anhand welches sortiert werden soll
 * @param sortOrder Aufsteigend/Absteigend
 */
void Engine::sortGroups(int sortType, int sortOrder)
{
    if (m_groupsList.isEmpty())
        return;

    QList<Groups*> sortList;
    for (int i = 0 ; i < m_groupsList.count() ; i ++) {
        Groups *temp = (Groups*) m_groupsList.at(i);
        sortList.append(temp);
    }

    std::sort(sortList.begin(), sortList.end(),
              [sortType,sortOrder](const Groups* a, const Groups* b) -> bool
    {
        switch(sortType) {
        case SortGroups::GID:
        {
            if (sortOrder == SortOrder::UP)
                return a->id() < b->id();
            if (sortOrder == SortOrder::DOWN)
                return a->id() > b->id();
        }
            break;
        case SortGroups::GNAME:
        {
            if (sortOrder == SortOrder::UP)
                return a->groupName() < b->groupName();
            if (sortOrder == SortOrder::DOWN)
                return a->groupName() > b->groupName();
        }
            break;
        case SortGroups::GTYPE:
        {
            if (sortOrder == SortOrder::UP)
                return a->groupType() < b->groupType();
            if (sortOrder == SortOrder::DOWN)
                return a->groupType() > b->groupType();
        }
            break;
        case SortGroups::GCOUNT:
        {
            if (sortOrder == SortOrder::UP)
                return a->userInGroup() < b->userInGroup();
            if (sortOrder == SortOrder::DOWN)
                return a->userInGroup() > b->userInGroup();
        }
            break;
        default:
            break;
        }
        return false;
    });

    m_groupsList.clear();
    for (int i = 0 ; i < sortList.count() ; i ++) {
        m_groupsList.append(sortList.at(i));
    }

    emit groupsListChanged(m_groupsList);
}

/**
 * @brief Engine::sortUsers Sortiere die Benutzer-Liste anhand eines Types
 * @param sortType Typ = Attribut anhand welches sortiert werden soll
 * @param sortOrder Aufsteigend/Absteigend
 */
void Engine::sortUsers(int sortType, int sortOrder)
{
    if (m_groupsList.isEmpty() || groupsIdx() < 0 || groupsIdx() > m_groupsList.count())
        return;

    Groups *group = (Groups*) m_groupsList.at(groupsIdx());
    QList<Users*> sortList;
    for (int i = 0 ; i < group->usersList().count() ; i ++) {
        Users *temp = (Users*)  group->usersList().at(i);
        sortList.append(temp);
    }

    std::sort(sortList.begin(), sortList.end(),
              [sortType,sortOrder](const Users* a, const Users* b) -> bool
    {
        switch(sortType) {
        case SortUsers::UID:
        {
            if (sortOrder == SortOrder::UP)
                return a->id() < b->id();
            if (sortOrder == SortOrder::DOWN)
                return a->id() > b->id();
        }
            break;
        case SortUsers::CFNAME:
        {
            if (sortOrder == SortOrder::UP)
                return a->firstName() < b->firstName();
            if (sortOrder == SortOrder::DOWN)
                return a->firstName() > b->firstName();
        }
            break;
        case SortUsers::CLNAME:
        {
            if (sortOrder == SortOrder::UP)
                return a->lastName() < b->lastName();
            if (sortOrder == SortOrder::DOWN)
                return a->lastName() > b->lastName();
        }
            break;
        case SortUsers::CMAIL:
        {
            if (sortOrder == SortOrder::UP)
                return a->eMail() < b->eMail();
            if (sortOrder == SortOrder::DOWN)
                return a->eMail() > b->eMail();
        }
            break;
        case SortUsers::CGNAME:
        {
            if (sortOrder == SortOrder::UP)
                return a->groupName() < b->groupName();
            if (sortOrder == SortOrder::DOWN)
                return a->groupName() > b->groupName();
        }
            break;
        default:
            break;
        }
        return false;
    });

    group->clearUserList();
    for (int i = 0 ; i < sortList.count() ; i ++) {
        group->addUser(sortList.at(i));
    }
}

/**
 * @brief Engine::sortPlaylists Sortiere die Playlisten-Liste anhand eines Types
 * @param sortType Typ = Attribut anhand welches sortiert werden soll
 * @param sortOrder Aufsteigend/Absteigend
 */
void Engine::sortPlaylists(int sortType, int sortOrder)
{
    if (m_playlistList.isEmpty())
        return;

    QList<Playlist*> sortList;
    for (int i = 0 ; i < m_playlistList.count() ; i ++) {
        Playlist *temp = (Playlist*) m_playlistList.at(i);
        sortList.append(temp);
    }

    std::sort(sortList.begin(), sortList.end(),
              [sortType,sortOrder](const Playlist* a, const Playlist* b) -> bool
    {
        switch(sortType) {
        case SortPlaylist::PLID:
        {
            if (sortOrder == SortOrder::UP)
                return a->id() < b->id();
            if (sortOrder == SortOrder::DOWN)
                return a->id() > b->id();
        }
            break;
        case SortPlaylist::PLNAME:
        {
            if (sortOrder == SortOrder::UP)
                return a->name() < b->name();
            if (sortOrder == SortOrder::DOWN)
                return a->name() > b->name();
        }
            break;
        case SortPlaylist::PLGNAME:
        {
            if (sortOrder == SortOrder::UP)
                return a->groupName() < b->groupName();
            if (sortOrder == SortOrder::DOWN)
                return a->groupName() > b->groupName();
        }
            break;
        case SortPlaylist::PLORIE:
        {
            if (sortOrder == SortOrder::UP)
                return a->orientation() < b->orientation();
            if (sortOrder == SortOrder::DOWN)
                return a->orientation() > b->orientation();
        }
            break;
        case SortPlaylist::PLCOUNT:
        {
            if (sortOrder == SortOrder::UP)
                return a->planningsInPlaylist() < b->planningsInPlaylist();
            if (sortOrder == SortOrder::DOWN)
                return a->planningsInPlaylist() > b->planningsInPlaylist();
        }
            break;
        default:
            break;
        }
        return false;
    });

    m_playlistList.clear();
    for (int i = 0 ; i < sortList.count() ; i ++) {
        m_playlistList.append(sortList.at(i));
    }

    emit playlistListChanged(m_playlistList);
}

/**
 * @brief Engine::sortPlanning Sortiere die Planungs-Liste anhand eines Types
 * @param sortType Typ = Attribut anhand welches sortiert werden soll
 * @param sortOrder Aufsteigend/Absteigend
 */
void Engine::sortPlanning(int sortType, int sortOrder)
{
    if ( (playlistIdx() >= 0) && (playlistIdx() < m_playlistList.count() ) ) {
        Playlist *tempPlaylist = (Playlist*) m_playlistList.at(playlistIdx());

        if (tempPlaylist->planningList().isEmpty())
            return;

        QList<Planning*> sortList;
        for (int i = 0 ; i < tempPlaylist->planningList().count() ; i ++) {
            Planning *tempPlanning = (Planning*) tempPlaylist->planningList().at(i);
            sortList.append(tempPlanning);
        }

        std::sort(sortList.begin(), sortList.end(),
                  [sortType,sortOrder](const Planning* a, const Planning* b) -> bool
        {
            switch(sortType) {
            case SortPlanning::PID:
            {
                if (sortOrder == SortOrder::UP)
                    return a->id_order() < b->id_order();
                if (sortOrder == SortOrder::DOWN)
                    return a->id_order() > b->id_order();
            }
                break;
            case SortPlanning::PNAME:
            {
                if (sortOrder == SortOrder::UP)
                    return a->contentName() < b->contentName();
                if (sortOrder == SortOrder::DOWN)
                    return a->contentName() > b->contentName();
            }
                break;
            case SortPlanning::PVP:
            {
                if (sortOrder == SortOrder::UP)
                    return a->validPeriod() < b->validPeriod();
                if (sortOrder == SortOrder::DOWN)
                    return a->validPeriod() > b->validPeriod();
            }
                break;
            case SortPlanning::PWEEK:
            {
                if (sortOrder == SortOrder::UP)
                    return a->weekdays() < b->weekdays();
                if (sortOrder == SortOrder::DOWN)
                    return a->weekdays() > b->weekdays();
            }
                break;
            case SortPlanning::PVT:
            {
                if (sortOrder == SortOrder::UP)
                    return a->validTime() < b->validTime();
                if (sortOrder == SortOrder::DOWN)
                    return a->validTime() > b->validTime();
            }
                break;
            case SortPlanning::PTYPE:
            {
                if (sortOrder == SortOrder::UP)
                    return a->type() < b->type();
                if (sortOrder == SortOrder::DOWN)
                    return a->type() > b->type();
            }
                break;
            default:
                break;
            }
            return false;
        });

        tempPlaylist->clearPlanningList();
        for (int i = 0 ; i < sortList.count() ; i ++) {
            tempPlaylist->addPlanning(sortList.at(i));
        }

        emit tempPlaylist->planningListChanged(tempPlaylist->planningList());
    }
}

/**
 * @brief Engine::sortHardware Sortiere die Hardware-Liste anhand eines Types
 * @param sortType Typ = Attribut anhand welches sortiert werden soll
 * @param sortOrder Aufsteigend/Absteigend
 */
void Engine::sortHardware(int sortType, int sortOrder)
{
    if (m_hardwareList.isEmpty())
        return;

    QList<Hardware*> sortList;
    for (int i = 0 ; i < m_hardwareList.count() ; i ++) {
        Hardware *temp = (Hardware*) m_hardwareList.at(i);
        sortList.append(temp);
    }

    std::sort(sortList.begin(), sortList.end(),
              [sortType,sortOrder](const Hardware* a, const Hardware* b) -> bool
    {
        switch(sortType) {
        case SortHardware::MAC:
        {
            if (sortOrder == SortOrder::UP)
                return a->mac() < b->mac();
            if (sortOrder == SortOrder::DOWN)
                return a->mac() > b->mac();
        }
            break;
        case SortHardware::LOCATION:
        {
            if (sortOrder == SortOrder::UP)
                return a->location() < b->location();
            if (sortOrder == SortOrder::DOWN)
                return a->location() > b->location();
        }
            break;
        case SortHardware::VERSION:
        {
            if (sortOrder == SortOrder::UP)
                return a->version() < b->version();
            if (sortOrder == SortOrder::DOWN)
                return a->version() > b->version();
        }
            break;
        case SortHardware::HGNAME:
        {
            if (sortOrder == SortOrder::UP)
                return a->groupName() < b->groupName();
            if (sortOrder == SortOrder::DOWN)
                return a->groupName() > b->groupName();
        }
            break;
        default:
            break;
        }
        return false;
    });

    m_hardwareList.clear();
    for (int i = 0 ; i < sortList.count() ; i ++) {
        m_hardwareList.append(sortList.at(i));
    }

    emit hardwareListChanged(m_hardwareList);
}

/**
 * @brief Engine::getContentListBACKUP Hole die Backup-Liste des Contents
 * @return
 */
QList<QObject *> Engine::getContentListBACKUP() const
{
    return m_contentListBACKUP;
}

/**
 * @brief Engine::setContentListBACKUP Setze die Backup-Liste des Contents
 * @param contentListBACKUP
 */
void Engine::setContentListBACKUP(const QList<QObject *> &contentListBACKUP)
{
    m_contentListBACKUP = contentListBACKUP;
}

/**
 * @brief Engine::setIdent Setze den Ident des Benutzers
 * @param ident Ident des Benutzers
 */
void Engine::setIdent(QString ident)
{
    if (m_ident == ident)
        return;

    m_ident = ident;
    emit identChanged(m_ident);
}

/**
 * @brief Engine::setPassword Setze das Passwort des Benutzers
 * @param password Passwort des Benutzers
 */
void Engine::setPassword(QString password)
{
    if (m_password == password)
        return;

    m_password = password;
    emit passwordChanged(m_password);
}


