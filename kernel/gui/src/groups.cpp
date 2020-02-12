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

#include "include/groups.h"

/**
 * @brief Groups::Groups Default-Konstruktor
 * @param parent QObject
 */
Groups::Groups(QObject *parent) :
    QObject(parent)
{

}

/**
 * @brief Groups::Groups Default-Konstruktor für eine gültige Gruppe
 * @param id Gruppen-ID
 * @param groupName Gruppen-Name
 * @param groupType Gruppen-Typ
 * @param id_groupType Gruppen-ID_GruppenTyp
 * @param parent QObject
 */
Groups::Groups(int id, QString groupName, QString groupType, int id_groupType, QObject *parent) :
    QObject(parent),
    m_id(id),
    m_groupName(groupName),
    m_groupType(groupType),
    m_id_grouptype(id_groupType)
{

}

/**
 * @brief Groups::~Groups Default-Destuktor
 */
Groups::~Groups()
{
    this->deleteLater();
}

/**
 * @brief Groups::getUserById Hole Benutzer by seiner ID von eine Gruppe
 * @param id Benutzer-ID
 * @return onFail = nullptr, onSuccess = User*
 */
Users *Groups::getUserById(int id)
{
    Users *ret = nullptr;
    for (int i = 0 ; i < m_usersList.size() ; i++){
        Users *temp = (Users*) m_usersList.at(i);
        if (temp->id() == id) {
            ret = temp;
            break;
        }
    }
    return ret;
}

/**
 * @brief Groups::addUser Füge Benutzer der Gruppe hinzu
 * @param addUser Pointer auf einen Benutzer
 */
void Groups::addUser(Users *addUser)
{
    m_usersList.append(addUser);
    emit usersListChanged(m_usersList);
}

/**
 * @brief Groups::deleteUser Lösche Benutzer an der stelle der usersList
 * @param idx Stelle die zu löschen ist
 */
void Groups::deleteUser(int idx)
{
    m_usersList.removeAt(idx);
    emit usersListChanged(m_usersList);
}

/**
 * @brief Groups::clearUserList Lösche alle Benutzer
 */
void Groups::clearUserList()
{
    m_usersList.clear();
}

/**
 * @brief Groups::setId Setze die Gruppen-ID
 * @param id Gruppen-ID
 */
void Groups::setId(int id)
{
    if (m_id == id)
        return;

    m_id = id;
    emit idChanged(m_id);
}

/**
 * @brief Groups::setGroupName Setze den Gruppen-Name
 * @param groupName Gruppen-Name
 */
void Groups::setGroupName(QString groupName)
{
    if (m_groupName == groupName)
        return;

    m_groupName = groupName;
    emit groupNameChanged(m_groupName);
}

/**
 * @brief Groups::setGroupType Setze den Gruppen-Typ
 * @param groupType Gruppen-Typ
 */
void Groups::setGroupType(QString groupType)
{
    if (m_groupType == groupType)
        return;

    m_groupType = groupType;
    emit groupTypeChanged(m_groupType);
}

/**
 * @brief Groups::setUserInGroup Setze Anzahl der Benutzer in der Gruppe
 * @param userInGroup Anzahl der Benutzer in der Gruppe
 */
void Groups::setUserInGroup(int userInGroup)
{
    if (m_userInGroup == userInGroup)
        return;

    m_userInGroup = userInGroup;
    emit userInGroupChanged(m_userInGroup);
}

/**
 * @brief Groups::setUsersList Setze eine komplette Benutzerliste
 * @param usersList Benutzerliste
 */
void Groups::setUsersList(QList<QObject *> usersList)
{
    if (m_usersList == usersList)
        return;

    m_usersList = usersList;
    emit usersListChanged(m_usersList);
}

/**
 * @brief Groups::setId_grouptype Setze die ID des Gruppen-Typs
 * @param id_grouptype ID-Gruppentyp
 */
void Groups::setId_grouptype(int id_grouptype)
{
    if (m_id_grouptype == id_grouptype)
        return;

    m_id_grouptype = id_grouptype;
    emit id_grouptypeChanged(m_id_grouptype);
}
