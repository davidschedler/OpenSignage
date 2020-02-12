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

#include "downloadserver.h"

DownloadServer::DownloadServer()
{

}

DownloadServer::~DownloadServer()
{
    settings->sync();
    m_config->deleteLater();
    m_sslTcpServer->deleteLater();
}

QString DownloadServer::get(int type) const
{
    switch (type) {
    case PID:
        return "0006";
    case NAME:
        return "DownloadServer";
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

int DownloadServer::onInit(int type)
{
    switch (type) {
    case FIRSTINIT:
    {
        /* plugin general */
        m_config = new DownloadServerConfig(this);
        corelibrary->subscribeEvent("DOWNLOAD_CONTENT", this);

        loadSettings();

        qInfo() << get(NAME) << "init done!";
    }
        break;
    case EXTENDEDINIT:
    {
        /* ssltcpserver operations */
        m_serverThread = new QThread;
        m_sslTcpServer = new SslTcpServer();
        m_sslTcpServer->moveToThread(m_serverThread);
        connect(m_serverThread, SIGNAL(started()), m_sslTcpServer, SLOT(process()));
        connect(m_sslTcpServer, SIGNAL(finished()), m_serverThread, SLOT(quit()));
        connect(m_sslTcpServer, SIGNAL(finished()), m_sslTcpServer, SLOT(deleteLater()));
        connect(m_serverThread, SIGNAL(finished()), m_serverThread, SLOT(deleteLater()));
        m_serverThread->start();
    }
        break;
    case EXIT:
        break;
    default:
        break;
    }
    return 0;
}

bool DownloadServer::onEvent(CustomEvent *customEvent)
{
    QString print;
    if (!customEvent->getCustomEvent(COMMAND).isEmpty())
    {
        QString command = customEvent->getCustomEvent(COMMAND);
        QString type = customEvent->getCustomEvent(TYPE);
        QString data = customEvent->getCustomEvent(DATA);
        if (command == "DOWNLOAD_CONTENT") {

            QByteArray byteMessage;
            if (type == "CONTENT") {
                QFile file(corelibrary->getAppDir().absolutePath() + "/content_uploaded/"+ customEvent->getCustomEvent(DATA));
                if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                    return false;
                byteMessage = file.readAll();
                qDebug() << customEvent->getCustomEvent(COMMAND);
            }

            //m_webSocket.sendBinaryMessage(byteMessage);
            qDebug() << customEvent->getCustomEvent(COMMAND);
        }
    }
    return false;
}

void DownloadServer::loadSettings()
{
    settings = new QSettings(get(PSETTINGS),QSettings::IniFormat);

    settings->beginGroup(get(NAME));
    if (!settings->contains("port"))
        settings->setValue("port","5678");
    m_config->setPort(settings->value("port").toString().toUShort());

    if (!settings->contains("socket_url"))
        settings->setValue("socket_url","localhost");
    m_config->setSocketUrl(settings->value("socket_url").toString());
    settings->endGroup();

    settings->sync();
}

void DownloadServer::saveSettings()
{
    settings->beginGroup(get(NAME));
    settings->setValue("port",m_config->port());
    settings->setValue("socket_url",m_config->socketUrl());
    settings->sync();
    settings->endGroup();
}





