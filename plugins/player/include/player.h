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

#ifndef PLAYER_H
#define PLAYER_H

#include <QUrl>
#include <QThread>

#include "plugininterface.h"
#include "corelibrary.h"
#include "playerconfig.h"
#include "player_global.h"
#include "vlc/vlc.h"

#define qtu( i ) ((i).toUtf8().constData())

class PLAYERSHARED_EXPORT Player : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Player_iid")
    Q_INTERFACES(PluginInterface)

public:
    Player();
    ~Player();

    /* interface implementation */
    QString get(int type) const;
    int onInit(int type);
    bool onEvent(CustomEvent *customEvent);
    void loadSettings();
    void saveSettings();

    uint32_t getWinID() const;
    void setWinID(const uint32_t &winID);

Q_SIGNALS:

public Q_SLOTS:


private Q_SLOTS:


private:
    /* player handling */
    libvlc_instance_t *vlcInstance;
    libvlc_media_player_t *vlcPlayer;

    uint32_t m_winID;

    /* settings */
    PlayerConfig *m_config;
};

#endif // PLAYER_H
