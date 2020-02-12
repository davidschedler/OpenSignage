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

#include "database.h"

/**
 * @brief Database::Database Default-Konstruktor
 */
Database::Database()
{
}

/**
 * @brief Database::~Database Default-Destruktor
 */
Database::~Database()
{

}

/**
 * @brief Database::get Plugin-spezifischer Getter (PID, NAME, PTYPE, PSETTINGS, VERSION)
 * @param type PluginInterface-Defines
 * @return Plugin-spezifische Informationen (PID, NAME, PTYPE, PSETTINGS, VERSION)
 */
QString Database::get(int type) const
{
    switch (type) {
    case PID:
        return "0001";
    case NAME:
        return "Database";
    case PTYPE:
        return "PLUGIN";
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
 * @brief Database::onInit FirstInit => Abonniere die Events von der Corelibrary und lade die Settings. Speichere diese in die DatabaseConfig.
 * ExtendedInit => Prüfe ob Datenbank vorhanden ist, wenn ja lade diese, wenn nein erstelle eine und befülle diese mit den gewünschten Tabellen.
 * Überprüfe aktuellen Bestand der Datenbank und erstelle bzw. starte einen FileWatcher-Thread, dieser fügt der Datenbank den nicht bekannten
 * Content hinzu inkl. MD5-Überprüfung und Content-Informationen.
 * @param type PluginInterface-Defines
 * @return 0
 */
int Database::onInit(int type)
{
    switch (type) {
    case FIRSTINIT:
    {
        /* plugin general */
        m_config = new DatabaseConfig(this);
        corelibrary->subscribeEvent("GETDATA",this);
        corelibrary->subscribeEvent("INSERTENTRY",this);
        corelibrary->subscribeEvent("UPDATEENTRY",this);
        corelibrary->subscribeEvent("DELETEENTRY",this);
        corelibrary->subscribeEvent("GET_PI_PLAYLIST",this);

        loadSettings();
        qInfo() << get(NAME) << "first init done!";
    }
        break;
    case EXTENDEDINIT:
    {
        /* db operations */
        if (QFile::exists(m_config->dbPath())) {
            m_databaseGeneral.initDatabase(m_config->connectionName());
            m_databaseGeneral.loadDatabaseFromFile("file://" + m_config->dbPath());
        }
        else {
            m_databaseGeneral.initDatabase(m_config->connectionName());
            createInitialTables();
            testFillTables();
            m_databaseGeneral.saveDatabaseToFile(m_config->dbPath());
        }

        //Initialen Bestand prüfen
        QStringList contentFromDb;
        QSqlQuery q(m_databaseGeneral.getDB());
        QString querytext = QString("SELECT Content.path FROM Content;");
        q.prepare(querytext);

        if (!q.exec()) {
            qDebug() << get(NAME) + "Cannot execute query:" << querytext;
        }
        else{
            while (q.next()) {
                contentFromDb.append(q.value(0).toString());
            }
        }

        /* md5 operations */
        QString mainPath  = corelibrary->getAppDir().absolutePath() + "/content_uploaded/";
        m_fileWatcherThread = new QThread;
        m_fileWatcher = new FileWatcher(mainPath,contentFromDb);
        m_fileWatcher->moveToThread(m_fileWatcherThread);
        connect(m_fileWatcher, SIGNAL(detectedNewContent(QStringList)), this, SLOT(addNewContentToDB(QStringList)));
        connect(m_fileWatcherThread, SIGNAL(started()), m_fileWatcher, SLOT(process()));
        connect(m_fileWatcher, SIGNAL(finished()), m_fileWatcherThread, SLOT(quit()));
        connect(m_fileWatcher, SIGNAL(finished()), m_fileWatcher, SLOT(deleteLater()));
        connect(m_fileWatcherThread, SIGNAL(finished()), m_fileWatcherThread, SLOT(deleteLater()));
        m_fileWatcherThread->start();

        qInfo() << get(NAME) << "extended init done!";
    }
        break;
    case EXIT:
    {

    }
        break;
    default:
        break;
    }

    return 0;
}

/**
 * @brief Database::onEvent Verarbeite bzw. bearbeite die Events, welche von der Corelibrary abonniert wurden.
 * @param customEvent Benutzerdefiniertes Events inkl. Arbeitsanweisungen.
 * @return false
 */
bool Database::onEvent(CustomEvent *customEvent)
{
    QString print;

    if (!customEvent->getCustomEvent(COMMAND).isEmpty())
    {
        QString command = customEvent->getCustomEvent(COMMAND);
        QString type = customEvent->getCustomEvent(TYPE);
        if (command == "GETDATA") {

            qInfo() << get(NAME) + " onEvent: " + "Command: " + command  + " Type: " + type;

            //Für gui
            QString getDataResult;
            if (type == "AUTH") {
                QJsonDocument jDoc(generateJsonDocument("PUTDATA","AUTH",getAuth(customEvent->getCustomEvent(IDENT),customEvent->getCustomEvent(PWD))));
                getDataResult = QString(jDoc.toJson(QJsonDocument::Compact));
            }
            if (type == "AUTH_PI") {
                QJsonDocument jDoc(generateJsonDocument("PUTDATA","AUTH",getAuthPi(customEvent->getCustomEvent(IDENT))));
                getDataResult = QString(jDoc.toJson(QJsonDocument::Compact));
            }
            if (type == "GROUPS") {
                QJsonDocument jDoc(generateJsonDocument("PUTDATA","GROUPS",getGroups(customEvent->getCustomEvent(IDENT))));
                getDataResult = QString(jDoc.toJson(QJsonDocument::Compact));
                qDebug() << get(NAME) + getDataResult;
            }
            if (type == "USERS") {
                QJsonDocument jDoc(generateJsonDocument("PUTDATA","USERS",getUsers(customEvent->getCustomEvent(IDENT))));
                getDataResult = QString(jDoc.toJson(QJsonDocument::Compact));
                qDebug() << get(NAME) + getDataResult;
            }
            if (type == "HARDWARE") {
                QJsonDocument jDoc(generateJsonDocument("PUTDATA","HARDWARE",getHardware(customEvent->getCustomEvent(IDENT))));
                getDataResult = QString(jDoc.toJson(QJsonDocument::Compact));
                qDebug() << get(NAME) + getDataResult;
            }
            if (type == "CONTENT") {
                QJsonDocument jDoc(generateJsonDocument("PUTDATA","CONTENT",getContent(customEvent->getCustomEvent(IDENT))));
                getDataResult = QString(jDoc.toJson(QJsonDocument::Compact));
                qDebug() << get(NAME) + getDataResult;
            }
            if (type == "PLAYLISTS") {
                QJsonDocument jDoc(generateJsonDocument("PUTDATA","PLAYLISTS",getPlaylists(customEvent->getCustomEvent(IDENT))));
                getDataResult = QString(jDoc.toJson(QJsonDocument::Compact));
                qDebug() << get(NAME) + getDataResult;
            }
            if (type == "PLANNINGS") {
                QJsonDocument jDoc(generateJsonDocument("PUTDATA","PLANNINGS",getPlannings(customEvent->getCustomEvent(IDENT))));
                getDataResult = QString(jDoc.toJson(QJsonDocument::Compact));
                qDebug() << get(NAME) + getDataResult;
            }

            //Für raspi
            if (type == "CLIENT_PLAYLIST") {
                QJsonDocument jDoc(generateJsonDocument("PUTDATA","CLIENT_PLAYLIST",getClientPlaylist(customEvent->getCustomEvent(IDENT))));
                getDataResult = QString(jDoc.toJson(QJsonDocument::Compact));
                qDebug() << get(NAME) + getDataResult;
            }
            if (type == "DOWNLOAD_URL") {
                QJsonDocument jDoc(generateJsonDocument("PUTDATA","DOWNLOAD_URL",getDownloadUrlFromFile(customEvent->getCustomEvent(IDENT),customEvent->getCustomEvent(DATA))));
                getDataResult = QString(jDoc.toJson(QJsonDocument::Compact));
                qDebug() << get(NAME) + getDataResult;
            }

            CustomEvent cEvent;
            cEvent.addCustomEvent(SENDER,get(NAME));
            cEvent.addCustomEvent(COMMAND,"PUTDATA");
            cEvent.addCustomEvent(TYPE,customEvent->getCustomEvent(TYPE));
            cEvent.addCustomEvent(IDENT,customEvent->getCustomEvent(IDENT));
            cEvent.addCustomEvent(DATA,getDataResult);
            corelibrary->addEvent(&cEvent);
        }
        if (command == "INSERTENTRY" || command == "DELETEENTRY" || command == "UPDATEENTRY") {

            qInfo() << get(NAME) + " onEvent: " + "Command: " + command  + " Type: " + type;

            int modi;
            if (command == "INSERTENTRY")
                modi = INSERT;
            else if (command == "DELETEENTRY")
                modi = DELETE;
            else if (command == "UPDATEENTRY")
                modi = UPDATE;
            else
                modi = UNKNOWN;

            int onSuccess = 0;
            QStringList dataList = generateDataList(customEvent->getCustomEvent(DATA));

            if (type == "GROUP") {
                onSuccess = groupOperation(dataList, modi);
            }
            if (type == "USER") {
                onSuccess = userOperation(dataList, modi);
            }
            if (type == "HARDWARE") {
                onSuccess = hardwareOperation(dataList, modi);
            }
            if (type == "PLAYLIST") {
                onSuccess = playlistOperation(dataList, modi);
            }
            if (type == "PLANNING") {
                onSuccess = planningOperation(dataList, modi);
            }

            if (onSuccess == true)
                m_databaseGeneral.saveDatabaseToFile(m_config->dbPath());
        }
    }
    return false;
}

/**
 * @brief Database::loadSettings Lädt die Plugin-spezifischen Informationen (db_path, db_connectionname)
 */
void Database::loadSettings()
{
    settings = new QSettings(get(PSETTINGS),QSettings::IniFormat);

    settings->beginGroup(get(NAME));
    if (!settings->contains("db_path"))
        settings->setValue("db_path",corelibrary->getAppDir().absolutePath() + "/database/database.db");
    m_config->setDbPath(settings->value("db_path").toString());
    if (!settings->contains("db_connectionname"))
        settings->setValue("db_connectionname","openSignageDB");
    m_config->setConnectionName(settings->value("db_connectionname").toString());
    settings->endGroup();

    qInfo() << get(NAME) + " Loaded settings: " + "db_path: " + m_config->dbPath() + " db_connectionname: " + m_config->connectionName();

    settings->sync();
}

/**
 * @brief Database::saveSettings Speichert die Plugin-spezifischen Informationen (db_path, db_connectionname)
 */
void Database::saveSettings()
{
    settings->beginGroup(get(NAME));
    settings->setValue("db_path",m_config->dbPath());
    settings->setValue("db_connectionname",m_config->dbPath());
    settings->sync();
    settings->endGroup();

    qInfo() << get(NAME) + " Loaded settings: " + "db_path: " + m_config->dbPath() + " db_connectionname: " + m_config->connectionName();
}

/**
 * @brief Database::addNewContentToDB Fügt den aus dem FileWatcher-Thread neu überprüften Content der Datenbank hinzu.
 * @param newContent Content-spezifische Informationen (Name, MD5, Reso, Size, Time, Path)
 */
void Database::addNewContentToDB(QStringList newContent)
{
    QSqlQuery q(m_databaseGeneral.getDB());
    QString querytext = QString("INSERT INTO Content (name,md5,reso,size,time,path) "
                                "VALUES (:name, :md5, :reso, :size, :time, :path)");
    q.prepare(querytext);

    QString name, md5, reso, mysize, time, path;
    foreach (QString singleData, newContent) {
        QStringList splittedData = singleData.split("=");
        if (splittedData.first() == "Name")
            name = splittedData.last();
        if (splittedData.first() == "MD5")
            md5 = splittedData.last();
        if (splittedData.first() == "Reso")
            reso = splittedData.last();
        if (splittedData.first() == "Size")
            mysize = splittedData.last();
        if (splittedData.first() == "Time")
            time = splittedData.last();
        if (splittedData.first() == "Path")
            path = splittedData.last();
    }

    q.bindValue(":name", name);
    q.bindValue(":md5", md5);
    q.bindValue(":reso", reso);
    q.bindValue(":size", mysize);
    q.bindValue(":time", time);
    q.bindValue(":path", path);

    if (!q.exec()) {
        qDebug() << get(NAME) + "Cannot execute query:" << querytext;
    }
    else{
        qDebug() << get(NAME) + "executed query:" << querytext;
    }

    m_databaseGeneral.saveDatabaseToFile(m_config->dbPath());
}

/**
 * @brief Database::createInitialTables Erstellt die initialen Tabellen der Datenbank
 * @return 0
 */
int Database::createInitialTables()
{
    /*************** Grouptypes, Groups and User - Tables ***************/
    QString query;
    query = "create table Grouptypes ("
            "id_grouptype INTEGER NOT NULL PRIMARY KEY UNIQUE,"
            "typename CHAR(255) NOT NULL"
            ")";
    m_databaseGeneral.createDatabaseTable(query);

    query = "create table Groups ("
            "id_group INTEGER NOT NULL PRIMARY KEY UNIQUE,"
            "name CHAR(255) NOT NULL,"
            "grouptype INTEGER NOT NULL,"
            "createdby INTEGER NOT NULL,"
            "FOREIGN KEY(grouptype) REFERENCES Grouptypes(id_grouptype)"
            ")";
    m_databaseGeneral.createDatabaseTable(query);

    query = "create table Users ("
            "id_user INTEGER NOT NULL PRIMARY KEY UNIQUE,"
            "firstname CHAR(255) NOT NULL,"
            "lastname CHAR(255) NOT NULL,"
            "password CHAR(255) NOT NULL,"
            "email CHAR(255) NOT NULL UNIQUE,"
            "fromgroup INTEGER NOT NULL,"
            "FOREIGN KEY(fromgroup) REFERENCES Groups(id_group)"
            ")";
    m_databaseGeneral.createDatabaseTable(query);

    /*************** Orientation, Playlists, Planning, Content - Tables ***************/
    query = "create table Orientation ("
            "id_orientation INTEGER NOT NULL PRIMARY KEY UNIQUE,"
            "name CHAR(255) NOT NULL"
            ")";
    m_databaseGeneral.createDatabaseTable(query);

    query = "create table Content ("
            "id_content INTEGER NOT NULL PRIMARY KEY UNIQUE,"
            "name CHAR(255) NOT NULL,"
            "md5 CHAR(255) NOT NULL,"
            "reso CHAR(255) NOT NULL,"
            "size CHAR(255) NOT NULL,"
            "time CHAR(255) NOT NULL,"
            "path TEXT NOT NULL"
            ")";
    m_databaseGeneral.createDatabaseTable(query);

    query = "create table Planning ("
            "id_planning INTEGER NOT NULL PRIMARY KEY UNIQUE,"
            "valid_period CHAR(255) NOT NULL,"
            "weekdays CHAR(255) NOT NULL,"
            "valid_time CHAR(255) NOT NULL,"
            "type CHAR(255) NOT NULL,"
            "id_order INTEGER NOT NULL,"
            "content INTEGER NOT NULL,"
            "fromgroup INTEGER NOT NULL,"
            "fromplaylist INTEGER NOT NULL,"
            "duration INTEGER NOT NULL,"
            "FOREIGN KEY(content) REFERENCES Content(id_content),"
            "FOREIGN KEY(fromgroup) REFERENCES Groups(id_group),"
            "FOREIGN KEY(fromplaylist) REFERENCES Playlists(id_playlist)"
            ")";
    m_databaseGeneral.createDatabaseTable(query);

    query = "create table Playlists ("
            "id_playlist INTEGER NOT NULL PRIMARY KEY UNIQUE,"
            "name CHAR(255) NOT NULL,"
            "orientation INTEGER NOT NULL,"
            "fromgroup INTEGER NOT NULL,"
            "planedon TEXT,"
            "FOREIGN KEY(orientation) REFERENCES Orientation(id_orientation),"
            "FOREIGN KEY(fromgroup) REFERENCES Groups(id_group)"
            ")";
    m_databaseGeneral.createDatabaseTable(query);

    /*************** Hardware - Tables ***************/
    query = "create table Clients ("
            "mac CHAR(255) NOT NULL PRIMARY KEY UNIQUE,"
            "location CHAR(255) NOT NULL,"
            "version CHAR(255) NOT NULL,"
            "fromgroup INTEGER NOT NULL,"
            "planedon INTEGER,"
            "FOREIGN KEY(planedon) REFERENCES Playlists(id_playlist)"
            "FOREIGN KEY(fromgroup) REFERENCES Groups(id_group)"
            ")";
    m_databaseGeneral.createDatabaseTable(query);

    m_databaseGeneral.saveDatabaseToFile(m_config->dbPath());
    return 0; //m_databaseGeneral.createDatabaseTable(query);
}

/**
 * @brief Database::testFillTables Erstellt die vordefinierten "GroupTypes" an welchen die Benutzerrechte gebunden sind. Darüber hinaus
 * wird die initiale Gruppe inkl. Admin-Account und die Playlisten-Orientation Tabelle erstellt.
 * @return 0
 */
int Database::testFillTables()
{
    //GROUPTYPES!
    QString query = "insert into Grouptypes (id_grouptype,typename) values(1, 'CMS-Administrator')";
    m_databaseGeneral.addDatabaseEntry(query);
    query = "insert into Grouptypes (id_grouptype,typename) values(2, 'Group-Administrator')";
    m_databaseGeneral.addDatabaseEntry(query);
    query = "insert into Grouptypes (id_grouptype,typename) values(3, 'User')";
    m_databaseGeneral.addDatabaseEntry(query);

    //GROUPS!
    query = "insert into Groups (id_group,name,grouptype,createdby) values(1, 'FH-Augsburg', 1,1)";
    m_databaseGeneral.addDatabaseEntry(query);

    //USERS!
    query = "insert into Users (id_user,firstname,lastname,password,email,fromgroup) values(1,'admin','admin','admin','admin@gadmin.de',1)";
    m_databaseGeneral.addDatabaseEntry(query);

    //ORIENTATION!
    query = "insert into Orientation (id_orientation,name) values(1, 'PANORAMA')";
    m_databaseGeneral.addDatabaseEntry(query);
    query = "insert into Orientation (id_orientation,name) values(2, 'PORTRAIT')";
    m_databaseGeneral.addDatabaseEntry(query);

    m_databaseGeneral.saveDatabaseToFile(m_config->dbPath());
    return 0;
}

/**
 * @brief Database::generateJsonDocument Die Funktion verpackt die zu übertragende Informationen in ein Json-Dokument.
 * @param command Das Kommando welches ein Plugin verarbeiten kann.
 * @param array Ein Array in welchem Informationen (Daten) verpackt wird.
 * @param data Die tatsächlichen zu übertragenden Informationen.
 * @return Das fertig generierte Json-Dokument.
 */
QJsonDocument Database::generateJsonDocument(QString command, QString array, QList<QStringList> data)
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
    return jDoc;
}

/**
 * @brief Database::generateDataList, "Entpackt" die Daten aus einem JsonDokument.
 * @param data Die tatsächlichen zu verarbeitende Informationen.
 * @return Daten die zu verarbeiten sind.
 */
QStringList Database::generateDataList(QString data)
{
    QJsonObject jsonMainObj;
    QStringList ret;
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());

    // check validity of the document
    if(!doc.isNull())
    {
        if(doc.isObject()) {
            jsonMainObj = doc.object();
        }
        else {
            qDebug() << get(NAME) + "Document is not an object" << endl;
        }
    }
    else {
        qDebug() << get(NAME) + "Invalid JSON...\n" << data << endl;
    }

    if (data != "") {
        QString objKeys;
        QStringList dataList;

        foreach (objKeys, jsonMainObj.keys()) {
            QString dataEntry = objKeys + "=" + jsonMainObj.value(objKeys).toString();
            dataList.append(dataEntry);
        }
        ret = dataList;
    }
    return ret;
}

/**
 * @brief Database::getAuth Der Benutzer von der GUI muss für die Übertragung von Informationen sich am CMS authentifizieren.
 * @param ident eMail des Benutzers
 * @param pwd Password des Benutzers
 * @return Strukturierte Information zur Generierung eines Json-Dokuments, welches den Authentifizierungs-Status beinhaltet.
 */
QList<QStringList> Database::getAuth(QString ident, QString pwd)
{
    QString rel = "auth=failed";
    QString groupType;

    QSqlQuery q(m_databaseGeneral.getDB());
    QString querytext = QString("SELECT Users.email, Users.password, Grouptypes.typename FROM Users INNER JOIN Groups INNER JOIN Grouptypes ON Users.fromgroup = Groups.id_group AND Groups.grouptype = Grouptypes.id_grouptype ") +
            "WHERE Users.email = :bind_email "
            "AND Users.password = :bind_password;";

    q.prepare(querytext);
    q.bindValue(":bind_email", ident);
    q.bindValue(":bind_password", pwd);

    if (!q.exec()) {
        qDebug() << get(NAME) + "Cannot execute query:" << querytext;
    }
    else{
        while (q.next()) {
            if ( (ident == q.value(0).toString()) && (pwd == q.value(1).toString())) {
                rel = "auth=success";
                groupType = q.value(2).toString();
            }
        }
    }

    QStringList auth;
    QList<QStringList> authList;
    auth.append(QString("groupType=") + groupType);
    auth.append(rel);
    authList.append(auth);
    return authList;
}

/**
 * @brief Database::getAuthPi Der Client (RASPI) muss für die Übertragung von Informationen sich am CMS authentifizieren.
 * @param ident MAC des Clients
 * @return Strukturierte Information zur Generierung eines Json-Dokuments, welches den Authentifizierungs-Status beinhaltet.
 */
QList<QStringList> Database::getAuthPi(QString ident)
{
    QString rel = "auth=failed";

    QSqlQuery q(m_databaseGeneral.getDB());
    QString querytext = QString("SELECT Clients.mac FROM Clients ") +
            "WHERE Clients.mac = :bind_mac;";

    q.prepare(querytext);
    q.bindValue(":bind_mac", ident);

    if (!q.exec()) {
        qDebug() << get(NAME) + "Cannot execute query:" << querytext;
    }
    else{
        while (q.next()) {
            if (ident == q.value(0).toString()) {
                rel = "auth=success";
            }
        }
    }

    QStringList auth;
    QList<QStringList> authList;
    auth.append(rel);
    authList.append(auth);
    return authList;
}

/**
 * @brief Database::getGroupFromIdent Extrahiert den Gruppentyp des Benutzers aus der Datenbank, an welcher die Rechte gebunden sind.
 * @param ident eMail des Benutzers
 * @return Der Gruppentyp des Benutzers, an welcher die Rechte gebunden sind.
 */
QString Database::getGroupFromIdent(QString ident)
{
    QString gruppe;

    QSqlQuery q(m_databaseGeneral.getDB());
    QString querytext = QString("SELECT Grouptypes.typename, Groups.createdby, Groups.name FROM Users INNER JOIN Groups INNER JOIN Grouptypes ON Users.fromgroup = Groups.id_group AND Groups.grouptype = Grouptypes.id_grouptype ") +
            "WHERE Users.email = :bind_email;";

    q.prepare(querytext);
    q.bindValue(":bind_email", ident);

    if (!q.exec()) {
        qDebug() << get(NAME) + "Cannot execute query:" << querytext;
    }
    else{
        while (q.next()) {
            gruppe = q.value(0).toString();
            gruppe += "=" + q.value(1).toString();
            gruppe += "=" + q.value(2).toString();
        }
    }
    return gruppe;
}

/**
 * @brief Database::getUserIdFromIdent Extrhiert die User-Id anhand des Idents aus der Datenbank
 * @param ident eMail des Benutzers
 * @return User-Id
 */
QString Database::getUserIdFromIdent(QString ident)
{
    QStringList names = ident.split(".");
    QString id_user;

    QSqlQuery q(m_databaseGeneral.getDB());
    QString querytext = QString("SELECT Users.id_user FROM Users ") +
            "WHERE Users.email = :bind_email;";

    q.prepare(querytext);
    q.bindValue(":bind_email", ident);

    if (!q.exec()) {
        qDebug() << get(NAME) + "Cannot execute query:" << querytext;
    }
    else{
        while (q.next()) {
            id_user = q.value(0).toString();
        }
    }
    return id_user;
}

/**
 * @brief Database::expandQueryTextWithGroup Erweitert das SQL-Query um den Gruppentyp des Benutzers, damit ein Benutzer nur Gruppentyp abhängige Informationen erhält.
 * @param querytext Aktueller SQL-Query-Text
 * @param group Der Name des Gruppentyps
 * @param id_user Die User-Id
 * @return Der erweitertete SQL-Query-Text
 */
QString Database::expandQueryTextWithGroup(QString querytext, QString group, QString id_user)
{
    Q_UNUSED(id_user)
    //grouptypename=createdby=groupname
    QStringList splitGroup = group.split("=");
    QString id_targetGroup; // beinhalted id von aktueller gruppe

    //alle gruppen die createdby und name von aktueller gruppe haben
    QSqlQuery q(m_databaseGeneral.getDB());
    QString querytext_step = QString("SELECT Groups.id_group FROM Groups ") +
            "WHERE Groups.createdby = :createdby "
            "AND Groups.name = :name;";

    q.prepare(querytext_step);
    q.bindValue(":createdby", splitGroup.at(1));
    q.bindValue(":name", splitGroup.at(2));

    if (!q.exec()) {
        qDebug() << get(NAME) + "Cannot execute query:" << querytext;
    }
    else{
        while (q.next()) {
            id_targetGroup = q.value(0).toString();
        }
    }

    QString expanded = querytext;
    if (splitGroup.at(0) == "User") {
        expanded += "AND (Groups.createdby = " + id_targetGroup + " OR Groups.name = '" + splitGroup.at(2) + "') ";
        expanded += " WHERE Grouptypes.typename = 'User';";
    }
    else if (splitGroup.at(0) == "Group-Administrator") {
        expanded += "AND (Groups.createdby = " + id_targetGroup + " OR Groups.name = '" + splitGroup.at(2) + "') ";
        expanded += "WHERE Grouptypes.typename = 'User' ";
        expanded += "OR Grouptypes.typename = 'Group-Administrator';";
    }
    else if (splitGroup.at(0) == "CMS-Administrator") {
        expanded += "WHERE Grouptypes.typename = 'User' ";
        expanded += "OR Grouptypes.typename = 'Group-Administrator' ";
        expanded += "OR Grouptypes.typename = 'CMS-Administrator';";

    }
    return expanded;
}

/**
 * @brief Database::getUsers Hole alle Benutzer
 * @param ident eMail des Benutzers
 * @return Strukturierte Information zur Generierung eines Json-Dokuments, welches die Benutzer beinhaltet.
 */
QList<QStringList> Database::getUsers(QString ident)
{
    QStringList user;
    QList<QStringList> userList;

    QSqlQuery q(m_databaseGeneral.getDB());

    QString querytext = QString("SELECT Users.id_user, Users.firstname, Users.lastname, Users.email, Groups.name, Grouptypes.typename FROM Users INNER JOIN Groups INNER JOIN Grouptypes ON Users.fromgroup = Groups.id_group AND Groups.grouptype = Grouptypes.id_grouptype ");
    querytext = expandQueryTextWithGroup(querytext,getGroupFromIdent(ident),0);
    q.prepare(querytext);

    if (!q.exec()) {
        qDebug() << get(NAME) + "Cannot execute query:" << querytext;
    }
    else{
        while (q.next()) {
            user.append(QString("ID") + QString("=") + q.value(0).toString());
            user.append(QString("FirstName") + QString("=") + q.value(1).toString());
            user.append(QString("LastName") + QString("=") + q.value(2).toString());
            user.append(QString("eMail") + QString("=") + q.value(3).toString());
            user.append(QString("GroupName") + QString("=") + q.value(4).toString());
            user.append(QString("GroupType") + QString("=") + q.value(5).toString());
            userList.append(user);
            user.clear();
        }
    }
    return userList;
}

/**
 * @brief Database::getGroups Hole alle Gruppen
 * @param ident eMail des Benutzers
 * @return Strukturierte Information zur Generierung eines Json-Dokuments, welches die Gruppen beinhaltet.
 */
QList<QStringList> Database::getGroups(QString ident)
{
    QStringList groups;
    QList<QStringList> groupsList;

    QSqlQuery q(m_databaseGeneral.getDB());
    QString id_user = getUserIdFromIdent(ident);
    QString querytext = QString("SELECT Groups.id_group, Grouptypes.id_grouptype, Groups.name, Grouptypes.typename FROM Groups INNER JOIN Grouptypes ON Groups.grouptype = Grouptypes.id_grouptype ");
    querytext = expandQueryTextWithGroup(querytext,getGroupFromIdent(ident),0);
    q.prepare(querytext);

    if (!q.exec()) {
        qDebug() << get(NAME) + "Cannot execute query:" << querytext;
    }
    else{
        while (q.next()) {
            groups.append(QString("ID") + QString("=") + q.value(0).toString());
            groups.append(QString("ID_Type") + QString("=") + q.value(1).toString());
            groups.append(QString("GroupName") + QString("=") + q.value(2).toString());
            groups.append(QString("GroupType") + QString("=") + q.value(3).toString());
            groupsList.append(groups);
            groups.clear();
        }
    }
    return groupsList;
}

/**
 * @brief Database::getPlaylists Hole alle Playlisten
 * @param ident eMail des Benutzers
 * @return Strukturierte Information zur Generierung eines Json-Dokuments, welches die Playlisten beinhaltet.
 */
QList<QStringList> Database::getPlaylists(QString ident)
{
    QStringList playlist;
    QList<QStringList> playlistList;
    QSqlQuery q(m_databaseGeneral.getDB());
    QString querytext = QString("SELECT Playlists.id_playlist, Playlists.name, Orientation.name, Groups.name, Playlists.planedon FROM Playlists INNER JOIN Orientation ON Playlists.orientation = Orientation.id_orientation INNER JOIN GROUPS ON Playlists.fromgroup = Groups.id_group INNER JOIN Grouptypes ON Groups.grouptype = Grouptypes.id_grouptype ");

    querytext = expandQueryTextWithGroup(querytext,getGroupFromIdent(ident),0);
    q.prepare(querytext);

    if (!q.exec()) {
        qDebug() << get(NAME) + "Cannot execute query:" << querytext;
    }
    else{
        while (q.next()) {
            playlist.append(QString("ID") + QString("=") + q.value(0).toString());
            playlist.append(QString("Name") + QString("=") + q.value(1).toString());
            playlist.append(QString("Orientation") + QString("=") + q.value(2).toString());
            playlist.append(QString("GroupName") + QString("=") + q.value(3).toString());
            playlist.append(QString("PlanedOn") + QString("=") + q.value(4).toString());
            playlistList.append(playlist);
            playlist.clear();
        }
    }
    return playlistList;
}

/**
 * @brief Database::getPlannings Hole alle Plannings
 * @param ident eMail des Benutzers
 * @return Strukturierte Information zur Generierung eines Json-Dokuments, welches die Planings beinhaltet.
 */
QList<QStringList> Database::getPlannings(QString ident)
{
    QStringList planninglist;
    QList<QStringList> planninglistList;
    QSqlQuery q(m_databaseGeneral.getDB());
    QString querytext = QString("SELECT Playlists.id_playlist, Planning.id_planning, Planning.valid_period, Planning.weekdays, Planning.valid_time, Planning.type, Content.id_content, Content.name, Planning.id_order, Planning.duration FROM Playlists INNER JOIN Planning INNER JOIN Content INNER JOIN Groups INNER JOIN Grouptypes ON Groups.grouptype = Grouptypes.id_grouptype AND Planning.fromgroup = Groups.id_group AND Playlists.id_playlist = Planning.fromplaylist AND Planning.content = Content.id_content ");
    querytext = expandQueryTextWithGroup(querytext,getGroupFromIdent(ident),0);
    q.prepare(querytext);

    if (!q.exec()) {
        qDebug() << get(NAME) + "Cannot execute query:" << querytext;
    }
    else{
        while (q.next()) {
            planninglist.append(QString("ID_Playlist") + QString("=") + q.value(0).toString());
            planninglist.append(QString("ID_Planning") + QString("=") + q.value(1).toString());
            planninglist.append(QString("Valid_Period") + QString("=") + q.value(2).toString());
            planninglist.append(QString("Weekdays") + QString("=") + q.value(3).toString());
            planninglist.append(QString("Valid_Time") + QString("=") + q.value(4).toString());
            planninglist.append(QString("Type") + QString("=") + q.value(5).toString());
            planninglist.append(QString("ID_Content") + QString("=") + q.value(6).toString());
            planninglist.append(QString("ContentName") + QString("=") + q.value(7).toString());
            planninglist.append(QString("ID_Order") + QString("=") + q.value(8).toString());
            planninglist.append(QString("Duration") + QString("=") + q.value(9).toString());
            planninglistList.append(planninglist);
            planninglist.clear(); //clear for new benutzer
        }
    }
    return planninglistList;
}

/**
 * @brief Database::getHardware Hole alle Clients
 * @param ident eMail des Benutzers
 * @return Strukturierte Information zur Generierung eines Json-Dokuments, welches die Clients beinhaltet.
 */
QList<QStringList> Database::getHardware(QString ident)
{
    QStringList hardware;
    QList<QStringList> hardwareList;
    QSqlQuery q(m_databaseGeneral.getDB());
    QString querytext = QString("SELECT Clients.mac, Clients.location, Clients.version, Groups.name, Grouptypes.typename, Clients.planedon FROM Clients INNER JOIN Groups INNER JOIN Grouptypes ON Clients.fromgroup = Groups.id_group AND Groups.grouptype = Grouptypes.id_grouptype ");
    querytext = expandQueryTextWithGroup(querytext,getGroupFromIdent(ident),0);
    q.prepare(querytext);

    if (!q.exec()) {
        qDebug() << get(NAME) + "Cannot execute query:" << querytext;
    }
    else{
        while (q.next()) {
            hardware.append(QString("MAC") + QString("=") + q.value(0).toString());
            hardware.append(QString("Location") + QString("=") + q.value(1).toString());
            hardware.append(QString("Version") + QString("=") + q.value(2).toString());
            hardware.append(QString("GroupName") + QString("=") + q.value(3).toString());
            hardware.append(QString("GroupType") + QString("=") + q.value(4).toString());
            hardware.append(QString("PlanedOn") + QString("=") + q.value(5).toString());
            hardwareList.append(hardware);
            hardware.clear(); //clear for new benutzer
        }
    }
    return hardwareList;
}

/**
 * @brief Database::getContent Hole alle Contents
 * @param ident eMail des Benutzers
 * @return Strukturierte Information zur Generierung eines Json-Dokuments, welches die Contents beinhaltet.
 */
QList<QStringList> Database::getContent(QString ident)
{
    Q_UNUSED(ident) // maybe when content should be depending on ident
    QStringList content;
    QList<QStringList> contentListe;
    QSqlQuery q(m_databaseGeneral.getDB());
    QString querytext = QString("SELECT Content.id_content, Content.name, Content.md5, Content.path, Content.reso, Content.size, Content.time FROM Content;");
    q.prepare(querytext);
    if (!q.exec()) {
        qDebug() << get(NAME) + "Cannot execute query:" << querytext;
    }
    else{
        while (q.next()) {
            content.append(QString("ID") + QString("=") + q.value(0).toString());
            content.append(QString("Name") + QString("=") + q.value(1).toString());
            content.append(QString("MD5") + QString("=") + q.value(2).toString());
            content.append(QString("FilePath") + QString("=") + q.value(3).toString());
            content.append(QString("Reso") + QString("=") + q.value(4).toString());
            content.append(QString("Size") + QString("=") + q.value(5).toString());
            content.append(QString("Time") + QString("=") + q.value(6).toString());
            contentListe.append(content);
            content.clear(); //clear for new benutzer
        }
    }
    return contentListe;
}

/**
 * @brief Database::getClientPlaylist Hole für den Client (RASPI) die spezifische Playlist inkl. aller Planings
 * @param ident MAC des RASPIS
 * @return Strukturierte Information zur Generierung eines Json-Dokuments, welches die Client spezifische Playliste inkl. aller Planings beinhaltet.
 */
QList<QStringList> Database::getClientPlaylist(QString ident)
{
    QStringList planningList;
    QList<QStringList> playlistList;
    int playlistFromMac = 0;
    QString playlistName;
    QString playlistOrientation;

    //playlist id bekommen
    QSqlQuery q(m_databaseGeneral.getDB());
    QString querytext = QString("SELECT Clients.planedon FROM Clients ") +
            "WHERE Clients.mac = :bind_mac;";

    q.prepare(querytext);
    q.bindValue(":bind_mac", ident);

    if (!q.exec()) {
        qDebug() << get(NAME) + "Cannot execute query:" << querytext;
    }
    else{
        while (q.next()) {
            playlistFromMac = q.value(0).toInt();
        }
    }

    if (playlistFromMac <= 0)
        return playlistList;

    // playlist allgemeine daten bekommen
    querytext = QString("SELECT Playlists.name, Orientation.name FROM Playlists INNER JOIN Orientation ON Playlists.orientation = Orientation.id_orientation ") +
            "WHERE Playlists.id_playlist = :bind_id;";

    q.prepare(querytext);
    q.bindValue(":bind_id", QString::number(playlistFromMac));

    if (!q.exec()) {
        qDebug() << get(NAME) + "Cannot execute query:" << querytext;
    }
    else{
        while (q.next()) {
            playlistName = q.value(0).toString();
            playlistOrientation = q.value(1).toString();
            planningList.append(QString("PlaylistName") + QString("=") + q.value(0).toString());
            planningList.append(QString("PlaylistOrientation") + QString("=") + q.value(1).toString());
            playlistList.append(planningList);
            planningList.clear();
        }
    }

    // plannings
    querytext = QString("SELECT Planning.id_order, Planning.type, Planning.weekdays, Planning.valid_period, Planning.valid_time, Content.id_content, Content.name, Content.md5, Planning.id_planning, Content.time, Planning.duration FROM Playlists INNER JOIN Planning INNER JOIN Content INNER JOIN Groups INNER JOIN Grouptypes ON Groups.grouptype = Grouptypes.id_grouptype AND Planning.fromgroup = Groups.id_group AND Playlists.id_playlist = Planning.fromplaylist AND Planning.content = Content.id_content ") +
            "WHERE Planning.fromplaylist = :bind_id " +
            "ORDER BY Planning.id_order ASC;";

    q.prepare(querytext);
    q.bindValue(":bind_id", QString::number(playlistFromMac));

    if (!q.exec()) {
        qDebug() << get(NAME) + "Cannot execute query:" << querytext;
    }
    else{
        while (q.next()) {
            planningList.append(QString("ID_Order") + QString("=") + q.value(0).toString());
            planningList.append(QString("Type") + QString("=") + q.value(1).toString());

            planningList.append(QString("Weekdays") + QString("=") + q.value(2).toString());
            planningList.append(QString("Valid_Period") + QString("=") + q.value(3).toString());
            planningList.append(QString("Valid_Time") + QString("=") + q.value(4).toString());

            planningList.append(QString("ID_Content") + QString("=") + q.value(5).toString());
            planningList.append(QString("ContentName") + QString("=") + q.value(6).toString().mid(q.value(6).toString().lastIndexOf("/")));
            planningList.append(QString("ContentMD5") + QString("=") + q.value(7).toString());

            planningList.append(QString("ID_Planning") + QString("=") + q.value(8).toString());
            planningList.append(QString("Duration") + QString("=") + q.value(9).toString());
            planningList.append(QString("DurationPlanning") + QString("=") + q.value(10).toString());

            playlistList.append(planningList);
            planningList.clear(); //clear for new benutzer
        }
    }
    return playlistList;
}

/**
 * @brief Database::getDownloadUrlFromFile Hole die Download-URL von einem spezifischen Content.
 * @param ident MAC des RASPIS
 * @param id_content ID des Contents
 * @return Strukturierte Information zur Generierung eines Json-Dokuments, welches die Download-Url beinhaltet.
 */
QList<QStringList> Database::getDownloadUrlFromFile(QString ident, QString id_content)
{
    Q_UNUSED(ident)

    QStringList url;
    QList<QStringList> urlList;
    QString cleanId = id_content.mid(id_content.lastIndexOf(":")+2);
    cleanId = cleanId.left(cleanId.lastIndexOf("\""));

    QSqlQuery q(m_databaseGeneral.getDB());
    QString querytext = QString("SELECT Content.path FROM Content ") +
            "WHERE Content.id_content = :bind_id";

    q.prepare(querytext);
    q.bindValue(":bind_id", cleanId);

    if (!q.exec()) {
        qDebug() << get(NAME) + "Cannot execute query:" << querytext;
    }
    else{
        while (q.next()) {
            QProcess proc;
            proc.start("hostname",QStringList() << "-I");
            proc.waitForFinished(200);
            QString ip = proc.readAll();
            //ip = "127.0.0.1" ;
            ip = ip.left(ip.lastIndexOf(" "));
            url.append("PATH=http://" + ip + q.value(0).toString().mid(q.value(0).toString().lastIndexOf("/content_uploaded/")));
            urlList.append(url);
        }
    }
    return urlList;
}

/**
 * @brief Database::groupOperation Operationen an der Gruppen-Tabelle (INSERT, UPDATE, DELETE)
 * @param data Benötigte Informationen für eine Operation
 * @param operationType (INSERT, UPDATE, DELETE)
 * @return onFail = false, onSuccess = true => notwendig für ggf. Speicherung der DB.
 */
int Database::groupOperation(QStringList data, int operationType)
{
    bool ret = false;
    QSqlQuery q(m_databaseGeneral.getDB());

    if (operationType == INSERT) {
        QString querytext = QString("INSERT INTO Groups (name,grouptype,createdby) "
                                    "VALUES (:name, :grouptype, :createdby)");
        q.prepare(querytext);

        QString name;
        QString grouptype;
        QString createdby;

        foreach (QString singleData, data) {
            QStringList splittedData = singleData.split("=");
            if (splittedData.first() == "groupName")
                name = splittedData.last();
            if (splittedData.first() == "groupTypeID")
                grouptype = splittedData.last();
            if (splittedData.first() == "createdby")
                createdby = splittedData.last();
        }

        q.bindValue(":name", name);
        q.bindValue(":grouptype", grouptype);
        q.bindValue(":createdby", createdby);

        if (!q.exec()) {
            qDebug() << get(NAME) + "Cannot execute query:" << querytext;
            ret = false;
        }
        else{
            qDebug() << get(NAME) + "executed query:" << querytext;
            ret = true;
        }
    }
    else if (operationType == UPDATE) {
        QString querytext = QString("UPDATE Groups SET name=:groupName, grouptype=:groupType WHERE id_group=:id_group");
        q.prepare(querytext);

        QString groupName;
        int groupType = 0;
        int id_group = 0;

        foreach (QString singleData, data) {
            QStringList splittedData = singleData.split("=");
            if (splittedData.first() == "groupName")
                groupName = splittedData.last();
            if (splittedData.first() == "groupTypeID")
                groupType = splittedData.last().toInt();
            if (splittedData.first() == "id_group")
                id_group = splittedData.last().toInt();

        }

        q.bindValue(":groupType", groupType);
        q.bindValue(":groupName", groupName);
        q.bindValue(":id_group", id_group);

        if (!q.exec()) {
            qDebug() << get(NAME) + "Cannot execute query:" << querytext;
            ret = false;
        }
        else{
            qDebug() << get(NAME) + "executed query:" << querytext;
            ret = true;
        }
    }
    else if (operationType == DELETE) {
        QString querytext = QString("DELETE FROM Groups WHERE id_group=");
        QString id;
        foreach (QString singleData, data) {
            QStringList splittedData = singleData.split("=");
            if (splittedData.first() == "id") {
                id = splittedData.last();
            }
        }
        querytext += id;
        q.prepare(querytext);

        if (!q.exec()) {
            qDebug() << get(NAME) + "Cannot execute query:" << querytext;
            ret = false;
        }
        else{
            qDebug() << get(NAME) + "executed query:" << querytext;
            qDebug() << get(NAME) + q.lastQuery();
            ret = true;
        }
    }
    else {
        return ret;
    }
    return ret;
}

/**
 * @brief Database::userOperation Operationen an der User-Tabelle (INSERT, UPDATE, DELETE)
 * @param data Benötigte Informationen für eine Operation
 * @param operationType (INSERT, UPDATE, DELETE)
 * @return onFail = false, onSuccess = true => notwendig für ggf. Speicherung der DB.
 */
int Database::userOperation(QStringList data, int operationType)
{
    bool ret = false;
    QSqlQuery q(m_databaseGeneral.getDB());

    if (operationType == INSERT) {
        QString querytext = QString("INSERT INTO Users (firstname,lastname,password,email,fromgroup) "
                                    "VALUES (:firstName, :lastName, :password, :eMail, :groupNameID)");
        q.prepare(querytext);

        QString firstName;
        QString lastName;
        QString password;
        QString eMail;
        QString groupNameID;

        foreach (QString singleData, data) {
            QStringList splittedData = singleData.split("=");
            if (splittedData.first() == "firstName")
                firstName = splittedData.last();
            if (splittedData.first() == "lastName")
                lastName = splittedData.last();
            if (splittedData.first() == "password")
                password = splittedData.last();
            if (splittedData.first() == "eMail")
                eMail = splittedData.last();
            if (splittedData.first() == "groupNameID")
                groupNameID = splittedData.last();
        }

        q.bindValue(":firstName", firstName);
        q.bindValue(":lastName", lastName);
        q.bindValue(":password", password);
        q.bindValue(":eMail", eMail);
        q.bindValue(":groupNameID", groupNameID);

        if (!q.exec()) {
            qDebug() << get(NAME) + "Cannot execute query:" << querytext;
            ret = false;
        }
        else{
            qDebug() << get(NAME) + "executed query:" << querytext;
            ret = true;
        }
    }
    else if (operationType == UPDATE) {

        QString id;
        QString firstName;
        QString lastName;
        QString password;
        QString eMail;
        QString groupNameID;

        foreach (QString singleData, data) {
            QStringList splittedData = singleData.split("=");
            if (splittedData.first() == "id")
                id = splittedData.last();
            if (splittedData.first() == "firstName")
                firstName = splittedData.last();
            if (splittedData.first() == "lastName")
                lastName = splittedData.last();
            if (splittedData.first() == "password")
                password = splittedData.last();
            if (splittedData.first() == "eMail")
                eMail = splittedData.last();
            if (splittedData.first() == "groupNameID")
                groupNameID = splittedData.last();
        }

        QString querytext;
        if (password != "") {
            querytext = QString("UPDATE Users SET firstname=:firstname, lastname=:lastname, password=:password, email=:email, fromgroup=:fromgroup WHERE id_user=:id");
            q.prepare(querytext);
        }
        else {
            querytext = QString("UPDATE Users SET firstname=:firstname, lastname=:lastname, email=:email, fromgroup=:fromgroup WHERE id_user=:id");
            q.prepare(querytext);
        }

        q.bindValue(":id", id);
        q.bindValue(":firstname", firstName);
        q.bindValue(":lastname", lastName);
        q.bindValue(":email", eMail);
        q.bindValue(":fromgroup", groupNameID);
        if (password != "")
            q.bindValue(":password", password);

        if (!q.exec()) {
            qDebug() << get(NAME) + "Cannot execute query:" << querytext;
            ret = false;
        }
        else{
            qDebug() << get(NAME) + "executed query:" << querytext;
            ret = true;
        }
    }
    else if (operationType == DELETE) {
        QString querytext = QString("DELETE FROM Users WHERE id_user=");


        QString id;
        foreach (QString singleData, data) {
            QStringList splittedData = singleData.split("=");
            if (splittedData.first() == "id") {
                id = splittedData.last();
            }
        }
        querytext += id;
        q.prepare(querytext);

        if (!q.exec()) {
            qDebug() << get(NAME) + "Cannot execute query:" << querytext;
            ret = false;
        }
        else{
            qDebug() << get(NAME) + "executed query:" << querytext;
            qDebug() << get(NAME) + q.lastQuery();
            ret = true;
        }
    }
    else {
        return ret;
    }
    return ret;
}

/**
 * @brief Database::hardwareOperation Operationen an der Hardware-Tabelle (INSERT, UPDATE, DELETE)
 * @param data Benötigte Informationen für eine Operation
 * @param operationType (INSERT, UPDATE, DELETE)
 * @return onFail = false, onSuccess = true => notwendig für ggf. Speicherung der DB.
 */
int Database::hardwareOperation(QStringList data, int operationType)
{
    bool ret = false;
    QSqlQuery q(m_databaseGeneral.getDB());

    if (operationType == INSERT) {
        QString querytext = QString("INSERT INTO Clients (mac,location,version,fromgroup,planedon) "
                                    "VALUES (:mac, :location, :version, :groupNameID, :planedon)");
        q.prepare(querytext);

        QString mac;
        QString location;
        QString version;
        QString groupNameID;
        int planedon = 0;

        foreach (QString singleData, data) {
            QStringList splittedData = singleData.split("=");
            if (splittedData.first() == "mac")
                mac = splittedData.last();
            if (splittedData.first() == "location")
                location = splittedData.last();
            if (splittedData.first() == "version")
                version = splittedData.last();
            if (splittedData.first() == "id")
                groupNameID = splittedData.last();
            if (splittedData.first() == "planedon")
                planedon = splittedData.last().toInt();
        }

        q.bindValue(":mac", mac);
        q.bindValue(":location", location);
        q.bindValue(":version", version);
        q.bindValue(":groupNameID", groupNameID);
        q.bindValue(":planedon", planedon);

        if (!q.exec()) {
            qDebug() << get(NAME) + "Cannot execute query:" << querytext;
            ret = false;
        }
        else{
            qDebug() << get(NAME) + "executed query:" << querytext;
            ret = true;
        }
    }
    else if (operationType == UPDATE) {
        QString querytext = QString("UPDATE Clients SET location=:location, version=:version, fromgroup=:groupNameID, planedon=:planedon WHERE mac=:mac");
        q.prepare(querytext);

        QString mac;
        QString location;
        QString version;
        QString groupNameID;
        int planedon = 0;

        foreach (QString singleData, data) {
            QStringList splittedData = singleData.split("=");
            if (splittedData.first() == "mac")
                mac = splittedData.last();
            if (splittedData.first() == "location")
                location = splittedData.last();
            if (splittedData.first() == "version")
                version = splittedData.last();
            if (splittedData.first() == "id")
                groupNameID = splittedData.last();
            if (splittedData.first() == "planedon")
                planedon = splittedData.last().toInt();
        }

        q.bindValue(":mac", mac);
        q.bindValue(":location", location);
        q.bindValue(":version", version);
        q.bindValue(":groupNameID", groupNameID);
        q.bindValue(":planedon", planedon);

        if (!q.exec()) {
            qDebug() << get(NAME) + "Cannot execute query:" << querytext;
            ret = false;
        }
        else{
            qDebug() << get(NAME) + "executed query:" << querytext;
            ret = true;
        }
    }
    else if (operationType == DELETE) {
        QString querytext = QString("DELETE FROM Clients WHERE mac=:mac");
        q.prepare(querytext);

        QString mac;
        foreach (QString singleData, data) {
            QStringList splittedData = singleData.split("=");
            if (splittedData.first() == "mac") {
                mac = splittedData.last();
            }
        }
        q.bindValue(":mac", mac);

        if (!q.exec()) {
            qDebug() << get(NAME) + "Cannot execute query:" << querytext;
            ret = false;
        }
        else{
            qDebug() << get(NAME) + "executed query:" << querytext;
            ret = true;
        }
    }
    else {
        return ret;
    }
    return ret;
}

/**
 * @brief Database::playlistOperation Operationen an der Playlist-Tabelle (INSERT, UPDATE, DELETE)
 * @param data Benötigte Informationen für eine Operation
 * @param operationType (INSERT, UPDATE, DELETE)
 * @return onFail = false, onSuccess = true => notwendig für ggf. Speicherung der DB.
 */
int Database::playlistOperation(QStringList data, int operationType)
{
    bool ret = false;
    QSqlQuery q(m_databaseGeneral.getDB());

    if (operationType == INSERT) {
        QString querytext = QString("INSERT INTO Playlists (name,orientation,fromgroup,planedon) "
                                    "VALUES (:name, :orientation, :fromgroup, :planedon)");
        q.prepare(querytext);

        QString name;
        QString orientation;
        int fromgroup = 0;
        QString planedon;

        foreach (QString singleData, data) {
            QStringList splittedData = singleData.split("=");
            if (splittedData.first() == "name")
                name = splittedData.last();
            if (splittedData.first() == "orientation")
                orientation = splittedData.last();
            if (splittedData.first() == "id_group")
                fromgroup = splittedData.last().toInt();
            if (splittedData.first() == "planedon")
                planedon = splittedData.last();
        }

        q.bindValue(":name", name);
        q.bindValue(":orientation", orientation);
        q.bindValue(":fromgroup", fromgroup);
        q.bindValue(":planedon", planedon);

        if (!q.exec()) {
            qDebug() << get(NAME) + "Cannot execute query:" << querytext;
            ret = false;
        }
        else{
            qDebug() << get(NAME) + "executed query:" << querytext;
            ret = true;
        }
    }
    else if (operationType == UPDATE) {
        QString querytext = QString("UPDATE Playlists SET name=:name, orientation=:orientation, fromgroup=:fromgroup, planedon=:planedon WHERE id_playlist=:id_playlist");
        q.prepare(querytext);

        QString name;
        QString orientation;
        int fromgroup = 0;
        int id_playlist = 0;
        QString planedon;

        foreach (QString singleData, data) {
            QStringList splittedData = singleData.split("=");
            if (splittedData.first() == "name")
                name = splittedData.last();
            if (splittedData.first() == "orientation")
                orientation = splittedData.last();
            if (splittedData.first() == "id_group")
                fromgroup = splittedData.last().toInt();
            if (splittedData.first() == "id_playlist")
                id_playlist = splittedData.last().toInt();
            if (splittedData.first() == "planedon")
                planedon = splittedData.last();
        }

        q.bindValue(":name", name);
        q.bindValue(":orientation", orientation);
        q.bindValue(":fromgroup", fromgroup);
        q.bindValue(":id_playlist", id_playlist);
        q.bindValue(":planedon", planedon);

        if (!q.exec()) {
            qDebug() << get(NAME) + "Cannot execute query:" << querytext;
            ret = false;
        }
        else{
            qDebug() << get(NAME) + "executed query:" << querytext;
            ret = true;
        }
    }
    else if (operationType == DELETE) {
        QString querytext = QString("DELETE FROM Playlists WHERE id_playlist=");

        QString id;
        foreach (QString singleData, data) {
            QStringList splittedData = singleData.split("=");
            if (splittedData.first() == "id") {
                id = splittedData.last();
            }
        }
        querytext += id;
        q.prepare(querytext);

        if (!q.exec()) {
            qDebug() << get(NAME) + "Cannot execute query:" << querytext;
            ret = false;
        }
        else{
            qDebug() << get(NAME) + "executed query:" << querytext;
            qDebug() << get(NAME) + q.lastQuery();
            ret = true;
        }
    }
    else {
        return ret;
    }
    return ret;
}

/**
 * @brief Database::planningOperation Operationen an der Plannings-Tabelle (INSERT, UPDATE, DELETE)
 * @param data Benötigte Informationen für eine Operation
 * @param operationType (INSERT, UPDATE, DELETE)
 * @return onFail = false, onSuccess = true => notwendig für ggf. Speicherung der DB.
 */
int Database::planningOperation(QStringList data, int operationType)
{
    bool ret = false;
    QSqlQuery q(m_databaseGeneral.getDB());

    if (operationType == INSERT) {

        QString validPeriod;
        QString weekdays;
        QString validTime;
        QString type;
        int duration = 0;
        int id_playlist = 0;
        int fromGroup = 0;
        int id_content = 0;
        int id_order = 0;

        foreach (QString singleData, data) {
            QStringList splittedData = singleData.split("=");
            if (splittedData.first() == "validPeriod")
                validPeriod = splittedData.last();
            if (splittedData.first() == "weekdays")
                weekdays = splittedData.last();
            if (splittedData.first() == "validTime")
                validTime = splittedData.last();
            if (splittedData.first() == "type")
                type = splittedData.last();
            if (splittedData.first() == "id_playlist")
                id_playlist = splittedData.last().toInt();
            if (splittedData.first() == "id_content")
                id_content = splittedData.last().toInt();
            if (splittedData.first() == "id_order")
                id_order = splittedData.last().toInt();
            if (splittedData.first() == "duration")
                duration = splittedData.last().toInt();
        }

        // get fromgroup
        QString querytext = QString("SELECT Playlists.fromgroup FROM Playlists WHERE Playlists.id_playlist =" + QString::number(id_playlist));
        q.prepare(querytext);

        if (!q.exec()) {
            qDebug() << get(NAME) + "Cannot execute query:" << querytext;
        }
        else{
            while (q.next()) {
                fromGroup = q.value(0).toInt();
            }
        }

        querytext = QString("INSERT INTO Planning (valid_period,weekdays,valid_time,type,content,fromgroup,fromplaylist,id_order,duration) "
                            "VALUES (:valid_period, :weekdays, :valid_time, :type, :content, :fromgroup, :fromplaylist, :id_order, :duration)");
        q.prepare(querytext);

        q.bindValue(":valid_period", validPeriod);
        q.bindValue(":weekdays", weekdays);
        q.bindValue(":valid_time", validTime);
        q.bindValue(":type", type);
        q.bindValue(":fromgroup", fromGroup);
        q.bindValue(":fromplaylist", id_playlist);
        q.bindValue(":content", id_content);
        q.bindValue(":id_order", id_order);
        q.bindValue(":duration", duration);

        if (!q.exec()) {
            qDebug() << get(NAME) + "Cannot execute query:" << querytext;
            ret = false;
        }
        else{
            qDebug() << get(NAME) + "executed query:" << querytext;
            ret = true;
        }
    }
    else if (operationType == UPDATE) {

        QString validPeriod;
        QString weekdays;
        QString validTime;
        int id_planning = 0;
        QString type;
        int duration = 0;
        int id_playlist = 0;
        int fromGroup = 0;
        int id_content = 0;
        int id_order = 0;

        foreach (QString singleData, data) {
            QStringList splittedData = singleData.split("=");
            if (splittedData.first() == "validPeriod")
                validPeriod = splittedData.last();
            if (splittedData.first() == "weekdays")
                weekdays = splittedData.last();
            if (splittedData.first() == "validTime")
                validTime = splittedData.last();
            if (splittedData.first() == "type")
                type = splittedData.last();
            if (splittedData.first() == "id_playlist")
                id_playlist = splittedData.last().toInt();
            if (splittedData.first() == "id_content")
                id_content = splittedData.last().toInt();
            if (splittedData.first() == "id_planning")
                id_planning = splittedData.last().toInt();
            if (splittedData.first() == "id_order")
                id_order = splittedData.last().toInt();
            if (splittedData.first() == "duration")
                duration = splittedData.last().toInt();
        }

        // get fromgroup
        QString querytext = QString("SELECT Playlists.fromgroup FROM Playlists WHERE Playlists.id_playlist =" + QString::number(id_playlist));
        q.prepare(querytext);

        if (!q.exec()) {
            qDebug() << get(NAME) + "Cannot execute query:" << querytext;
        }
        else{
            while (q.next()) {
                fromGroup = q.value(0).toInt();
            }
        }

        querytext = QString("UPDATE Planning SET valid_period=:valid_period, weekdays=:weekdays, valid_time=:valid_time, type=:type, content=:content, fromgroup=:fromgroup, fromplaylist=:fromplaylist, id_order=:id_order, duration=:duration WHERE id_planning=:id_planning");
        q.prepare(querytext);

        q.bindValue(":valid_period", validPeriod);
        q.bindValue(":weekdays", weekdays);
        q.bindValue(":valid_time", validTime);
        q.bindValue(":type", type);
        q.bindValue(":fromgroup", fromGroup);
        q.bindValue(":fromplaylist", id_playlist);
        q.bindValue(":content", id_content);
        q.bindValue(":id_planning", id_planning);
        q.bindValue(":id_order", id_order);
        q.bindValue(":duration", duration);


        if (!q.exec()) {
            qDebug() << get(NAME) + "Cannot execute query:" << querytext;
            ret = false;
        }
        else{
            qDebug() << get(NAME) + "executed query:" << querytext;
            ret = true;
        }
    }
    else if (operationType == DELETE) {
        QString querytext = QString("DELETE FROM Planning WHERE id_planning=");


        QString id;
        foreach (QString singleData, data) {
            QStringList splittedData = singleData.split("=");
            if (splittedData.first() == "id") {
                id = splittedData.last();
            }
        }
        querytext += id;
        q.prepare(querytext);

        if (!q.exec()) {
            qDebug() << get(NAME) + "Cannot execute query:" << querytext;
            ret = false;
        }
        else{
            qDebug() << get(NAME) + "executed query:" << querytext;
            qDebug() << get(NAME) + q.lastQuery();
            ret = true;
        }
    }
    else {
        return ret;
    }
    return ret;
}

/**
 * @brief Database::contentOperation Operationen an der Content-Tabelle (INSERT, UPDATE, DELETE)
 * @param data Benötigte Informationen für eine Operation
 * @param operationType (INSERT, UPDATE, DELETE)
 * @return onFail = false, onSuccess = true => notwendig für ggf. Speicherung der DB.
 */
int Database::contentOperation(QStringList data, int operationType)
{
    Q_UNUSED(data)
    Q_UNUSED(operationType)
    return 0;
}
