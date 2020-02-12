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

#ifndef CORELIBRARY_H
#define CORELIBRARY_H

#define ERROR   0
#define WARNING 1
#define SUCCESS 2

#define CORE    "CORE"

#include <QObject>

#include "customevent.h"
#include "eventmanager.h"
#include "plugininterface.h"
#include "pluginmanager.h"
#include "corelibrary_global.h"

QT_FORWARD_DECLARE_CLASS(CustomEvent);

class CORELIBRARYSHARED_EXPORT CoreLibrary : public QObject
{
    Q_OBJECT

    friend class CustomEvent;

public:
    explicit CoreLibrary(QString appPath, QObject *parent = nullptr);

    /* gerneral */
    QDir getAppDir();

    /* init plugins */
    void initPlugins(QString pluginLocation);

    /* event handling */
    void subscribeEvent(QString message, PluginInterface *plugin);
    void subscribeEvents(QStringList messages, PluginInterface *plugin);
    void addEvent(CustomEvent *customEvent);

private:
    /* general */
    QString m_appPath;

    /* corelibrary handling */
    PluginManager   *m_pluginManager;
    EventManager    *m_eventManager;
};

#endif // CORELIBRARY_H
