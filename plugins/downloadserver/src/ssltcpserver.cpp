/* Copyright (C) 2020  David Schedler (https://github.com/davidschedler/)
 *
 * This file is part of OpenSigange
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "include/ssltcpserver.h"
#include <QSslSocket>
#include <QFile>

SslTcpServer::SslTcpServer(QObject *parent) :
    QTcpServer(parent)
{

}

void SslTcpServer::process()
{
    QFile keyFile("/home/david/Downloads/QSslSocket-example-master/certificates/red_local.key");
    keyFile.open(QIODevice::ReadOnly);
    key = QSslKey(keyFile.readAll(), QSsl::Rsa);
    keyFile.close();

    QFile certFile("/home/david/Downloads/QSslSocket-example-master/certificates/red_local.pem");
    certFile.open(QIODevice::ReadOnly);
    cert = QSslCertificate(certFile.readAll());
    certFile.close();

    if (!listen(QHostAddress("127.0.0.1"), 12345)) { // FQDN in red_local.pem is set to 127.0.0.1.  If you change this, it will not authenticate.
        qCritical() << "Unable to start the TCP server";
        //exit(0);
    }
    else {
        qInfo("\"TcpServer\" listining on port 12345");
    }
    connect(this, &SslTcpServer::newConnection, this, &SslTcpServer::link);
}

void SslTcpServer::incomingConnection(qintptr socketDescriptor)
{
    QSslSocket *sslSocket = new QSslSocket(this);

    connect(sslSocket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(sslErrors(QList<QSslError>)));
    sslSocket->setSocketDescriptor(socketDescriptor);
    sslSocket->setPrivateKey(key);
    sslSocket->setLocalCertificate(cert);
    sslSocket->setPeerVerifyMode(QSslSocket::VerifyNone);
    sslSocket->startServerEncryption();

    addPendingConnection(sslSocket);
}

void SslTcpServer::sslErrors(const QList<QSslError> &errors)
{
    foreach (const QSslError &error, errors)
        qInfo() << error.errorString();
}

void SslTcpServer::link()
{
    QTcpSocket *clientSocket;

    clientSocket = nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &SslTcpServer::rx);
    connect(clientSocket, &QTcpSocket::disconnected, this, &SslTcpServer::disconnected);
}

void SslTcpServer::rx()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    qInfo() << clientSocket->readAll();
    clientSocket->write("Server says Hello");
}

void SslTcpServer::disconnected()
{
    qInfo("Client Disconnected");
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    clientSocket->deleteLater();
}
