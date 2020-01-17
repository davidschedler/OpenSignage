#ifndef SSLTCPSERVER_H
#define SSLTCPSERVER_H

#include <QTcpServer>
#include <QSslKey>
#include <QSslCertificate>

class SslTcpServer : public QTcpServer
{
    Q_OBJECT

public:
    SslTcpServer(QObject *parent = nullptr);

public slots:
    void process();

signals:
    void finished();
    void error(QString err);

private:
    QSslKey key;
    QSslCertificate cert;

private slots:
    void sslErrors(const QList<QSslError> &errors);
    void link();
    void rx();
    void disconnected();

protected:
    void incomingConnection(qintptr socketDescriptor);
};

#endif // SSLTCPSERVER_H
