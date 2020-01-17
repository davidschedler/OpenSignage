#ifndef DOWNLOADSERVERCONFIG_H
#define DOWNLOADSERVERCONFIG_H

#include <QObject>

class DownloadServerConfig : public QObject
{
    Q_OBJECT
public:
    explicit DownloadServerConfig(QObject *parent = nullptr);

    quint16 port() const;
    void setPort(const quint16 &port);

    QString socketUrl() const;
    void setSocketUrl(const QString &socketUrl);

private:
    quint16 m_port;
    QString m_socketUrl;
};

#endif // DOWNLOADSERVERCONFIG_H
