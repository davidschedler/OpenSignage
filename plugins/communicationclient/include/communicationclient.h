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

#ifndef COMMUNICATIONCLIENT_H
#define COMMUNICATIONCLIENT_H

#include <QWebSocket>
#include <QWebSocketServer>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>


#include "plugininterface.h"
#include "corelibrary.h"
#include "communicationclientconfig.h"
#include "communicationclient_global.h"

class COMMUNICATIONCLIENTSHARED_EXPORT CommunicationClient : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "CommunicationClient_iid")
    Q_INTERFACES(PluginInterface)

public:
    CommunicationClient();
    ~CommunicationClient();

    /* interface implementation */
    QString get(int type) const;
    int onInit(int type);
    bool onEvent(CustomEvent *customEvent);
    void loadSettings();
    void saveSettings();

Q_SIGNALS:
    void closed();
    void incommingMessage(QByteArray message);

private Q_SLOTS:
    /* websocket handling */
    void onConnected();
    void onTextMessageReceived(QString message);
    void onBinaryMessageReceived(QByteArray message);

private:
    /* websocket handling */
    QWebSocket m_webSocket;
    QUrl m_url;

    /* settings */
    CommunicationClientConfig *m_config;
};

#endif // COMMUNICATIONCLIENT_H
