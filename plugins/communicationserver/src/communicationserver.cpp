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

#include "communicationserver.h"

/**
 * @brief CommunicationServer::CommunicationServer Standard-Konstruktor
 */
CommunicationServer::CommunicationServer() :
    m_webSocketServer(nullptr),
    m_config(nullptr)
{

}

/**
 * @brief CommunicationServer::~CommunicationServer Standard-Destruktor
 */
CommunicationServer::~CommunicationServer()
{
    settings->sync();
    m_config->deleteLater();
    m_webSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

/**
 * @brief CommunicationServer::get Plugin-spezifischer Getter (PID, NAME, PTYPE, PSETTINGS, VERSION)
 * @param type PluginInterface-Defines
 * @return Plugin-spezifische Informationen (PID, NAME, PTYPE, PSETTINGS, VERSION)
 */
QString CommunicationServer::get(int type) const
{
    switch (type) {
    case PID:
        return "0001";
    case NAME:
        return "CommunicationServer";
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
 * @brief CommunicationServer::onInit FirstInit => Abonniere die Events von der Corelibrary und lade die Settings. Speichere diese in die CommunicationServerConfig.
 * ExtendedInit => Starte den Websocket-Server
 * @param type PluginInterface-Defines
 * @return 0
 */
int CommunicationServer::onInit(int type)
{
    switch (type) {
    case FIRSTINIT:
    {
        /* plugin general */
        m_config = new CommunicationServerConfig(this);
        corelibrary->subscribeEvent("PUTDATA", this);

        loadSettings();
        qInfo() << get(NAME) << "first init done!";
    }
        break;
    case EXTENDEDINIT:
    {
        /* websocket init */
        m_webSocketServer = new QWebSocketServer(m_config->socketName(), QWebSocketServer::NonSecureMode, this);
        if (m_webSocketServer->listen(QHostAddress::Any, m_config->port()))
        {
            qInfo() << m_config->socketName() << " listening on port" << m_config->port();
            connect(m_webSocketServer, &QWebSocketServer::newConnection, this, &CommunicationServer::onNewConnection);
            connect(m_webSocketServer, &QWebSocketServer::closed, this, &CommunicationServer::closed);
        }
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
 * @brief CommunicationServer::onEvent Verarbeite bzw. bearbeite die Events, welche von der Corelibrary abonniert wurden.
 * @param customEvent Benutzerdefiniertes Events inkl. Arbeitsanweisungen.
 * @return false
 */
bool CommunicationServer::onEvent(CustomEvent *customEvent)
{
    QString print;
    if (!customEvent->getCustomEvent(COMMAND).isEmpty())
    {
        QString command = customEvent->getCustomEvent(COMMAND);
        QString type = customEvent->getCustomEvent(TYPE);
        QString data = customEvent->getCustomEvent(DATA);
        if (command == "PUTDATA") {

            qInfo() << get(NAME) + " onEvent: " + "Command: " + command  + " Type: " + type;

            QByteArray byteMessage;
            if (type == "AUTH") {
                byteMessage = customEvent->getCustomEvent(DATA).toUtf8().data();
            }
            if (type == "GROUPS") {
                byteMessage = customEvent->getCustomEvent(DATA).toUtf8().data();
            }
            if (type == "USERS") {
                byteMessage = customEvent->getCustomEvent(DATA).toUtf8().data();
            }
            if (type == "HARDWARE") {
                byteMessage = customEvent->getCustomEvent(DATA).toUtf8().data();
            }
            if (type == "CONTENT") {
                byteMessage = customEvent->getCustomEvent(DATA).toUtf8().data();
            }
            if (type == "PLAYLISTS") {
                byteMessage = customEvent->getCustomEvent(DATA).toUtf8().data();
            }
            if (type == "PLANNINGS") {
                byteMessage = customEvent->getCustomEvent(DATA).toUtf8().data();
            }
            if (type == "AUTH_PI") {
                byteMessage = customEvent->getCustomEvent(DATA).toUtf8().data();
            }
            if (type == "CLIENT_PLAYLIST") {
                byteMessage = customEvent->getCustomEvent(DATA).toUtf8().data();
            }
            if (type == "DOWNLOAD_URL") {
                byteMessage = customEvent->getCustomEvent(DATA).toUtf8().data();
            }

            qDebug() << get(NAME) + " onEvent: " + byteMessage;

            QWebSocket *pSocket = findWebSocket(customEvent->getCustomEvent(IDENT));
            if (!pSocket)
                return false;

            pSocket->sendBinaryMessage(byteMessage);

            qInfo() << "Sending message to : " + pSocket->objectName();
        }

    }
    return false;
}

/**
 * @brief CommunicationServer::loadSettings Lädt die Plugin-spezifischen Informationen (port, websocket_name)
 */
void CommunicationServer::loadSettings()
{
    settings = new QSettings(get(PSETTINGS),QSettings::IniFormat);

    settings->beginGroup(get(NAME));
    if (!settings->contains("port"))
        settings->setValue("port","1234");
    m_config->setPort(settings->value("port").toString().toUShort());

    if (!settings->contains("socket_name"))
        settings->setValue("socket_name","WebSocket");
    m_config->setSocketName(settings->value("socket_name").toString());
    settings->endGroup();

    qInfo() << get(NAME) + " Loaded settings: " + "port: " + QString::number(m_config->port()) + " socket_name: " + m_config->socketName();

    settings->sync();
}

/**
 * @brief CommunicationServer::saveSettings Speichert die Plugin-spezifischen Informationen (port, websocket_name)
 */
void CommunicationServer::saveSettings()
{
    settings->beginGroup(get(NAME));
    settings->setValue("port",m_config->port());
    settings->setValue("socketname",m_config->socketName());
    settings->sync();
    settings->endGroup();

    qInfo() << get(NAME) + " Saved settings: " + "port: " + QString::number(m_config->port()) + " socket_name: " + m_config->socketName();
}

/**
 * @brief CommunicationServer::onNewConnection Neue eingehende Verbindung zum WEbsockert-Server
 */
void CommunicationServer::onNewConnection()
{
    QWebSocket *pSocket = m_webSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &CommunicationServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &CommunicationServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &CommunicationServer::socketDisconnected);

    m_clients << pSocket;

    qInfo() << get(NAME) << " Client connected: " << pSocket->objectName();
}

/**
 * @brief CommunicationServer::processTextMessage Schicke neue Text-Nachricht an WebSocket
 * @param message
 */
void CommunicationServer::processTextMessage(QString message)
{
    Q_UNUSED(message);

    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qInfo() << get(NAME) + " Text Message received: " + pClient->objectName();
}

/**
 * @brief CommunicationServer::processBinaryMessage Schicke neue Binäre-Nachricht an WebSocket
 * @param message
 */
void CommunicationServer::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qInfo() << get(NAME) + " Binary Message received from: " + pClient->objectName();

    QJsonParseError *err = new QJsonParseError();
    QJsonDocument jDoc = QJsonDocument::fromJson(message, err);

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
        QJsonDocument dataDoc(mainObj.value("DATA").toObject());
        QString dataDocString = QString(dataDoc.toJson(QJsonDocument::Compact));
        QJsonObject dataObj = dataDoc.object();

        if (!mainObj.contains("COMMAND") && !mainObj.contains("TYPE")
                && !mainObj.contains("IDENT") && !mainObj.contains("DATA"))
        {
            qDebug() << get(NAME) + " QJsonDocument doesn't contains command, type, ident or data";
            return;
        }

        CustomEvent cEvent;
        cEvent.addCustomEvent(SENDER,get(NAME));
        cEvent.addCustomEvent(COMMAND,mainObj.value("COMMAND").toString());
        cEvent.addCustomEvent(TYPE,mainObj.value("TYPE").toString());
        cEvent.addCustomEvent(IDENT,mainObj.value("IDENT").toString());
        cEvent.addCustomEvent(DATA,dataDocString);
        if (mainObj.value("TYPE") == "AUTH") {
            pClient->setObjectName(mainObj.value("IDENT").toString());
            cEvent.addCustomEvent(PWD,dataObj.value("password").toString());
        }
        if (mainObj.value("TYPE") == "AUTH_PI") {
            pClient->setObjectName(mainObj.value("IDENT").toString());
        }
        corelibrary->addEvent(&cEvent);
    }
    //delete error?
}

/**
 * @brief CommunicationServer::socketDisconnected WebSocket hat Verbindung beendet
 */
void CommunicationServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qInfo() << get(NAME) + " Client disconnected: " << pClient->objectName();
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

/**
 * @brief CommunicationServer::findWebSocket Hilfsfunction, um Websocket zu finden an welchen eine Nachricht gesendet werden soll
 * @param mac MAC
 * @return onFail = nullptr, onSuccess = Websocket
 */
QWebSocket *CommunicationServer::findWebSocket(QString mac)
{
    QWebSocket *findSocket = nullptr;
    QWebSocket *temp;
    foreach(temp, m_clients)
    {
        if (temp->objectName() == mac)
            findSocket = temp;
    }
    return findSocket;
}

//wird nun alles in der db gemacht somit depricated bzw. moved
QByteArray CommunicationServer::generateByteMessage(QString command, QString type, QString data)
{
    QJsonObject jsonMainObj;
    jsonMainObj.insert("COMMAND", QJsonValue::fromVariant(command));
    jsonMainObj.insert("TYPE", QJsonValue::fromVariant(type));

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
