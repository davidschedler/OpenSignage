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
