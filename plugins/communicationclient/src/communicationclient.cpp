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

#include "communicationclient.h"

CommunicationClient::CommunicationClient() :
    m_webSocket(nullptr),
    //m_url(QStringLiteral("ws://localhost:1234")),
    m_url(""),
    m_config(nullptr)
{

}

CommunicationClient::~CommunicationClient()
{
    settings->sync();
    m_config->deleteLater();
    m_webSocket.deleteLater();
}

QString CommunicationClient::get(int type) const
{
    switch (type) {
    case PID:
        return "0003";
    case NAME:
        return "CommunicationClient";
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

int CommunicationClient::onInit(int type)
{
    switch (type) {
    case FIRSTINIT:
    {
        /* plugin general */
        m_config = new CommunicationClientConfig(this);
        corelibrary->subscribeEvent("GETDATA", this);

        loadSettings();

        qInfo() << get(NAME) << "init done!";
    }
        break;
    case EXTENDEDINIT:
    {
        /* websocket operations */
        qDebug() << "WebSocket server:" << m_url;
        connect(&m_webSocket, &QWebSocket::connected, this, &CommunicationClient::onConnected);
        connect(&m_webSocket, &QWebSocket::disconnected, this, &CommunicationClient::closed);
        m_url = "ws://" + m_config->socketUrl() + ":" + QString::number(m_config->port());
        QNetworkRequest request{m_url};
        request.setRawHeader("Sec-WebSocket-Protocol", "binary");
        m_webSocket.open(request);
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

bool CommunicationClient::onEvent(CustomEvent *customEvent)
{
    QString print;
    if (!customEvent->getCustomEvent(COMMAND).isEmpty())
    {
        QString command = customEvent->getCustomEvent(COMMAND);
        QString type = customEvent->getCustomEvent(TYPE);
        QString data = customEvent->getCustomEvent(DATA);
        if (command == "GETDATA") {

            qInfo() << get(NAME) + " onEvent: " + "Command: " + command  + " Type: " + type;

            QByteArray byteMessage;
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

            m_webSocket.sendBinaryMessage(byteMessage);
            qInfo() << "Sending message to: " << m_url;
        }
    }
    return false;
}

void CommunicationClient::loadSettings()
{
    settings = new QSettings(get(PSETTINGS),QSettings::IniFormat);

    settings->beginGroup(get(NAME));
    if (!settings->contains("port"))
        settings->setValue("port","1234");
    m_config->setPort(settings->value("port").toString().toUShort());

    if (!settings->contains("socket_url"))
        settings->setValue("socket_url","localhost");
    m_config->setSocketUrl(settings->value("socket_url").toString());
    settings->endGroup();

    qInfo() << get(NAME) + " Loaded settings: " + "port: " + QString::number(m_config->port()) + " socket_url: " + m_config->socketUrl();

    settings->sync();
}

void CommunicationClient::saveSettings()
{
    settings->beginGroup(get(NAME));
    settings->setValue("port",m_config->port());
    settings->setValue("url",m_config->socketUrl());
    settings->sync();
    settings->endGroup();

    qInfo() << get(NAME) + " Saved settings: " + "port: " + QString::number(m_config->port()) + " socket_url: " + m_config->socketUrl();

}

void CommunicationClient::onConnected()
{
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &CommunicationClient::onTextMessageReceived);
    connect(&m_webSocket, &QWebSocket::binaryMessageReceived,
            this, &CommunicationClient::onBinaryMessageReceived);

    qInfo() << get(NAME) + " connected!";
}

void CommunicationClient::onTextMessageReceived(QString message)
{
    Q_UNUSED(message)
    qInfo() << get(NAME) + " Text Message received from: " + m_config->socketUrl();
}

void CommunicationClient::onBinaryMessageReceived(QByteArray message)
{
    qInfo() << get(NAME) + " Binary Message received from: "  + m_config->socketUrl();

    CustomEvent cEvent;
    cEvent.addCustomEvent(SENDER,get(NAME));
    cEvent.addCustomEvent(COMMAND,"INCOMMINGDATA");
    if (message.contains("http://"))
        cEvent.addCustomEvent(TYPE,"DOWNLOAD_URL");
    else
        cEvent.addCustomEvent(TYPE,"DATA");
    cEvent.addCustomEvent(DATA,message);
    corelibrary->addEvent(&cEvent);

}

