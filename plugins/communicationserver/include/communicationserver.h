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

#ifndef COMMUNICATIONSERVER_H
#define COMMUNICATIONSERVER_H

#include <QWebSocket>
#include <QWebSocketServer>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>


#include "plugininterface.h"
#include "corelibrary.h"
#include "communicationserverconfig.h"
#include "communicationserver_global.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class COMMUNICATIONSERVERSHARED_EXPORT CommunicationServer : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Communication_iid")
    Q_INTERFACES(PluginInterface)

public:
    CommunicationServer();
    ~CommunicationServer();

    /* interface implementation */
    QString get(int type) const;
    int onInit(int type);
    bool onEvent(CustomEvent *customEvent);
    void loadSettings();
    void saveSettings();

Q_SIGNALS:
    /* websocket signals */
    void closed();

private Q_SLOTS:
    /* websocket slots */
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();

    /* helper functions */
    QWebSocket* findWebSocket(QString mac);
    QByteArray generateByteMessage(QString command, QString type, QString data = "");


private:
    /* websocket handling */
    QWebSocketServer    *m_webSocketServer;
    QList<QWebSocket *>  m_clients;

    /* settings */
    CommunicationServerConfig *m_config;
};

#endif // COMMUNICATIONSERVER_H
