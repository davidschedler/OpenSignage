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

#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <QObject>
#include <QHash>
#include <QCoreApplication>

#include "customevent.h"
#include "plugininterface.h"
#include "pluginmanager.h"

QT_FORWARD_DECLARE_CLASS(CustomEvent);
QT_FORWARD_DECLARE_CLASS(PluginManagerInterface);
QT_FORWARD_DECLARE_CLASS(PluginManager);

class EventManager : public QObject
{
    Q_OBJECT

public:
    explicit EventManager(PluginManager *pluginManager, QObject *parent = nullptr);

    /* event handling */
    void addEvent(CustomEvent *customEvent);
    void subscribeEvent(QString message, PluginInterface *receiver);
    void subscribeEvents(QStringList messages, PluginInterface *receiver);

private:
    /* plugin handling */
    PluginManager  *m_pluginManager;

    // Plugins can subscribe to certain MediaEvents.
    // This hash table holds for every message a list of plugin UUIDs, which subscribed to the message.
    QHash<QString, QList<PluginInterface*> *> m_subscriptions;
};

#endif // EVENTMANAGER_H
