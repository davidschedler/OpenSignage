#include "include/downloadserverconfig.h"

DownloadServerConfig::DownloadServerConfig(QObject *parent) : QObject(parent)
{

}

quint16 DownloadServerConfig::port() const
{
    return m_port;
}

void DownloadServerConfig::setPort(const quint16 &port)
{
    m_port = port;
}

QString DownloadServerConfig::socketUrl() const
{
    return m_socketUrl;
}

void DownloadServerConfig::setSocketUrl(const QString &socketUrl)
{
    m_socketUrl = socketUrl;
}

