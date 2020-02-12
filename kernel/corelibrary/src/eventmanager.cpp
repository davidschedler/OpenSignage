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

#include "eventmanager.h"

/**
 * @brief EventManager::EventManager Default-Konstruktor
 * @param pluginManager Pointer auf den PluginManger
 * @param parent QObject
 */
EventManager::EventManager(PluginManager *pluginManager, QObject *parent) :
    QObject(parent),
    m_pluginManager(pluginManager)
{

}

/**
 * @brief EventManager::addEvent Füge Event der Eventloop hinzu
 * @param customEvent Benutzerdefiniertes Event
 */
void EventManager::addEvent(CustomEvent *customEvent)
{
    PluginInterface *sender = nullptr;
    QString senderName = "unknown";

    sender = m_pluginManager->getPlugin(customEvent->getCustomEvent(SENDER));
    if (sender)
        senderName = sender->get(NAME);

    QList<PluginInterface *> receivers;
    QList<PluginInterface *> *list = m_subscriptions.value(customEvent->getCustomEvent(COMMAND));
    if (list)
    {
        PluginInterface *plugin = nullptr;
        foreach (plugin, *list)
        {
            CustomEvent *task = new CustomEvent(*customEvent);
            QCoreApplication::postEvent((QObject*)plugin, task);
            receivers << plugin;
        }
    }
}

/**
 * @brief EventManager::subscribeEvent Füge Empfänger für Abonnement hinzu
 * @param message Thema (Command)
 * @param receiver Plugin, welches Thema abonnieren möchte
 */
void EventManager::subscribeEvent(QString message, PluginInterface *receiver)
{
    if (!m_subscriptions.contains(message))
        m_subscriptions.insert(message, new QList<PluginInterface *>());

    m_subscriptions.value(message)->append(receiver);
}

/**
 * @brief EventManager::subscribeEvents Füge Empfänger für Abonnement hinzu
 * @param messages Themen (Commands)
 * @param receiver Plguins, welches Themen abonnieren möchten
 */
void EventManager::subscribeEvents(QStringList messages, PluginInterface *receiver)
{
    QString message;
    foreach (message, messages)
    {
        subscribeEvent(message, receiver);
    }
}
