#ifndef DATABASECONFIG_H
#define DATABASECONFIG_H

#include <QObject>

class DatabaseConfig : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseConfig(QObject *parent = nullptr);

    QString dbPath() const;
    void setDbPath(const QString &dbPath);

    QString connectionName() const;
    void setConnectionName(const QString &connectionName);

private:
    QString m_dbPath;
    QString m_connectionName;
};

#endif // DATABASECONFIG_H
