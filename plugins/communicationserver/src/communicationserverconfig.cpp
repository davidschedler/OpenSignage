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

#include "include/communicationserverconfig.h"

/**
 * @brief CommunicationServerConfig::CommunicationServerConfig Default-Konstruktor
 * @param parent QObject
 */
CommunicationServerConfig::CommunicationServerConfig(QObject *parent) :
    QObject(parent)
{

}

/**
 * @brief CommunicationServerConfig::port Hole Port
 * @return Port
 */
quint16 CommunicationServerConfig::port() const
{
    return m_port;
}

/**
 * @brief CommunicationServerConfig::setPort Setze Port
 * @param port Port
 */
void CommunicationServerConfig::setPort(const quint16 &port)
{
    m_port = port;
}

/**
 * @brief CommunicationServerConfig::socketName Hole Socketname
 * @return Socketname
 */
QString CommunicationServerConfig::socketName() const
{
    return m_socketName;
}

/**
 * @brief CommunicationServerConfig::setSocketName Setze Socketname
 * @param socketName Socketname
 */
void CommunicationServerConfig::setSocketName(const QString &socketName)
{
    m_socketName = socketName;
}
