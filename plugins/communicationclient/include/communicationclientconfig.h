#ifndef COMMUNICATIONCLIENTCONFIG_H
#define COMMUNICATIONCLIENTCONFIG_H

#include <QObject>

class CommunicationClientConfig : public QObject
{
    Q_OBJECT
public:
    explicit CommunicationClientConfig(QObject *parent = nullptr);

    quint16 port() const;
    void setPort(const quint16 &port);

    QString socketUrl() const;
    void setSocketUrl(const QString &socketUrl);

private:
    quint16 m_port;
    QString m_socketUrl;
};

#endif // COMMUNICATIONCLIENTCONFIG_H
