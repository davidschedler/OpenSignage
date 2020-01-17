#include "include/databaseconfig.h"

/**
 * @brief DatabaseConfig::DatabaseConfig Default-Konstruktor
 * @param parent
 */
DatabaseConfig::DatabaseConfig(QObject *parent) :
    QObject(parent)
{

}

/**
 * @brief DatabaseConfig::dbPath Hole Datenbank-Pfad
 * @return Datenbank-Path
 */
QString DatabaseConfig::dbPath() const
{
    return m_dbPath;
}

/**
 * @brief DatabaseConfig::setDbPath Setze Datenbank-Pfad
 * @param dbPath Datenbank-Pfad
 */
void DatabaseConfig::setDbPath(const QString &dbPath)
{
    m_dbPath = dbPath;
}

/**
 * @brief DatabaseConfig::connectionName Hole Datenbank Verbindungsname
 * @return Datenbank Verbindungsname
 */
QString DatabaseConfig::connectionName() const
{
    return m_connectionName;
}

/**
 * @brief DatabaseConfig::setConnectionName Setze Datenbank Verbindungsname
 * @param connectionName Datenbank Verbindungsname
 */
void DatabaseConfig::setConnectionName(const QString &connectionName)
{
    m_connectionName = connectionName;
}
