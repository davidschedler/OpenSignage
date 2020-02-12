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

#ifndef DOWNLOADCLIENT_H
#define DOWNLOADCLIENT_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QThread>

#include "plugininterface.h"
#include "corelibrary.h"
#include "downloadclientconfig.h"
#include "downloadclient_global.h"

class DOWNLOADCLIENTSHARED_EXPORT DownloadClient : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "DownloadClient_iid")
    Q_INTERFACES(PluginInterface)

public:
    DownloadClient();
    ~DownloadClient();

    /* interface implementation */
    QString get(int type) const;
    int onInit(int type);
    bool onEvent(CustomEvent *customEvent);
    void loadSettings();
    void saveSettings();

Q_SIGNALS:
    void downloadCompleted(QString fileName);
    void downloadError(QString fileName);

public Q_SLOTS:
    void downloadPlaylistItem(QString fileName, QString url);

private Q_SLOTS:
    void slotReadyRead();
    void slotRequestFinished(QNetworkReply* reply);
    void slotError(QNetworkReply::NetworkError err);
    void slotSslErrors(QList<QSslError> sslErr);

private:
    /* qsslsocket handling */
    QNetworkAccessManager *m_networkManager;
    QNetworkReply *m_reply;
    QFile m_file;
    QString m_fileName;
    QString m_md5;

    /* settings */
    DownloadClientConfig *m_config;
};

#endif // DOWNLOADCLIENT_H
