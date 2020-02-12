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
