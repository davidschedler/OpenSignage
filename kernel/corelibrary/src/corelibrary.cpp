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

#include "corelibrary.h"

/**
 * @brief CoreLibrary::CoreLibrary Default-Konstruktor
 * @param appPath Programmpfad
 * @param parent QObject
 */
CoreLibrary::CoreLibrary(QString appPath, QObject *parent) :
    QObject (parent),
    m_appPath(appPath),
    m_pluginManager(new PluginManager(this)),
    m_eventManager(new EventManager(m_pluginManager))
{

}

/**
 * @brief CoreLibrary::getAppDir Hole Programmpfad
 * @return Programmpfad
 */
QDir CoreLibrary::getAppDir()
{
    return m_appPath;
}

/**
 * @brief CoreLibrary::initPlugins PluginManager lade Plugin
 * @param pluginLocation Pluginpfad
 */
void CoreLibrary::initPlugins(QString pluginLocation)
{
    m_pluginManager->loadAllPlugins(pluginLocation);
}

/**
 * @brief CoreLibrary::subscribeEvent Plugin abonniert Thema (Command)
 * @param message Thema (Command)
 * @param plugin Plugin, welches ein Thema abonnieren möchte
 */
void CoreLibrary::subscribeEvent(QString message, PluginInterface *plugin)
{
    m_eventManager->subscribeEvent(message, plugin);
}

/**
 * @brief CoreLibrary::subscribeEvents Plugin abonniert Themen (Commands)
 * @param messages Themen (Commands)
 * @param plugin Plugins, welches Themen abonnieren möchte
 */
void CoreLibrary::subscribeEvents(QStringList messages, PluginInterface *plugin)
{
    QString message;
    foreach (message, messages)
    {
        m_eventManager->subscribeEvent(message, plugin);
    }
}

/**
 * @brief CoreLibrary::addEvent EventManager füge Benutzerdefiniertes Event der Eventloop hinzu.
 * @param customEvent Benutzerdefiniertes Event
 */
void CoreLibrary::addEvent(CustomEvent *customEvent)
{
    m_eventManager->addEvent(customEvent);
}
