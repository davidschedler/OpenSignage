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

#include "include/manager.h"

/**
 * @brief Manager::Manager Default-Konstruktor
 */
Manager::Manager() :
    m_config(nullptr),
    m_md5Worker(nullptr)
{

}

/**
 * @brief Manager::requestPlaylistFromMac Hilfs-Funktion um sich am CMS zu authentifizieren
 */
void Manager::requestPlaylistFromMac()
{
    QByteArray data = generateByteMessage("GETDATA","AUTH_PI","");

    CustomEvent cEvent;
    cEvent.addCustomEvent(SENDER,"MANAGER");
    cEvent.addCustomEvent(COMMAND,"GETDATA");
    cEvent.addCustomEvent(TYPE,"AUTH_PI");
    cEvent.addCustomEvent(IDENT,m_config->getMac());
    cEvent.addCustomEvent(DATA,data);
    corelibrary->addEvent(&cEvent);
}

/**
 * @brief Manager::checkThis Debug Funktion um "THIS" zu prüfen
 */
void Manager::checkThis()
{
    qDebug() << "HIER";
}

/**
 * @brief Manager::generateJsonDocument Generiere ein Json-Dokument für die Kommunikation mit anderen Plugins
 * @param command Anweisung für das Plugin
 * @param array Arbeitsauftrag für das Plugin
 * @param data Daten für das Plugin
 * @return erzeugtes Json-Dokument
 */
QJsonDocument Manager::generateJsonDocument(QString command, QString array, QList<QStringList> data)
{
    QJsonObject jsonMainObj;
    jsonMainObj.insert("COMMAND", QJsonValue::fromVariant(command));

    QJsonArray jsonAr;
    foreach (QStringList singleData, data) {
        QJsonObject singleObj;
        foreach (QString singleInfo, singleData) {
            QStringList split = singleInfo.split("=");
            singleObj.insert(split.first(),split.last());
        }
        jsonAr.push_back(singleObj);
    }
    jsonMainObj.insert(array, jsonAr);

    QJsonDocument jDoc(jsonMainObj);
    qInfo() << jDoc.toJson();
    return jDoc;
}

/**
 * @brief Manager::requestPlayliste Frage Playliste am CMS an. Prüfe dabei im Vorfeld die Internetverbindung wenn keine
 * vorhanden nutze falls vorhanden die locale Playliste der alten Anfrage.
 */
void Manager::requestPlayliste()
{
    QProcess proc;
    QString program = "ping";
    QStringList args;
    args << "-c 1" << "www.google.com";
    proc.start(program, args);
    proc.waitForFinished(1000);
    QString reply = QString(proc.readAll());
    int exitCode = proc.exitCode();

    qDebug() << get(NAME) + "Ping finished! reply=" + reply + " exitCode=" + QString::number(exitCode);

    if (exitCode == 0) {
        qDebug() << get(NAME) + " network online get playlist ";

        QByteArray data = generateByteMessage("GETDATA","AUTH_PI","");
        CustomEvent cEvent;
        cEvent.addCustomEvent(SENDER,"MANAGER");
        cEvent.addCustomEvent(COMMAND,"GETDATA");
        cEvent.addCustomEvent(TYPE,"AUTH_PI");
        cEvent.addCustomEvent(IDENT,m_config->getMac());
        cEvent.addCustomEvent(DATA,data);
        corelibrary->addEvent(&cEvent);
    }
    else {
        qDebug() << get(NAME) + " network offline check local playlist ";

        if (QFile::exists(corelibrary->getAppDir().absolutePath() + "/playlist/playlist.ini")) {
            QFile jsonFile(corelibrary->getAppDir().absolutePath() + "/playlist/playlist.ini");
            jsonFile.open(QFile::ReadOnly | QIODevice::Text);
            QByteArray message = jsonFile.readAll();
            jsonFile.close();
            newCommunicationDataToParse(message);
        }
        else {

        }
    }
}

/**
 * @brief Manager::generateByteMessageGeneriere ein Json-Dokument für die Kommunikation mit anderen Plugins
 * @param command Anweisung für das Plugin
 * @param array Arbeitsauftrag für das Plugin
 * @param data Daten für das Plugin
 * @return erzeugtes Json-Dokument
 */
QByteArray Manager::generateByteMessage(QString command, QString type, QString data)
{
    QJsonObject jsonMainObj;
    jsonMainObj.insert("COMMAND", QJsonValue::fromVariant(command));
    jsonMainObj.insert("TYPE", QJsonValue::fromVariant(type));
    jsonMainObj.insert("IDENT", QJsonValue::fromVariant(m_config->getMac()));

    if (data != "") {
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
    return message;
}

void Manager::newCommunicationDataToParse(QByteArray newData)
{
    QJsonParseError *err = new QJsonParseError();
    QJsonDocument jDoc = QJsonDocument::fromJson(newData, err);
    qInfo() << get(NAME) + " Binary Message received from server! Start parsing";


    if (err->error != 0)
    {
        qDebug() << get(NAME) + " QJsonParserError: " + err->errorString();
    }
    if (jDoc.isNull())
    {
        qDebug() << get(NAME) + " QJsonDocument is null.";
        return;
    }
    else if (jDoc.isEmpty())
    {
        qDebug() << get(NAME) + " QJsonDocument is empty.";
        return;
    }
    else if (jDoc.isObject())
    {
        QJsonObject mainObj = jDoc.object();
        QVariantMap mainMap = mainObj.toVariantMap();

        if (!mainObj.contains("COMMAND"))
        {
            qDebug() << get(NAME) + "Json does not contains command...";
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

                        // HIER IST CLIENT AUTHENTIFIZIERT DAHER HOLE NEUE PLAYLISTE
                        QByteArray data = generateByteMessage("GETDATA","CLIENT_PLAYLIST","");

                        CustomEvent cEvent;
                        cEvent.addCustomEvent(SENDER,"MANAGER");
                        cEvent.addCustomEvent(COMMAND,"GETDATA");
                        cEvent.addCustomEvent(TYPE,"CLIENT_PLAYLIST");
                        cEvent.addCustomEvent(IDENT,m_config->getMac());
                        cEvent.addCustomEvent(DATA,data);
                        corelibrary->addEvent(&cEvent);
                    }
                    else {

                        // HIER IST CLIENT NICHT AUTHENTIFIZIERT (GGF KEIN INTERNET)
                        // HOLE LETZTE RSS AUS ORDNER PARSE DIESE

                        if (QFile::exists(corelibrary->getAppDir().absolutePath() + "/playlist/playlist.ini")) {
                            QFile jsonFile(corelibrary->getAppDir().absolutePath() + "/playlist/playlist.ini");
                            jsonFile.open(QFile::ReadOnly | QIODevice::Text);
                            QByteArray message = jsonFile.readAll();
                            jsonFile.close();
                            newCommunicationDataToParse(message);
                        }
                    }
                }
            }
        }

        if (mainObj.contains("CLIENT_PLAYLIST")) {
            //speichere aktuelle rss ab falls internet mal nicht geht um backup zu haben
            QFile jsonFile(corelibrary->getAppDir().absolutePath() + "/playlist/playlist.ini");
            jsonFile.open(QIODevice::WriteOnly | QIODevice::Text);
            jsonFile.write(jDoc.toJson());
            jsonFile.close();

            //clear
            m_newParsedPlaylist.clear();
            m_playlistToCheckMD5.clear();
            m_playlistToDownload.clear();

            //FANGE AN PLAYLISTE AUSZUWERTEN
            QVariantList dataList = mainMap.value("CLIENT_PLAYLIST").toList();
            QString playlistName, orientation, id_content, id_planning, validPeriod, weekdays, validTime, type, contentName, contentmd5, id_order, duration;
            int durationInt = 0;

            for (int i = 0; i < dataList.count(); i++){
                durationInt = 0;
                QVariantMap dataMap = dataList.at(i).toMap();
                if (dataMap.contains("PlaylistName")){
                    playlistName = dataMap.value("PlaylistName").toString();
                }
                if (dataMap.contains("PlaylistOrientation")){
                    orientation = dataMap.value("PlaylistOrientation").toString();
                    QProcess proc;
                    QString program = "xrandr";
                    QStringList args;
                    if (orientation == "PANORAMA")
                        args << "-o" << "normal";
                    if (orientation == "PORTRAIT")
                        args << "-o" << "left";
                    proc.start(program, args);
                    proc.waitForFinished(1000);
                    QString reply = QString(proc.readAll());
                    int exitCode = proc.exitCode();
                }

                if (dataMap.contains("ID_Order")){
                    id_order = dataMap.value("ID_Order").toString();
                }
                if (dataMap.contains("ID_Planning")){
                    id_planning = dataMap.value("ID_Planning").toString();
                }
                if (dataMap.contains("Type")){
                    type = dataMap.value("Type").toString();
                }

                if (dataMap.contains("Weekdays")){
                    weekdays = dataMap.value("Weekdays").toString();
                }
                if (dataMap.contains("Valid_Period")){
                    validPeriod = dataMap.value("Valid_Period").toString();
                }
                if (dataMap.contains("Valid_Time")){
                    validTime = dataMap.value("Valid_Time").toString();
                }

                if (dataMap.contains("ID_Content")){
                    id_content = dataMap.value("ID_Content").toString();
                }
                if (dataMap.contains("ContentName")){
                    contentName = dataMap.value("ContentName").toString();
                }
                if (dataMap.contains("ContentMD5")){
                    contentmd5 = dataMap.value("ContentMD5").toString();
                }
                if (dataMap.contains("DurationPlanning")){
                    durationInt = dataMap.value("DurationPlanning").toInt();
                }

                if (durationInt > -1 && !id_planning.isEmpty() && !id_content.isEmpty() && !validPeriod.isEmpty() && !weekdays.isEmpty() && !id_order.isEmpty() &&
                        !validTime.isEmpty() && !type.isEmpty() && !contentName.isEmpty() && !contentmd5.isEmpty()) {

                    //FALL 1: Item ist bereits added von alter Playliste dann update nur Attribute
                    PlaylistItem *itemFound = getPlaylistItemByID(id_planning.toInt());
                    if (itemFound != nullptr) {
                        itemFound->setId_content(id_content.toInt());
                        itemFound->setValidPeriod(validPeriod);
                        itemFound->setWeekdays(weekdays);
                        itemFound->setValidTime(validTime);
                        itemFound->setType(type);
                        itemFound->setContentName(contentName);
                        itemFound->setId_order(id_order.toInt());
                        itemFound->setContentMD5(contentmd5);
                        itemFound->setDuration(durationInt);

                        // Falls MD5 sich geändert hat muss item zur sicherheit nochmal durch den md5check
                        if (!itemFound->isMD5Correct()) {
                            itemFound->setIsMD5Correct(false);
                            //füge zu liste die gecheckt werden muss hinzu
                            m_playlistToCheckMD5.append(itemFound);
                        }
                        //Falls Object noch nicht downloaded muss item heruntergeladen werden
                        if (!itemFound->isDownloaded())  {
                            itemFound->setIsDownloaded(false);
                            //füge zu liste die gedownloaded werden muss
                            m_playlistToDownload.append(itemFound);
                        }

                        //da update bereits in finaler playlist drin adde zu newparsed
                        m_newParsedPlaylist.append(itemFound);
                        continue;
                    }

                    //Fall 2: Item ist noch nicht vorhanten, daher adde Item
                    PlaylistItem *newPlaylist = new PlaylistItem(id_content.toInt(),validPeriod,weekdays,validTime,type,contentName,contentmd5,id_order.toInt(),id_planning.toInt(),durationInt,this);
                    //add to final, newparsed und tocheckmd5 list
                    m_playlist.append(newPlaylist);
                    m_newParsedPlaylist.append(newPlaylist);
                    m_playlistToCheckMD5.append(newPlaylist);
                    m_playlistToDownload.append(newPlaylist);
                }
            }
            //Räume Content auf, der in alten Playliste war und nun nicht mehr vorhanden ist! (LÖSCHEN)
            QList<QObject*> copy = m_playlist;
            for ( int i = 0 ; i < copy.count() ; i ++) {
                bool shouldDelete = true;
                PlaylistItem *checkItem = (PlaylistItem*) copy.at(i);

                for ( int j = 0 ; j < m_newParsedPlaylist.count() ; j++) {
                    PlaylistItem *oldItems = (PlaylistItem*) m_newParsedPlaylist.at(j);
                    if (checkItem->id_planning() == oldItems->id_planning()) {
                        shouldDelete = false; // wenn gefunden nicht löschen ist neu
                        break;
                    }
                }

                if (shouldDelete) {
                    QFile delFile(corelibrary->getAppDir().absolutePath() + "/content_downloaded/" + checkItem->contentName());
                    if (delFile.exists()) {
                        delFile.remove();
                    }
                    checkItem->deleteLater();
                    m_playlist.removeAt(i);
                    copy = m_playlist;
                    i = 0;
                }
            }
            //räume ordner auf nur verplanter content draf hier drin liegen
            QDir dir(corelibrary->getAppDir().absolutePath() + "/content_downloaded/");
            dir.setNameFilters(QStringList("*.*"));
            dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
            QStringList filesToCheck;
            QStringList fileList = dir.entryList();

            for (int i = 0; i < fileList.count(); i++)
            {
                bool shouldDelete = true;
                for ( int j = 0 ; j < m_playlist.count() ; j++) {
                    PlaylistItem *checkItem = (PlaylistItem*) m_playlist.at(j);
                    if (checkItem->contentName() == fileList.at(i)) {
                        shouldDelete = false; // wenn gefunden nicht löschen stammt aus playliste
                        break;
                    }
                }

                if (shouldDelete) {
                    QFile delFile(corelibrary->getAppDir().absolutePath() + "/content_downloaded/" + fileList.at(i));
                    if (delFile.exists()) {
                        delFile.remove();
                    }
                }
            }
            //sortiere liste nach order
            QList<PlaylistItem*> sortList;
            for (int i = 0 ; i < m_playlist.count() ; i++) {
                PlaylistItem *temp = (PlaylistItem*) m_playlist.at(i);
                sortList.append(temp);
            }
            std::sort(sortList.begin(), sortList.end(),
                      [](const PlaylistItem* a, const PlaylistItem* b) -> bool
            {
                return a->id_order() < b->id_order();
            });
            m_playlist.clear();
            for (int j = 0 ; j < sortList.count() ; j ++) {
                m_playlist.append(sortList.at(j));
            }
            //das erst item los shcicken rest macht der slot
            if (!m_playlistToDownload.isEmpty()) {
                //cleanup
                QList<QObject*> copy = m_playlistToDownload;
                for ( int i = 0 ; i < copy.count() ; i++) {
                    PlaylistItem *firstItem = (PlaylistItem*) copy.at(i);
                    if (QFile::exists(corelibrary->getAppDir().absolutePath() + "/content_downloaded/" + firstItem->contentName())) {

                        for (int j = 0 ; j < m_playlistToDownload.count() ; j ++) {
                            PlaylistItem *matchItem = (PlaylistItem*) m_playlistToDownload.at(j);
                            if (matchItem->id_planning() == firstItem->id_planning()) {
                                matchItem->setIsDownloaded(true);
                                m_playlistToDownload.removeAt(j);
                                //                                copy = m_playlistToDownload;
                                //                                i = 0;
                                break;
                            }
                        }
                    }
                }
                copy = m_playlistToDownload;
                // nichts zum downloaden alles bereits auf dem raspi
                if (copy.isEmpty()) {
                    //das erste item los schicken rest macht der slot (befüllt nach antwort weiter)
                    if (!m_playlistToCheckMD5.isEmpty()) {
                        //cleanup
                        QList<QObject*> copy = m_playlistToCheckMD5;
                        for ( int i = 0 ; i < copy.count() ; i++) {
                            PlaylistItem *firstItem = (PlaylistItem*) copy.at(i);
                            if (firstItem->isMD5Correct()) {
                                m_playlistToCheckMD5.removeAt(i);
                                //                                copy = m_playlistToCheckMD5;
                                //                                i = 0;
                            }
                        }
                        copy = m_playlistToCheckMD5;
                        //initial start
                        for ( int j = 0 ; j < copy.count() ; j++) {
                            PlaylistItem *firstItem = (PlaylistItem*) copy.at(j);
                            if (!firstItem->isMD5Correct()) {
                                emit checkMD5Sum(QString::number(firstItem->id_content()),corelibrary->getAppDir().absolutePath() + "/content_downloaded/" + firstItem->contentName());
                                break;
                            }
                        }
                    }
                }
                else {
                    for ( int j = 0 ; j < copy.count() ; j++) {
                        PlaylistItem *firstItem = (PlaylistItem*) copy.at(j);

                        if (!QFile::exists(corelibrary->getAppDir().absolutePath() + "/content_downloaded/" + firstItem->contentName())) {

                            QString fileName = "ID_CONTENT=" + QString::number(firstItem->id_content());
                            QByteArray data = generateByteMessage("GETDATA","DOWNLOAD_URL",fileName);

                            CustomEvent cEvent;
                            cEvent.addCustomEvent(SENDER,"MANAGER");
                            cEvent.addCustomEvent(COMMAND,"GETDATA");
                            cEvent.addCustomEvent(TYPE,"DOWNLOAD_URL");
                            cEvent.addCustomEvent(IDENT,m_config->getMac());
                            cEvent.addCustomEvent(DATA,data);
                            corelibrary->addEvent(&cEvent);
                            break;
                        }
                    }
                }
            }
            playerStateMachine();
        }
    }
    qInfo() << get(NAME) + "PARSING DONE";
}

/**
 * @brief Manager::newDownloadUrl Verarbeite eingehende neue Download-URL indem der Download gestartet wird
 * @param event eingehendes benutzerdefiniertes Event mit einem neuen Download-Link
 */
void Manager::newDownloadUrl(CustomEvent event)
{    
    if (!event.getCustomEvent(COMMAND).isEmpty())
    {
        QString command = event.getCustomEvent(COMMAND);
        QString type = event.getCustomEvent(TYPE);
        QString data = event.getCustomEvent(DATA);
        QString url;

        if (!data.isEmpty()) {
            url = data.mid(data.lastIndexOf("http://"));
            url = url.left(url.lastIndexOf("\""));
            url = url.left(url.lastIndexOf("\""));
        }

        qInfo() << get(NAME) + " new Download-Url start download from: " + url;

        CustomEvent cEvent;
        cEvent.addCustomEvent(SENDER,"MANAGER");
        cEvent.addCustomEvent(COMMAND,"START_DOWNLOAD");
        cEvent.addCustomEvent(TYPE,url);
        cEvent.addCustomEvent(IDENT,m_config->getMac());
        cEvent.addCustomEvent(DATA,"");
        corelibrary->addEvent(&cEvent);
    }
}

/**
 * @brief Manager::get Getter für pluginspezifische Informationen
 * @param type Typ der Anfrage (ID,NAME,TYPE,SETTINGS,VERSION)
 * @return Wert der Anfrage
 */
QString Manager::get(int type) const
{
    switch (type) {
    case PID:
        return "0000";
    case NAME:
        return "Manager";
    case PTYPE:
        return "RASPI";
    case PSETTINGS:
        return corelibrary->getAppDir().absolutePath() + "/settings/settings.ini";
    case VERSION:
        return "VERSION_1.0.0";
    default:
        break;
    }
    return "";
}

/**
 * @brief Manager::onInit FirstInit => Abonniere die Events von der Corelibrary und lade die Settings. Speichere diese in die CommunicationServerConfig.
 * ExtendedInit => Thread für MD5 Erstellung
 * @param type PluginInterface-Defines
 * @return 0
 */
int Manager::onInit(int type)
{
    switch (type) {
    case FIRSTINIT:
    {
        /* plugin general */
        m_config = new ManagerConfig(this);
        corelibrary->subscribeEvent("INCOMMINGDATA", this);
        corelibrary->subscribeEvent("PUTDATA", this);
        corelibrary->subscribeEvent("FINISHED_DOWNLOAD", this);

        loadSettings();

        QList<QNetworkInterface> allInterfaces = QNetworkInterface::allInterfaces();
        foreach (QNetworkInterface singleInterface, allInterfaces) {

            if (singleInterface.name() == "eth0") {
                qDebug() << "NET-TYPE:" + singleInterface.name();
                qInfo() << "MAC ADDRESS:" + singleInterface.hardwareAddress();
                if (m_config->getMac() != singleInterface.hardwareAddress())
                    m_config->setMac(singleInterface.hardwareAddress());
            }
        }
        saveSettings();

        qInfo() << get(NAME) << "first init done!";
    }
        break;
    case EXTENDEDINIT:
    {
        /* player statemachine operations */
        m_playerState = READY;
        m_playerPosition = 0;

        m_playerStateTimer.setSingleShot(true);
        connect(&m_playerStateTimer, &QTimer::timeout, this, &Manager::playerStateMachine);
        connect(&m_refreshPlayliste, &QTimer::timeout, this, &Manager::requestPlayliste);
        m_refreshPlayliste.start(5 * 60 * 1000); // 15 mal 60 sekunden a 1000 ms => 15min

        m_contentThread = new QThread;
        m_md5Worker = new MD5Worker(corelibrary->getAppDir().absolutePath() + "/content_downloaded/");
        m_md5Worker->moveToThread(m_contentThread);
        //connect(contentWorker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
        connect(this, SIGNAL(checkMD5Sum(QString, QString)), m_md5Worker, SLOT(generateMD5Sum(QString,QString))); //worker befüllen
        connect(m_md5Worker, SIGNAL(readyGeneratedMD5(QString,QString,QString)), this, SLOT(incomingCheckedMD5(QString,QString,QString))); // woker antwort
        connect(m_contentThread, SIGNAL(started()), m_md5Worker, SLOT(process()));
        connect(m_md5Worker, SIGNAL(finished()), m_contentThread, SLOT(quit()));
        connect(m_md5Worker, SIGNAL(finished()), m_md5Worker, SLOT(deleteLater()));
        connect(m_contentThread, SIGNAL(finished()), m_contentThread, SLOT(deleteLater()));
        m_contentThread->start();

        //gernerall update interval
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(requestPlayliste()));
        timer->setInterval(m_config->getRefreshTimer().toInt() * 1000);
        timer->start();

        //initial update
        QTimer::singleShot(1000,this, SLOT(requestPlayliste()));

        qInfo() << get(NAME) << "extended init done!";
    }
        break;
    case EXIT:
        break;
    default:
        break;
    }
    return 0;
}

/**
 * @brief Manager::onEvent Verarbeite bzw. bearbeite die Events, welche von der Corelibrary abonniert wurden.
 * @param customEvent Benutzerdefiniertes Events inkl. Arbeitsanweisungen.
 * @return false
 */
bool Manager::onEvent(CustomEvent *customEvent)
{
    QString print;
    if (!customEvent->getCustomEvent(COMMAND).isEmpty())
    {
        QString command = customEvent->getCustomEvent(COMMAND);
        QString type = customEvent->getCustomEvent(TYPE);
        QString data = customEvent->getCustomEvent(DATA);
        if (command == "INCOMMINGDATA") {

            qInfo() << get(NAME) + " onEvent: " + "Command: " + command  + " Type: " + type;

            QByteArray byteMessage;
            if (type == "DATA") {
                byteMessage = customEvent->getCustomEvent(DATA).toUtf8().data();
                newCommunicationDataToParse(byteMessage);

            }
            if (type == "DOWNLOAD_URL") {
                byteMessage = customEvent->getCustomEvent(DATA).toUtf8().data();
                newDownloadUrl(customEvent);
            }
            qDebug() << get(NAME) + " onEvent: " + byteMessage;

        }
        if (command == "FINISHED_DOWNLOAD") {

            QString ident = customEvent->getCustomEvent(IDENT);
            ident = ident.mid(ident.lastIndexOf("/")+1);
            QByteArray byteMessage = customEvent->getCustomEvent(DATA).toUtf8().data();

            if (type == "SUCCESS") {
                QList<QObject*> copy = m_playlistToDownload;
                for (int i = 0 ; i < copy.count() ; i++) {
                    PlaylistItem *temp = (PlaylistItem*) copy.at(i);
                    if (temp->contentName() == ident) {
                        temp->setIsDownloaded(true);
                        m_playlistToDownload.removeAt(i);

                        qDebug() << get(NAME) + " start MD5Worker generating MD5 for: " + temp->contentName();
                        emit checkMD5Sum(QString::number(temp->id_content()),corelibrary->getAppDir().absolutePath() + "/content_downloaded/" + temp->contentName());
                        break;
                    }
                }
                if (!m_playlistToDownload.isEmpty()) {
                    PlaylistItem *firstItem = (PlaylistItem*) m_playlistToDownload.at(0);

                    qDebug() << get(NAME) + " start MD5Worker generating MD5 for: " + firstItem->contentName();

                    QString fileName = "ID_CONTENT=" + QString::number(firstItem->id_content());
                    QByteArray data = generateByteMessage("GETDATA","DOWNLOAD_URL",fileName);

                    CustomEvent cEvent;
                    cEvent.addCustomEvent(SENDER,"MANAGER");
                    cEvent.addCustomEvent(COMMAND,"GETDATA");
                    cEvent.addCustomEvent(TYPE,"DOWNLOAD_URL");
                    cEvent.addCustomEvent(IDENT,m_config->getMac());
                    cEvent.addCustomEvent(DATA,data);
                    corelibrary->addEvent(&cEvent);
                }
            }
            if (type == "ERROR") {
                QList<QObject*> copy = m_playlistToDownload;
                for (int i = 0 ; i < copy.count() ; i++) {
                    PlaylistItem *temp = (PlaylistItem*) copy.at(i);
                    if (temp->contentName() == ident) {
                        m_playlistToDownload.removeAt(i);
                        m_playlistToDownload.append(temp);
                        break;
                    }
                }
                if (!m_playlistToDownload.isEmpty()) {
                    PlaylistItem *firstItem = (PlaylistItem*) m_playlistToDownload.at(0);

                    QString fileName = "ID_CONTENT=" + QString::number(firstItem->id_content());
                    QByteArray data = generateByteMessage("GETDATA","DOWNLOAD_URL",fileName);

                    CustomEvent cEvent;
                    cEvent.addCustomEvent(SENDER,"MANAGER");
                    cEvent.addCustomEvent(COMMAND,"GETDATA");
                    cEvent.addCustomEvent(TYPE,"DOWNLOAD_URL");
                    cEvent.addCustomEvent(IDENT,m_config->getMac());
                    cEvent.addCustomEvent(DATA,data);
                    corelibrary->addEvent(&cEvent);
                }
            }
        }
    }
    return false;
}

/**
 * @brief Manager::loadSettings Lade Plugin-spezifische Einstellungen
 */
void Manager::loadSettings()
{
    settings = new QSettings(get(PSETTINGS),QSettings::IniFormat);

    settings->beginGroup(get(NAME));
    if (!settings->contains("mac"))
        settings->setValue("mac","XX:XX:XX:XX:XX:XX");
    if (!settings->contains("refreshTimer"))
        settings->setValue("refreshTimer","15");
    m_config->setMac(settings->value("mac").toString());

    settings->endGroup();

    qInfo() << get(NAME) + " Loaded settings: " + " MAC: " + m_config->getMac();

    settings->sync();
}

/**
 * @brief Manager::saveSettings Speichere Plugin-spezifische Einstellungen
 */
void Manager::saveSettings()
{
    settings->beginGroup(get(NAME));
    settings->setValue("mac",m_config->getMac());
    settings->setValue("refreshTimer",m_config->getRefreshTimer());
    settings->sync();
    settings->endGroup();

    qInfo() << get(NAME) + " Saved settings: " + " MAC: " + m_config->getMac();
}

/**
 * @brief Manager::setPlaylist Setze neue Playliste
 * @param playlist neue Playliste
 */
void Manager::setPlaylist(QList<QObject *> playlist)
{
    if (m_playlist == playlist)
        return;

    m_playlist = playlist;
    emit playlistChanged(m_playlist);
}

/**
 * @brief Manager::incomingCheckedMD5 Eingehender MD5 vom MD5Worker, falls MD5 korrekt setze Gültigkeit.
 * Ansonsten prüfe MD5 später noch einmal
 * @param id_content Content-ID
 * @param filePath Content-Pfad
 * @param md5Sum Content-MD5
 */
void Manager::incomingCheckedMD5(QString id_content, QString filePath, QString md5Sum)
{
    QList<QObject*> copy = m_playlistToCheckMD5;

    for (int i = 0 ; i < copy.count(); i ++) {
        PlaylistItem *checkMD5 = (PlaylistItem*) copy.at(i);
        QString name = filePath.mid(filePath.lastIndexOf("/")+1);
        if (name == checkMD5->contentName() && id_content.toInt() == checkMD5->id_content()) {
            if (md5Sum == checkMD5->contentMD5()) {
                checkMD5->setIsMD5Correct(true);
                m_playlistToCheckMD5.removeAt(i);
            } else {
                // hinten anhängen für nächste runde dann
                m_playlistToCheckMD5.append(checkMD5);
                m_playlistToCheckMD5.removeAt(i);
            }
        }
    }
    if (m_playlistToDownload.isEmpty() && !m_playlistToCheckMD5.isEmpty()) {
        QString nextFile;
        PlaylistItem *nextOne = nullptr;
        for (int j = 0 ; j < m_playlistToCheckMD5.count(); j ++) {
            nextOne = (PlaylistItem*) m_playlistToCheckMD5.at(j);
            if (!nextOne->isMD5Correct()) {
                nextFile = nextOne->contentName();
                break;
            }
        }
        if (nextOne != nullptr)
            emit checkMD5Sum(QString::number(nextOne->id_content()),corelibrary->getAppDir().absolutePath() + "/content_downloaded/" + nextOne->contentName());
    }
}

/**
 * @brief Manager::setCurrentPicture Setze aktuelles Bild
 * @param currentPicture aktuelles Bild
 */
void Manager::setCurrentPicture(QString currentPicture)
{
    if (m_currentPicture == currentPicture)
        return;

    m_currentPicture = currentPicture;
    emit currentPictureChanged(m_currentPicture);
}

/**
 * @brief Manager::playerStateMachine State-Maschine für das Abspielen des Contents
 * Dabei wird stets die Gültigkeit des Contents geprüft. Wenn dieser gültig ist wird er
 * abgespielt wenn nicht übersprungen.
 */
void Manager::playerStateMachine()
{
    bool validPlaylistItem = false;

    switch (m_playerState) {

    case WAIT:
        break;

    case READY:
    {
        // prüfe ob etwas zum abspielen exsistiert
        if (m_playlist.isEmpty())
            return;

        if (m_playerStateTimer.isActive()) {
            break;
        }

        for (int i = m_playerPosition ; i < m_playlist.count() ; i++) {
            PlaylistItem *item = (PlaylistItem *) m_playlist.at(i);
            qDebug() << get(NAME) + " validate PlaylistItem: " + item->contentName();
            if (item->isPlaylistItemValid()) {
                validPlaylistItem = true;
                //m_playerState = WAIT;

                qDebug() << get(NAME) + " validate done is valid!";

                if (item->isVideo()) {
                    CustomEvent cEvent;
                    cEvent.addCustomEvent(SENDER,"MANAGER");
                    cEvent.addCustomEvent(COMMAND,"PLAYER");
                    cEvent.addCustomEvent(TYPE,"PLAY");
                    cEvent.addCustomEvent(IDENT,item->contentName());
                    cEvent.addCustomEvent(DATA,"");
                    corelibrary->addEvent(&cEvent);
                    // setCurrentVideo(item->contentName());
                    //setCurrentPicture("");
                }
                else {

                    CustomEvent cEvent;
                    cEvent.addCustomEvent(SENDER,"MANAGER");
                    cEvent.addCustomEvent(COMMAND,"PLAYER");
                    cEvent.addCustomEvent(TYPE,"STOP");
                    cEvent.addCustomEvent(IDENT,"");
                    cEvent.addCustomEvent(DATA,"");
                    corelibrary->addEvent(&cEvent);
                    //setCurrentVideo(item->contentName());
                    qDebug() << "Display picture: " + corelibrary->getAppDir().absolutePath() + "/content_downloaded/" + item->contentName();
                    setCurrentPicture(corelibrary->getAppDir().absolutePath() + "/content_downloaded/" + item->contentName());
                }
                //start timer
                m_playerStateTimer.start(item->duration());
                m_playerPosition = i;

                if (m_playerPosition < m_playlist.count()-1)
                    m_playerPosition = m_playerPosition + 1;
                else
                    m_playerPosition = 0;

                break;
            }
            qDebug() << get(NAME) + " validate done is not valid...  checking next one in 500 ms";
        }
        if (!validPlaylistItem) {
            m_playerStateTimer.start(500);
        }
    }
        break;

    default:
        break;

    }
}

/**
 * @brief Manager::getPlaylistItemByID Hole PlaylistenItem anhand seiner ID
 * @param id_planning ID der Planung
 * @return true = Playlisten-Objekt, false = nullptr
 */
PlaylistItem *Manager::getPlaylistItemByID(int id_planning)
{
    PlaylistItem *ret = nullptr;
    for (int i = 0 ; i < m_playlist.count() ; i++ ) {
        PlaylistItem *searchItem = (PlaylistItem*) m_playlist.at(i);
        if (searchItem->id_planning() == id_planning) {
            ret = searchItem;
            break;
        }
    }
    return ret;
}


