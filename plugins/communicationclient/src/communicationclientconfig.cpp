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

