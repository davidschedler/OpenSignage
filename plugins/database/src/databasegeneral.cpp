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

#include "databasegeneral.h"
/**
 * @brief Default-Konstruktor
 * @param parent QObject
 */
DatabaseGeneral::DatabaseGeneral(QObject *parent) :
    QObject(parent)
{
    m_filename = "C:\test.db";
}


/**
 * @brief Defailt-Destruktor
 */
DatabaseGeneral::~DatabaseGeneral()
{
    if (m_database.isOpen())
        m_database.close();
}


/**
 * @brief Erstelle eine Datenbank im Speicher
 * @param databaseName Name der Datenbank
 * @return onFail = -1, onSuccess = 0
 */
int DatabaseGeneral::initDatabase(QString databaseName)
{
    m_database = QSqlDatabase::addDatabase("QSQLITE", databaseName);
    m_database.setDatabaseName(":memory:");
    if (!m_database.open()) {
        qDebug("Cannot Create database in memory!");
        return -1;
    }
    return 0;
}


/**
 * @brief Starte sqlMemoryDBLoadorSave() Funktion im Speicher-Modus
 * @param fileUrl Pfad zur Datenbank
 * @return 0
 */
int DatabaseGeneral::saveDatabaseToFile(QString fileUrl)
{
    //QUrl url(fileUrl);
    //QString urlString = "file://" + fileUrl;

    sqlMemoryDBLoadorSave(m_database, fileUrl, false);
    m_filename = fileUrl;
    return 0;
}


/**
 * @brief Starte sqlMemoryDBLoadorSave() Funktion im Lade-Modus
 * @param fileUrl Pfad zur Datenbank
 * @return onFail = -1, onSuccess = 0
 */
int DatabaseGeneral::loadDatabaseFromFile(QString fileUrl)
{
    QUrl url(fileUrl);
    QString urlString = url.toLocalFile();

    if (!QFile::exists(urlString))
    {
        qDebug() << "File does not exist! FileURL: " << urlString;
        return -1;
    }

    sqlMemoryDBLoadorSave(m_database, urlString, true);
    m_filename = fileUrl;
    return 0;
}


/**
 * @brief Lade Datenbank via QML - C++ Interaktion
 * @param fileUrl Pfad zur Datenbank
 * @return onFail = -1, onSuccess = 0
 */
int DatabaseGeneral::importDataBase(QString fileUrl)
{
    return loadDatabaseFromFile(fileUrl);
}


/**
 * @brief Datenbank via QML - C++ Interaktion. Starte saveDatabaseToFile()
 * @param fileUrl Pfad Datei
 * @return onFail = -1, onSuccess = 0
 */
int DatabaseGeneral::exportDataBase(QString fileUrl)
{
    return saveDatabaseToFile(fileUrl);
}


/**
 * @brief Füge Entry zur Datenbank hinzu.
 * @param sqlStatement SQL-Query
 * @return onFail = -1, onSuccess = 0
 */
int DatabaseGeneral::addDatabaseEntry(QString sqlStatement)
{
    QSqlQuery query(m_database);

    if(!query.exec(sqlStatement)) {
        qDebug() << "Cannot add database entry. Statement ="  << sqlStatement;
        return -1;
    }
    return 0;
}

/**
 * @brief Lösche Entry aus Datenbank
 * @param sqlStatement SQL-Query
 * @return onFail = -1, onSuccess = 0
 */
int DatabaseGeneral::removeDatabaseEntry(QString sqlStatement)
{
    QSqlQuery query(m_database);

    if(!query.exec(sqlStatement)) {
        qDebug() << "Cannot remove database entry. Statement ="  << sqlStatement;
        return -1;
    }
    return 0;
}


/**
 * @brief Erstelle Datenbank Tabelle
 * @param sqlQuery SQL-Query
 * @return onFail = -1, onSuccess = 0
 */
int DatabaseGeneral::createDatabaseTable(QString sqlQuery)
{
    QSqlQuery query(m_database);

    if(!query.exec(sqlQuery)) {
        qDebug() << "Cannot create table. Statement ="  << sqlQuery;
        qDebug() << m_database.lastError().type();
        return -1;
    }

    return 0;
}


/**
 * @brief Starte Datenbank Transaktion. Jedes Query ist im Speicher bis es hinzugefügt wurde.
 * @return onFail = -1, onSuccess = 0
 */
int DatabaseGeneral::beginTransaction()
{
    return m_database.transaction();
}

/**
 * @brief Bestätige ein Transaktion
 * @return onFail = -1, onSuccess = 0
 */
int DatabaseGeneral::commitTransaction()
{
    return m_database.commit();
}


/**
 * @brief Mache Transaktion rückgängig
 * @return onFail = -1, onSuccess = 0
 */
int DatabaseGeneral::rollback()
{
    return m_database.rollback();
}


/**
 * @brief Lade oder Speichere eine Datenbank von/in ein File
 * @param memdb Datenbank
 * @param filedb_path Pfad zur Datenbank
 * @param Lade = True, Save = False
 */
void DatabaseGeneral::sqlMemoryDBLoadorSave(QSqlDatabase& memdb, const QString& filedb_path, bool load)
{
    if (!load)
        QFile::remove (filedb_path); //drop old database

    {
        QSqlDatabase filedb = QSqlDatabase::addDatabase ("QSQLITE", "tmp_file_db");
        filedb.setDatabaseName (filedb_path);
        filedb.open();
        QSqlDatabase *fromDB = load ? &filedb : &memdb;
        QSqlDatabase *toDB = load ? &memdb : &filedb;
        QSqlQuery fromQuery (*fromDB);
        QSqlQuery toQuery (*toDB);
        QStringList tableList;
        //Copy the tables
        fromQuery.exec ("SELECT sql, tbl_name FROM sqlite_master "
                        " WHERE type='table' AND sql NOT NULL and name NOT IN ('sqlite_stat1', 'sqlite_sequence')");

        while (fromQuery.next())
        {
            toQuery.exec (fromQuery.value (0).toString());
            tableList.push_back (fromQuery.value (1).toString());
        }

        //Attach db
        memdb.exec (QString ("ATTACH DATABASE '%1' as source").arg (filedb_path));
        //Copy records
        memdb.exec ("BEGIN");

        if (load)
        {
            foreach (QString tableName, tableList)
                memdb.exec (QString ("insert into main.%1 select * from source.%1").arg (tableName));
        }
        else
        {
            foreach (QString tableName, tableList)
                memdb.exec (QString ("insert into source.%1 select * from main.%1").arg (tableName));
        }

        memdb.exec ("COMMIT");
        //Detach
        memdb.exec ("DETACH DATABASE source");
        //Optimization
        toDB->exec ("PRAGMA case_sensitive_like=true;");
        toDB->exec ("PRAGMA journal_mode=MEMORY;");
        toDB->exec ("PRAGMA temp_store=MEMORY;");
        toDB->exec ("PRAGMA locking_mode=EXCLUSIVE;");
        toDB->exec ("PRAGMA synchronous = OFF;");
        toDB->exec ("PRAGMA encoding = \"UTF-8\";");
        //Copy the triggers
        fromQuery.exec ("SELECT sql FROM sqlite_master "
                        " WHERE type='trigger' AND sql NOT NULL and name NOT IN ('sqlite_stat1', 'sqlite_sequence')");

        while (fromQuery.next())
            toQuery.exec (fromQuery.value (0).toString());

        //Copy the indexes
        fromQuery.exec ("SELECT sql FROM sqlite_master "
                        " WHERE type='index' AND sql NOT NULL and name NOT IN ('sqlite_stat1', 'sqlite_sequence')");

        while (fromQuery.next())
            toQuery.exec (fromQuery.value (0).toString());

        //Copy the views
        fromQuery.exec ("SELECT sql FROM sqlite_master "
                        " WHERE type='view' AND sql NOT NULL and name NOT IN ('sqlite_stat1', 'sqlite_sequence')");

        while (fromQuery.next())
            toQuery.exec (fromQuery.value (0).toString());

        toDB->exec ("PRAGMA case_sensitive_like=false;");

        filedb.close();
    }
    QSqlDatabase::removeDatabase("tmp_file_db");
}

/**
 * @brief Kopiere Datenbank von Quelle nach Ziel
 * @param srcdb Quelle (Datenbank)
 * @param destdb Ziel (Datenbank)
 */
void DatabaseGeneral::sqlDBCopy(QSqlDatabase& srcdb, QSqlDatabase& destdb)
{
    QSqlDatabase *fromDB = &srcdb;
    QSqlDatabase *toDB = &destdb;
    QSqlQuery fromQuery (*fromDB);
    QSqlQuery toQuery (*toDB);
    QStringList tableList;
    //Copy the tables
    fromQuery.exec ("SELECT sql, tbl_name FROM sqlite_master "
                    " WHERE type='table' AND sql NOT NULL and name NOT IN ('sqlite_stat1', 'sqlite_sequence')");

    while (fromQuery.next())
    {
        toQuery.exec (fromQuery.value (0).toString());
        tableList.push_back (fromQuery.value (1).toString());
    }

    //Copy records
    toDB->exec ("BEGIN");
    fromQuery.setForwardOnly (true);
    foreach (QString tableName, tableList)
    {
        fromQuery.exec (QString ("select * from %1").arg (tableName));

        if (fromQuery.next())
        {
            int cols = fromQuery.record().count();

            if (cols != 0)
            {
                std::vector<QVariantList> rec;
                rec.resize (cols);
                QString tmp = "?";

                for (int i = 1; i < cols; ++i)
                    tmp.append (",?");

                toQuery.prepare (QString ("insert into %1 values(").arg (tableName).append (tmp).append (")"));

                for (int i = 0; i < cols; ++i)
                    rec[i] << fromQuery.value (i);

                while (fromQuery.next())
                    for (int i = 0; i < cols; ++i)
                        rec[i] << fromQuery.value (i);

                for (int i = 0; i < cols; ++i)
                    toQuery.addBindValue (rec[i]);

                toQuery.execBatch();
            }
        }
    }
    fromQuery.finish();
    toDB->exec ("COMMIT");
    //Optimization
    toDB->exec ("PRAGMA case_sensitive_like=true;");
    toDB->exec ("PRAGMA journal_mode=MEMORY;");
    toDB->exec ("PRAGMA temp_store=MEMORY;");
    toDB->exec ("PRAGMA locking_mode=EXCLUSIVE;");
    toDB->exec ("PRAGMA synchronous = OFF;");
    toDB->exec ("PRAGMA encoding = \"UTF-8\";");
    //Copy the triggers
    fromQuery.exec ("SELECT sql FROM sqlite_master "
                    " WHERE type='trigger' AND sql NOT NULL and name NOT IN ('sqlite_stat1', 'sqlite_sequence')");

    while (fromQuery.next())
        toQuery.exec (fromQuery.value (0).toString());

    //Copy the indexes
    fromQuery.exec ("SELECT sql FROM sqlite_master "
                    " WHERE type='index' AND sql NOT NULL and name NOT IN ('sqlite_stat1', 'sqlite_sequence')");

    while (fromQuery.next())
        toQuery.exec (fromQuery.value (0).toString());

    //Copy the views
    fromQuery.exec ("SELECT sql FROM sqlite_master "
                    " WHERE type='view' AND sql NOT NULL and name NOT IN ('sqlite_stat1', 'sqlite_sequence')");

    while (fromQuery.next())
        toQuery.exec (fromQuery.value (0).toString());

    toDB->exec ("PRAGMA case_sensitive_like=false;");
}

/**
 * @brief Hole Datenbank-Verbindung
 * @return QSqlDatabase Referenz zur Datenbank-Verbindung
 */
QSqlDatabase &DatabaseGeneral::getDB()
{
    return m_database;
}
