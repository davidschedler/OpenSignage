#include "include/managerconfig.h"

/**
 * @brief ManagerConfig::ManagerConfig Default-Konstruktor
 * @param parent QObject
 */
ManagerConfig::ManagerConfig(QObject *parent) :
    QObject(parent)
{

}

/**
 * @brief ManagerConfig::getMac Hole MAC
 * @return MAC
 */
QString ManagerConfig::getMac() const
{
    return m_mac;
}

/**
 * @brief ManagerConfig::setMac Setze MAC
 * @param mac MAC
 */
void ManagerConfig::setMac(const QString &mac)
{
    m_mac = mac;
}

/**
 * @brief ManagerConfig::getRefreshTimer Hole RefreshTimer
 * @return
 */
QString ManagerConfig::getRefreshTimer() const
{
    return refreshTimer;
}

/**
 * @brief ManagerConfig::setRefreshTimer Setze RefreshTimer
 * @param value
 */
void ManagerConfig::setRefreshTimer(const QString &value)
{
    refreshTimer = value;
}

