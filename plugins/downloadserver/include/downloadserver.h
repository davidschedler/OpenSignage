#ifndef DOWNLOADSERVER_H
#define DOWNLOADSERVER_H

#include <QTcpServer>
#include <QSslSocket>
#include <QThread>
#include <QSslKey>
#include <QSslCertificate>
#include <QUrl>

#include "plugininterface.h"
#include "corelibrary.h"
#include "downloadserverconfig.h"
#include "downloadserver_global.h"
#include "ssltcpserver.h"

class DOWNLOADSERVERSHARED_EXPORT DownloadServer : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "DownloadServer_iid")
    Q_INTERFACES(PluginInterface)

public:
    DownloadServer();
    ~DownloadServer();

    /* interface implementation */
    QString get(int type) const;
    int onInit(int type);
    bool onEvent(CustomEvent *customEvent);
    void loadSettings();
    void saveSettings();

Q_SIGNALS:


private Q_SLOTS:

private:
    /* qsslsocket handling */
    SslTcpServer *m_sslTcpServer;
    QThread *m_serverThread;

    /* settings */
    DownloadServerConfig *m_config;
};

#endif // DOWNLOADSERVER_H
