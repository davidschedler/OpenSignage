#include "include/communicationserverconfig.h"

/**
 * @brief CommunicationServerConfig::CommunicationServerConfig Default-Konstruktor
 * @param parent QObject
 */
CommunicationServerConfig::CommunicationServerConfig(QObject *parent) :
    QObject(parent)
{

}

/**
 * @brief CommunicationServerConfig::port Hole Port
 * @return Port
 */
quint16 CommunicationServerConfig::port() const
{
    return m_port;
}

/**
 * @brief CommunicationServerConfig::setPort Setze Port
 * @param port Port
 */
void CommunicationServerConfig::setPort(const quint16 &port)
{
    m_port = port;
}

/**
 * @brief CommunicationServerConfig::socketName Hole Socketname
 * @return Socketname
 */
QString CommunicationServerConfig::socketName() const
{
    return m_socketName;
}

/**
 * @brief CommunicationServerConfig::setSocketName Setze Socketname
 * @param socketName Socketname
 */
void CommunicationServerConfig::setSocketName(const QString &socketName)
{
    m_socketName = socketName;
}
