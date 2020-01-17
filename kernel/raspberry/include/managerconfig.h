#ifndef MANAGERCONFIG_H
#define MANAGERCONFIG_H

#include <QObject>

class ManagerConfig : public QObject
{
    Q_OBJECT
public:
    explicit ManagerConfig(QObject *parent = nullptr);

    QString getMac() const;
    void setMac(const QString &mac);
    QString getRefreshTimer() const;
    void setRefreshTimer(const QString &value);

signals:

public slots:

private:
    QString m_mac;
    QString refreshTimer;
};

#endif // MANAGERCONFIG_H
