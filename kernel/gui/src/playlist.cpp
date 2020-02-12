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

#include "include/playlist.h"

/**
 * @brief Playlist::Playlist Default-Konstruktor
 * @param parent QObject
 */
Playlist::Playlist(QObject *parent) :
    QObject(parent)
{

}

/**
 * @brief Playlist::Playlist Default-Konstruktor einer gültigen Playliste
 * @param id Playlisten-ID
 * @param name Playlisten-Name
 * @param orientation Playlisten-Orientierung
 * @param groupName Playlisten-GruppenName
 * @param planedon Playlisten-Hardware (MACs)
 * @param parent QObject
 */
Playlist::Playlist(int id, QString name, QString orientation, QString groupName, QString planedon, QObject *parent) :
    QObject(parent),
    m_id(id),
    m_name(name),
    m_orientation(orientation),
    m_groupName(groupName),
    m_planedon(planedon)
{

}

/**
 * @brief Playlist::deletePlanning Lösche Planung an der Stelle der planningList
 * @param idx Stelle, welche zu löschen ist
 */
void Playlist::deletePlanning(int idx)
{
    m_planningList.removeAt(idx);
    emit planningListChanged(m_planningList);
}

/**
 * @brief Playlist::clearPlanningList Lösche alle Planungen
 */
void Playlist::clearPlanningList()
{
    m_planningList.clear();
}

/**
 * @brief Playlist::getPlanningById Hole eine Planung anhand ihrer ID aus der Playliste
 * @param id Planungs-ID
 * @return onFail = nullptr, onSuccess = Planning
 */
Planning *Playlist::getPlanningById(int id)
{
    Planning *ret = nullptr;
    for (int i = 0 ; i < m_planningList.size() ; i++){
        Planning *temp = (Planning*) m_planningList.at(i);
        if (temp->id() == id) {
            ret = temp;
            break;
        }
    }
    return ret;
}

/**
 * @brief Playlist::addPlanning Füge eine neue Planung hinzu
 * @param newPlanning Planungs-Pointer
 */
void Playlist::addPlanning(Planning *newPlanning)
{
    m_planningList.append(newPlanning);
    emit planningListChanged(m_planningList);
}

/**
 * @brief Playlist::swapPlanning Tausche die Sortierungs-IDs zweier Planungen in einer Playliste
 * @param oldIdx alte Planungs Sortierung
 * @param newIdx neue Planungs Sortierung
 */
void Playlist::swapPlanning(int oldIdx, int newIdx)
{
    Planning *save = (Planning*) m_planningList.at(newIdx);
    Planning *move = (Planning*) m_planningList.at(oldIdx);
    int save_order = save->id_order();
    int move_order = move->id_order();

    m_planningList.removeAt(newIdx);
    m_planningList.insert(oldIdx,save);

    save->setId_order(move_order);
    move->setId_order(save_order);

    emit planningListChanged(m_planningList);
}

/**
 * @brief Playlist::swapPlanningInformation Verpacke alle Informationen der vertauschen Planungen in eine Struktur, um Sie mit der Datenbank zu syncronisieren
 * @param target1 Planung 1
 * @param target2 Planung 2
 * @return Zusammengestellte Planungsstruktur für eine Datenbanksyncronisation
 */
QPair<QString, QString> Playlist::swapPlanningInformation(int target1, int target2)
{
    QPair<QString, QString> updatePair;
    QString up1String;
    QString up2String;

    Planning *update1 = (Planning*) m_planningList.at(target1);
    Planning *update2 = (Planning*) m_planningList.at(target2);

    up1String = "duration=" + QString::number(update1->duration()) + "|" +  "validPeriod=" + update1->validPeriod() + "|" + "weekdays=" + update1->weekdays() + "|" + "validTime=" + update1->validTime() + "|" + "type=" + update1->type() + "|" + "id_content=" + QString::number(update1->id_content()) + "|" + "id_order=" + QString::number(update1->id_order()) + "|" + "id_planning=" + QString::number(update1->id());
    up2String = "duration=" + QString::number(update2->duration()) + "|" +  "validPeriod=" + update2->validPeriod() + "|" + "weekdays=" + update2->weekdays() + "|" + "validTime=" + update2->validTime() + "|" + "type=" + update2->type() + "|" + "id_content=" + QString::number(update2->id_content()) + "|" + "id_order=" + QString::number(update2->id_order()) + "|" + "id_planning=" + QString::number(update2->id());

    updatePair.first = up1String;
    updatePair.second = up2String;

    return updatePair;
}

/**
 * @brief Playlist::setId Setze Playlist-ID
 * @param id Playlist-ID
 */
void Playlist::setId(int id)
{
    if (m_id == id)
        return;

    m_id = id;
    emit idChanged(m_id);
}

/**
 * @brief Playlist::setName Setze Playlist-Name
 * @param name Playlist-Name
 */
void Playlist::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

/**
 * @brief Playlist::setOrientation Setze Playlist-Orientierung
 * @param orientation Playlist-Orientierung
 */
void Playlist::setOrientation(QString orientation)
{
    if (m_orientation == orientation)
        return;

    m_orientation = orientation;
    emit orientationChanged(m_orientation);
}

/**
 * @brief Playlist::setGroupName Setze Playlist-GruppenName
 * @param groupName Playlist-GruppenName
 */
void Playlist::setGroupName(QString groupName)
{
    if (m_groupName == groupName)
        return;

    m_groupName = groupName;
    emit groupNameChanged(m_groupName);
}

/**
 * @brief Playlist::setPlanningList Setze eine komplette Planungsliste für die Playliste
 * @param planningList Planungsliste
 */
void Playlist::setPlanningList(QList<QObject *> planningList)
{
    if (m_planningList == planningList)
        return;

    m_planningList = planningList;
    emit planningListChanged(m_planningList);
}

/**
 * @brief Playlist::setPlanningsInPlaylist Setze Anzahl der Planungen in der aktuellen Playliste
 * @param planningsInPlaylist Anzahl der Planungen in der aktuellen Playliste
 */
void Playlist::setPlanningsInPlaylist(int planningsInPlaylist)
{
    if (m_planningsInPlaylist == planningsInPlaylist)
        return;

    m_planningsInPlaylist = planningsInPlaylist;
    emit planningsInPlaylistChanged(m_planningsInPlaylist);
}

/**
 * @brief Playlist::setPlanedon Setze MACs auf welcher diese Playliste läuft
 * @param planedon MACs die diese Playliste besitzen
 */
void Playlist::setPlanedon(QString planedon)
{
    if (m_planedon == planedon)
        return;

    m_planedon = planedon;
    emit planedonChanged(m_planedon);
}
