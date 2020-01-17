#ifndef COMMUNICATIONSERVERCONFIG_H
#define COMMUNICATIONSERVERCONFIG_H

#include <QObject>

class CommunicationServerConfig : public QObject
{
    Q_OBJECT
public:
    explicit CommunicationServerConfig(QObject *parent = nullptr);

    quint16 port() const;
    void setPort(const quint16 &port);

    QString socketName() const;
    void setSocketName(const QString &socketName);

private:
    quint16 m_port;
    QString m_socketName;
};

#endif // COMMUNICATIONSERVERCONFIG_H
