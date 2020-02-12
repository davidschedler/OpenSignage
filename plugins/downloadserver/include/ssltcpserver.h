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
