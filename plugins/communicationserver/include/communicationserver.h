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
