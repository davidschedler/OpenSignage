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
