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

#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QtCore/QDebug>
#include <QNetworkRequest>
#include <QTimer>

#include "groups.h"

class WebSocketClient : public QObject
{
    Q_OBJECT

public:
    explicit WebSocketClient(const QUrl &url, QObject *parent = nullptr);

    /* gui requests */
    void getDatabaseData(QByteArray message);

signals:
    /* to inform engine */
    void disconnected();
    void connected();
    void parsedGroup(QObject *groupList);
    void incommingMessage(QByteArray message);

private slots:
    /* websocket handling */
    void onConnected();
    void onTextMessageReceived(QString message);
    void onBinaryMessageReceived(QByteArray message);
    void tryToReconnect();

private:
    /* websocket handling */
    QWebSocket m_webSocket;
    QUrl m_url;
    QTimer m_timer;
};

#endif // WEBSOCKETCLIENT_H
