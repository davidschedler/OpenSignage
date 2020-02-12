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

#ifndef HARDWARE_H
#define HARDWARE_H

#include <QObject>

class Hardware : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString mac READ mac WRITE setMac NOTIFY macChanged)
    Q_PROPERTY(QString groupName READ groupName WRITE setGroupName NOTIFY groupNameChanged)
    Q_PROPERTY(QString groupType READ groupType WRITE setGroupType NOTIFY groupTypeChanged)
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY locationChanged)
    Q_PROPERTY(QString version READ version WRITE setVersion NOTIFY versionChanged)
    Q_PROPERTY(int planedonPlaylist READ planedonPlaylist WRITE setPlanedonPlaylist NOTIFY planedonPlaylistChanged)

private:
    QString m_mac;
    QString m_groupName;
    QString m_groupType;
    QString m_location;
    QString m_version;
    int m_planedonPlaylist;

public:
    Hardware(QObject *parent = nullptr);
    Hardware(QString mac, QString groupName, QString groupType, QString location, QString version, int planedonPlaylist, QObject *parent = nullptr);
    ~Hardware();

    QString mac() const { return m_mac; }
    QString groupName() const { return m_groupName; }
    QString groupType() const { return m_groupType; }
    QString location() const { return m_location; }
    QString version() const { return m_version; }
 int planedonPlaylist() const { return m_planedonPlaylist; }

signals:
    void macChanged(QString mac);
    void groupNameChanged(QString groupName);
    void groupTypeChanged(QString groupType);
    void locationChanged(QString location);
    void versionChanged(QString version);
    void planedonPlaylistChanged(int planedonPlaylist);

public slots:
    void setMac(QString mac);
    void setGroupName(QString groupName);
    void setGroupType(QString groupType);
    void setLocation(QString location);
    void setVersion(QString version);
    void setPlanedonPlaylist(int planedonPlaylist);
};

#endif // HARDWARE_H
