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

#include "include/managerconfig.h"

/**
 * @brief ManagerConfig::ManagerConfig Default-Konstruktor
 * @param parent QObject
 */
ManagerConfig::ManagerConfig(QObject *parent) :
    QObject(parent)
{

}

/**
 * @brief ManagerConfig::getMac Hole MAC
 * @return MAC
 */
QString ManagerConfig::getMac() const
{
    return m_mac;
}

/**
 * @brief ManagerConfig::setMac Setze MAC
 * @param mac MAC
 */
void ManagerConfig::setMac(const QString &mac)
{
    m_mac = mac;
}

/**
 * @brief ManagerConfig::getRefreshTimer Hole RefreshTimer
 * @return
 */
QString ManagerConfig::getRefreshTimer() const
{
    return refreshTimer;
}

/**
 * @brief ManagerConfig::setRefreshTimer Setze RefreshTimer
 * @param value
 */
void ManagerConfig::setRefreshTimer(const QString &value)
{
    refreshTimer = value;
}

