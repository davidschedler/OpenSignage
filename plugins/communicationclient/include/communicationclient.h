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
