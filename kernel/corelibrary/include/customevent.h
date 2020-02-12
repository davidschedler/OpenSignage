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

#ifndef CUSTOMEVENT_H
#define CUSTOMEVENT_H

#include <QEvent>
#include <QHash>

#define SENDER      "SENDER"
#define COMMAND     "COMMAND"
#define TYPE        "TYPE"
#define IDENT       "IDENT"
#define PWD         "PWD"
#define DATA        "DATA"

class Q_DECL_EXPORT CustomEvent : public QEvent
{

public:
    CustomEvent();
    CustomEvent(CustomEvent *mySelf);

    virtual ~CustomEvent();

    /* creation and deleation */
    void addCustomEvent(QString key, QString value);
    void delCustomEvent(QString key);

    /* task handling */
    QString getCustomEvent(QString key, QString value = "");
    QStringList getCustomEventList();
    bool containsCustomEvent(QString key);
    bool isCustomEventEmpty();

protected:
    /* structure of a task => value => key */
    QHash<QString, QString> p_customEvent;
};

#endif // CUSTOMEVENT_H
