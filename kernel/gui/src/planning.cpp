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

#include "include/planning.h"

/**
 * @brief Planning::Planning Default-Konstruktor
 * @param parent QObject
 */
Planning::Planning(QObject *parent) :
    QObject(parent)
{

}

/**
 * @brief Planning::Planning Default-Konstruktor für eine gültige Planung
 * @param id Planungs-ID
 * @param id_content ID des Contents, welcher der Planung zugeordnet ist
 * @param contentName Name des Contents, welcher der Planung zugeordnet ist
 * @param validPeriod Gültiger Zeitraum (Datum) der Planung
 * @param weekdays Gültige Wochentage der Planung
 * @param validTime Gültiger Zeitraum (Zeit) der Planung
 * @param type Planungs-Typ (Standard, Event)
 * @param id_order Planungs-Sortierungsnummer
 * @param duraiton Planungs-Zeit
 * @param parent QObject
 */
Planning::Planning(int id, int id_content, QString contentName, QString validPeriod, QString weekdays, QString validTime, QString type, int id_order, int duraiton,QObject *parent)  :
    QObject(parent),
    m_id(id),
    m_id_content(id_content),
    m_contentName(contentName),
    m_validPeriod(validPeriod),
    m_weekdays(weekdays),
    m_validTime(validTime),
    m_type(type),
  m_id_order(id_order),
  m_duration(duraiton)
{

}

/**
 * @brief Planning::setId Setze Planungs-ID
 * @param id Planungs-ID
 */
void Planning::setId(int id)
{
    if (m_id == id)
        return;

    m_id = id;
    emit idChanged(m_id);
}

/**
 * @brief Planning::setId_content Setze Content-ID
 * @param id_content Content-ID
 */
void Planning::setId_content(int id_content)
{
    if (m_id_content == id_content)
        return;

    m_id_content = id_content;
    emit id_contentChanged(m_id_content);
}

/**
 * @brief Planning::setContentName Setze Content-Name
 * @param contentName Content-Name
 */
void Planning::setContentName(QString contentName)
{
    if (m_contentName == contentName)
        return;

    m_contentName = contentName;
    emit contentNameChanged(m_contentName);
}

/**
 * @brief Planning::setValidPeriod Setze gültigen Zeitraum (Datum) der Plangung
 * @param validPeriod Planungs Zeitraums (Datum)
 */
void Planning::setValidPeriod(QString validPeriod)
{
    if (m_validPeriod == validPeriod)
        return;

    m_validPeriod = validPeriod;
    emit validPeriodChanged(m_validPeriod);
}

/**
 * @brief Planning::setWeekdays Setze gültige Wochentage der Planung
 * @param weekdays gültige Wochentage
 */
void Planning::setWeekdays(QString weekdays)
{
    if (m_weekdays == weekdays)
        return;

    m_weekdays = weekdays;
    emit weekdaysChanged(m_weekdays);
}

/**
 * @brief Planning::setValidTime Setze gültigen Zeitraum (Zeit) der Planung
 * @param validTime Planungs Zeitraum (Zeit)
 */
void Planning::setValidTime(QString validTime)
{
    if (m_validTime == validTime)
        return;

    m_validTime = validTime;
    emit validTimeChanged(m_validTime);
}

/**
 * @brief Planning::setType Setze Typ der Planung
 * @param type Planungs-Typ
 */
void Planning::setType(QString type)
{
    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged(m_type);
}

/**
 * @brief Planning::setId_order Setze Sortierungsnummer
 * @param id_order Sortierungsnummer
 */
void Planning::setId_order(int id_order)
{
    if (m_id_order == id_order)
        return;

    m_id_order = id_order;
    emit id_orderChanged(m_id_order);
}

/**
 * @brief Planning::setDuration Setze Laufzeit der Planung
 * @param duration Laufzeit der Planung
 */
void Planning::setDuration(int duration)
{
    if (m_duration == duration)
        return;

    m_duration = duration;
    emit durationChanged(m_duration);
}
