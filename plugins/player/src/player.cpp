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

#include "player.h"

Player::Player()
{

}

Player::~Player()
{
    settings->sync();
    m_config->deleteLater();
}

QString Player::get(int type) const
{
    switch (type) {
    case PID:
        return "0004";
    case NAME:
        return "Player";
    case PTYPE:
        return "PLUGIN";
    case PSETTINGS:
        return corelibrary->getAppDir().absolutePath() + "/settings/settings.ini";
    case VERSION:
        return "VERSION_1.0.0";
    default:
        break;
    }
    return "";
}

int Player::onInit(int type)
{
    switch (type) {
    case FIRSTINIT:
    {
        /* plugin general */
        m_config = new PlayerConfig(this);
        corelibrary->subscribeEvent("PLAYER", this);

        loadSettings();

        qInfo() << get(NAME) << "first init done!";
    }
        break;
    case EXTENDEDINIT:
    {
        vlcPlayer = nullptr;
        vlcInstance = libvlc_new(0, nullptr);
        if (vlcInstance == nullptr) {
            qDebug() << "CANT CREATE VLC INSTANCE";
        }
        qInfo() << get(NAME) << "extended init done!";

    }
        break;
    case EXIT:
        break;
    default:
        break;
    }
    return 0;
}

bool Player::onEvent(CustomEvent *customEvent)
{
    QString print;
    if (!customEvent->getCustomEvent(COMMAND).isEmpty())
    {
        QString command = customEvent->getCustomEvent(COMMAND);
        QString type = customEvent->getCustomEvent(TYPE);
        QString ident = customEvent->getCustomEvent(IDENT);
        if (command == "PLAYER") {

            qInfo() << get(NAME) + " onEvent: " + "Command: " + command  + " Type: " + type + " File: " + ident;

            QByteArray byteMessage;
            if (type == "PLAY") {

                //STOP IF PLAYING
                if (vlcPlayer && libvlc_media_player_is_playing(vlcPlayer)) {
                    libvlc_media_player_stop(vlcPlayer);
                    libvlc_media_player_release(vlcPlayer);
                    vlcPlayer = nullptr;
                }
                // CREATE A NEW MEDIA
                libvlc_media_t *vlcMedia = libvlc_media_new_path(vlcInstance, qtu(corelibrary->getAppDir().absolutePath() + "/content_downloaded/" + ident));
                if (!vlcMedia) {
                    qDebug() << "CANT CREATE NEW MEDIA";
                    return 0;
                }
                //CReate a new liblc player
                vlcPlayer = libvlc_media_player_new_from_media(vlcMedia);
                libvlc_media_release(vlcMedia);

                //in window einhängen
                libvlc_media_player_set_xwindow(vlcPlayer, m_winID);

                //start playback
                libvlc_media_player_play(vlcPlayer);
                qInfo() << get(NAME) + " playing Video: " + ident;
            }
            if (type == "STOP") {
                //STOP IF PLAYING
                if (vlcPlayer) {
                    libvlc_media_player_stop(vlcPlayer);
                    libvlc_media_player_release(vlcPlayer);
                    vlcPlayer = nullptr;
                }
            }
            if (type == "WINID") {
                setWinID(ident.toUInt());
                qInfo() << get(NAME) + "Setting WinID for VLC-PLayer: " + ident;
            }
        }
    }
    return false;
}

void Player::loadSettings()
{
    settings = new QSettings(get(PSETTINGS),QSettings::IniFormat);

    settings->beginGroup(get(NAME));
    settings->endGroup();

    qInfo() << get(NAME) + " Loaded settings: ";
    settings->sync();
}

void Player::saveSettings()
{
    settings->beginGroup(get(NAME));
    settings->sync();
    settings->endGroup();

    qInfo() << get(NAME) + " SAVED settings: ";
}

uint32_t Player::getWinID() const
{
    return m_winID;
}

void Player::setWinID(const uint32_t &winID)
{
    m_winID = winID;
}


