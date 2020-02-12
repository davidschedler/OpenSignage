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

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include "planning.h"

class Playlist : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)
    Q_PROPERTY(QString groupName READ groupName WRITE setGroupName NOTIFY groupNameChanged)
    Q_PROPERTY(QList<QObject*> planningList READ planningList WRITE setPlanningList NOTIFY planningListChanged)
    Q_PROPERTY(int planningsInPlaylist READ planningsInPlaylist WRITE setPlanningsInPlaylist NOTIFY planningsInPlaylistChanged)
    Q_PROPERTY(QString planedon READ planedon WRITE setPlanedon NOTIFY planedonChanged)

private:
    int m_id;
    QString m_name;
    QString m_orientation;
    QString m_groupName;
    QList<QObject*> m_planningList;
    int m_planningsInPlaylist;
    QString m_planedon;

public:
    Playlist(QObject *parent = nullptr);
    Playlist(int id, QString name, QString orientation, QString groupName, QString planedon, QObject *parent = nullptr);

    int id() const { return m_id; }
    QString name() const { return m_name;}
    QString orientation() const { return m_orientation; }
    QString groupName() const { return m_groupName; }
    QList<QObject*> planningList() const { return m_planningList;}
    int planningsInPlaylist() const { return m_planningsInPlaylist; }
    QString planedon() const { return m_planedon; }

    /* helper functions */
    void deletePlanning(int idx);
    void clearPlanningList();
    Planning *getPlanningById(int id);
    void addPlanning(Planning *newPlanning);
    void swapPlanning(int oldIdx, int newIdx);
    QPair<QString, QString> swapPlanningInformation(int target1, int target2);

signals:
    void idChanged(int id);
    void nameChanged(QString name);
    void orientationChanged(QString orientation);
    void groupNameChanged(QString groupName);
    void planningListChanged(QList<QObject*> planningList);
    void planningsInPlaylistChanged(int planningsInPlaylist);
    void planedonChanged(QString planedon);

public slots:
    void setId(int id);
    void setName(QString name);
    void setOrientation(QString orientation);
    void setGroupName(QString groupName);
    void setPlanningList(QList<QObject*> planningList);
    void setPlanningsInPlaylist(int planningsInPlaylist);
    void setPlanedon(QString planedon);
};

#endif // PLAYLIST_H
