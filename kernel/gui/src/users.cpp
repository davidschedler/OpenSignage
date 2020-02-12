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

#include "include/users.h"

/**
 * @brief Users::Users Default-Konstruktor
 * @param parent
 */
Users::Users(QObject *parent) :
    QObject(parent)
{

}

/**
 * @brief Users::Users Default-Konstruktor für einen gültigen Benutzer
 * @param id Benutzer-ID
 * @param firstName Benutzer-Vorname
 * @param lastName Benutzer-Nachname
 * @param groupName Benutzer-Gruppenzugehörigkeit
 * @param groupType Benutzer-Gruppentyp
 * @param eMail Benutzer-eMail
 * @param parent QObject
 */
Users::Users(int id, QString firstName, QString lastName, QString groupName, QString groupType, QString eMail, QObject *parent) :
    QObject(parent),
    m_id(id),
    m_firstName(firstName),
    m_lastName(lastName),
    m_groupName(groupName),
    m_groupType(groupType),
    m_eMail(eMail)
{

}

/**
 * @brief Users::~Users Lösche Benutzer
 */
Users::~Users()
{
 this->deleteLater();
}

/**
 * @brief Users::setId Setze Benutzer-ID
 * @param id Benutzer-ID
 */
void Users::setId(int id)
{
    if (m_id == id)
        return;

    m_id = id;
    emit idChanged(m_id);
}

/**
 * @brief Users::setFirstName Setze Benutzer-Vorname
 * @param firstName Benutzer-Vorname
 */
void Users::setFirstName(QString firstName)
{
    if (m_firstName == firstName)
        return;

    m_firstName = firstName;
    emit firstNameChanged(m_firstName);
}

/**
 * @brief Users::setLastName Setze Benutzer-Nachname
 * @param lastName Benutzer-Nachname
 */
void Users::setLastName(QString lastName)
{
    if (m_lastName == lastName)
        return;

    m_lastName = lastName;
    emit lastNameChanged(m_lastName);
}

/**
 * @brief Users::setGroupName Setze Benutzer-Gruppenzugehörigkeit
 * @param groupName Benutzer-GruppenName
 */
void Users::setGroupName(QString groupName)
{
    if (m_groupName == groupName)
        return;

    m_groupName = groupName;
    emit groupNameChanged(m_groupName);
}

/**
 * @brief Users::setGroupType Setze Benutzer-Gruppentyp
 * @param groupType Benutzer-Gruppentyp
 */
void Users::setGroupType(QString groupType)
{
    if (m_groupType == groupType)
        return;

    m_groupType = groupType;
    emit groupTypeChanged(m_groupType);
}

/**
 * @brief Users::setEMail Setze Benutzer-eMail
 * @param eMail Benutzer-eMail
 */
void Users::setEMail(QString eMail)
{
    if (m_eMail == eMail)
        return;

    m_eMail = eMail;
    emit eMailChanged(m_eMail);
}
