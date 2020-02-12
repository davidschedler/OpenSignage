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

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QDebug>
#include <QPluginLoader>
#include <QtPlugin>
#include <QDir>

#include "plugininterface.h"

QT_FORWARD_DECLARE_CLASS(CoreLibrary);
QT_FORWARD_DECLARE_CLASS(PluginInterface);

class PluginManager : public QObject
{
    Q_OBJECT

public:
    explicit PluginManager(CoreLibrary *corelibrary, QObject *parent = nullptr);

    /* gerneral */
    void setupDirectories();

    /* load plugins */
    void loadAllPlugins(QString pluginLocation);
    void loadPlugin(QString fileName);

    /* plugin handling */
    PluginInterface* getPlugin(QString pid);
    bool containsPlugin(QString pid);
    QList<PluginInterface *> getAllPlugins();

private:
    /* plugin handling*/
    CoreLibrary            *m_coreLibrary;
    QList<PluginInterface*> m_pluginList;
};

#endif // PLUGINMANAGER_H
