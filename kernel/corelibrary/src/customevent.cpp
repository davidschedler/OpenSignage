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

#include "customevent.h"

/**
 * @brief CustomEvent::CustomEvent Default-Konstruktor
 */
CustomEvent::CustomEvent() :
    QEvent (QEvent::Type(QEvent::User+1))
{

}

/**
 * @brief CustomEvent::CustomEvent Konstruktor auf sich selbst
 * @param mySelf
 */
CustomEvent::CustomEvent(CustomEvent *mySelf) :
    QEvent (QEvent::Type(QEvent::User+1))
{
    if (mySelf)
        p_customEvent = mySelf->p_customEvent;
}

/**
 * @brief CustomEvent::~CustomEvent Default-Destruktor
 */
CustomEvent::~CustomEvent()
{

}

/**
 * @brief CustomEvent::addCustomEvent Füge benutzerdefiniertes Event hinzu
 * @param key Event-Key (Anweisung)
 * @param value Event-Value (Daten)
 */
void CustomEvent::addCustomEvent(QString key, QString value)
{
    p_customEvent.insert(key, value);
}

/**
 * @brief CustomEvent::delCustomEvent Lösche benutzerdefiniertes Event
 * @param key Event-Key (Anweisung)
 */
void CustomEvent::delCustomEvent(QString key)
{
    p_customEvent.remove(key);
}

/**
 * @brief CustomEvent::getCustomEvent Hole Daten von benutzerdefiniertes Event
 * @param key Event-Key (Anweisung)
 * @param value Event-Value (Daten)
 * @return Daten des benutzerdefinierten Events anhand des Event-Keys
 */
QString CustomEvent::getCustomEvent(QString key, QString value)
{
    if (p_customEvent.contains(key))
        return p_customEvent.value(key);
    else
        return value;
}

/**
 * @brief CustomEvent::getCustomEventList Hole Liste aller benutzerdefiniertens Event-Keys
 * @return alle Event-Keys
 */
QStringList CustomEvent::getCustomEventList()
{
    QStringList list;
    QString key;
    foreach (key, p_customEvent.keys())
    {
        list.append(key);
    }
    return list;
}

/**
 * @brief CustomEvent::containsCustomEvent Prüfe ob benutzerdefiniertes Event den Event-Key beinhaltet
 * @param key Event-Key
 * @return onFail = false, onSuccess = true
 */
bool CustomEvent::containsCustomEvent(QString key)
{
    if (p_customEvent.contains(key))
        return true;
    else
        return false;
}

/**
 * @brief CustomEvent::isCustomEventEmpty Prüfe ob benutzerdefiniertes Event leer ist
 * @return empty = false, !empty = true
 */
bool CustomEvent::isCustomEventEmpty()
{
    return p_customEvent.isEmpty();
}
