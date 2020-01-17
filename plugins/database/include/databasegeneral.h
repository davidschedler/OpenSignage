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
