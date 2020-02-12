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

#ifndef DATABASEGENERAL_H
#define DATABASEGENERAL_H

#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QObject>
#include <QFile>
#include <QDir>

#include <qdebug.h>

class DatabaseGeneral : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseGeneral(QObject *parent = nullptr);
    ~DatabaseGeneral();

    int initDatabase(QString databaseName);
    int saveDatabaseToFile(QString fileUrl);
    int loadDatabaseFromFile(QString fileUrl);
    int importDataBase(QString fileUrl);
    int exportDataBase(QString fileUrl);
    int addDatabaseEntry(QString sqlStatement);
    int removeDatabaseEntry(QString sqlStatement);
    int createDatabaseTable(QString sqlQuery);

    int beginTransaction();
    int commitTransaction();
    int rollback();

    void sqlMemoryDBLoadorSave(QSqlDatabase& memdb, const QString& filedb_path, bool load);
    void sqlDBCopy(QSqlDatabase& srcdb, QSqlDatabase& destdb);
    QSqlDatabase& getDB();


private:
    QSqlDatabase m_database;
    QString m_filename;
};

#endif // DATABASEGENERAL_H
