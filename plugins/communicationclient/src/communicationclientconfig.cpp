#include "include/communicationclientconfig.h"

CommunicationClientConfig::CommunicationClientConfig(QObject *parent) : QObject(parent)
{

}

quint16 CommunicationClientConfig::port() const
{
    return m_port;
}

void CommunicationClientConfig::setPort(const quint16 &port)
{
    m_port = port;
}

QString CommunicationClientConfig::socketUrl() const
{
    return m_socketUrl;
}

void CommunicationClientConfig::setSocketUrl(const QString &socketUrl)
{
    m_socketUrl = socketUrl;
}

