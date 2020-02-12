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

#ifndef DATABASE_H
#define DATABASE_H

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QProcess>

#include "databaseconfig.h"
#include "databasegeneral.h"
#include "plugininterface.h"
#include "corelibrary.h"
#include "database_global.h"
#include "filewatcher.h"

class DATABASESHARED_EXPORT Database : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Database_iid")
    Q_INTERFACES(PluginInterface)

public:
    Database();
    ~Database();

    enum DataBaseOperation {
        GET=0,
        INSERT,
        UPDATE,
        DELETE,
        UNKNOWN
    };

    /*interface implementation */
    QString get(int type) const;
    int onInit(int type);
    bool onEvent(CustomEvent *customEvent);
    void loadSettings();
    void saveSettings();

public slots:
    void addNewContentToDB(QStringList newContent);

private:
    /* settings */
    DatabaseConfig *m_config;

    /* db handling */
    DatabaseGeneral     m_databaseGeneral;
    int createInitialTables();
    int testFillTables();

    /* FIles handling */
    FileWatcher *m_fileWatcher;
    QThread *m_fileWatcherThread;

    /* helper functions */
    QJsonDocument generateJsonDocument(QString command, QString array, QList<QStringList> data);
    QStringList generateDataList(QString data);

    QList<QStringList> getAuth(QString ident, QString pwd);
    QList<QStringList> getAuthPi(QString ident);

    QString getGroupFromIdent(QString ident);
    QString getUserIdFromIdent(QString ident);
    QString expandQueryTextWithGroup(QString querytext, QString group, QString id_user);

    QList<QStringList> getUsers(QString ident);
    QList<QStringList> getGroups(QString ident);
    QList<QStringList> getPlaylists(QString ident);
    QList<QStringList> getPlannings(QString ident);
    QList<QStringList> getHardware(QString ident);
    QList<QStringList> getContent(QString ident);
    QList<QStringList> getClientPlaylist(QString ident);
    QList<QStringList> getDownloadUrlFromFile(QString ident, QString id_content);

    int groupOperation(QStringList data, int operationType);
    int userOperation(QStringList data, int operationType);
    int hardwareOperation(QStringList data, int operationType);
    int playlistOperation(QStringList data, int operationType);
    int planningOperation(QStringList data, int operationType);
    int contentOperation(QStringList data, int operationType);
};

#endif // DATABASE_H
