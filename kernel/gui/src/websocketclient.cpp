#include "websocketclient.h"

/**
 * @brief WebSocketClient::WebSocketClient Default-Konstruktor
 * @param url Verbindungs-Url des Websockets
 * @param parent QObject
 */
WebSocketClient::WebSocketClient(const QUrl &url, QObject *parent) :
    QObject(parent),
    m_url(url)
{
    qDebug() << "WebSocket server:" << url;
    connect(&m_webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(&m_webSocket, &QWebSocket::connected, this, &WebSocketClient::connected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &WebSocketClient::disconnected);

    connect(&m_webSocket, &QWebSocket::disconnected, this, &WebSocketClient::tryToReconnect);
    connect(&m_timer, &QTimer::timeout, this, &WebSocketClient::tryToReconnect);

    QNetworkRequest request{url};
    request.setRawHeader("Sec-WebSocket-Protocol", "binary");
    m_webSocket.open(request);

    m_timer.setSingleShot(true);
    m_timer.start(7000);
}

/**
 * @brief WebSocketClient::getDatabaseData Schickt eine binäre Nachricht über den Websocket an den Server
 * @param message
 */
void WebSocketClient::getDatabaseData(QByteArray message)
{
    m_webSocket.sendBinaryMessage(message);
}

/**
 * @brief WebSocketClient::onConnected Erfolgreicher Verbindungsaufbau mit dem Websocketserver
 */
void WebSocketClient::onConnected()
{
    qDebug() << "WebSocket connected";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &WebSocketClient::onTextMessageReceived);
    connect(&m_webSocket, &QWebSocket::binaryMessageReceived,
            this, &WebSocketClient::onBinaryMessageReceived);
    //m_webSocket.sendBinaryMessage(QByteArray("getAuth,David.Schedler,pass123"));
}

/**
 * @brief WebSocketClient::onTextMessageReceived Eingehende textuelle-Nachricht
 * @param message textuelle-Nachricht
 */
void WebSocketClient::onTextMessageReceived(QString message)
{
    qDebug() << "Message received:" << message;
}

/**
 * @brief WebSocketClient::onBinaryMessageReceived Eingehende binäre-Nachricht
 * @param message binäre-Nachricht
 */
void WebSocketClient::onBinaryMessageReceived(QByteArray message)
{
    emit incommingMessage(message);
}

/**
 * @brief WebSocketClient::tryToReconnect Bei Verbindungsabbruch erneuter Verbindungsaufbau alle 5 Sekunden
 */
void WebSocketClient::tryToReconnect()
{
    if (m_webSocket.isValid())
        return;

    QNetworkRequest request{m_url};
    request.setRawHeader("Sec-WebSocket-Protocol", "binary");
    m_webSocket.open(request);
    m_timer.start(5000);
}
