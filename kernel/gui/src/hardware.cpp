#include "include/hardware.h"

/**
 * @brief Hardware::Hardware Defaul-Konstruktor
 * @param parent QObject
 */
Hardware::Hardware(QObject *parent) :
    QObject(parent)
{

}

/**
 * @brief Hardware::Hardware Default-Konstruktor für eine gültige Hardware
 * @param mac Hardware-Mac
 * @param groupName Hardware-Gruppen-Name
 * @param groupType Hardware-Gruppen-Typ
 * @param location Hardware-Standplatz
 * @param version Hardware-Version
 * @param planedonPlaylist Hardware-Eingehängt auf Playliste
 * @param parent QObject
 */
Hardware::Hardware(QString mac, QString groupName, QString groupType, QString location, QString version, int planedonPlaylist, QObject *parent):
    QObject(parent),
    m_mac(mac),
    m_groupName(groupName),
    m_groupType(groupType),
    m_location(location),
    m_version(version),
    m_planedonPlaylist(planedonPlaylist)
{

}

/**
 * @brief Hardware::~Hardware Default-Destruktor
 */
Hardware::~Hardware()
{
    this->deleteLater();
}

/**
 * @brief Hardware::setMac Setze Hardware-MAC
 * @param mac Hardware-MAC
 */
void Hardware::setMac(QString mac)
{
    if (m_mac == mac)
        return;

    m_mac = mac;
    emit macChanged(m_mac);
}

/**
 * @brief Hardware::setGroupName Setze Hardware-Gruppen-Name
 * @param groupName Hardware-Gruppen-Name
 */
void Hardware::setGroupName(QString groupName)
{
    if (m_groupName == groupName)
        return;

    m_groupName = groupName;
    emit groupNameChanged(m_groupName);
}

/**
 * @brief Hardware::setGroupType Setze Hardware-Gruppen-Typ
 * @param groupType Hardware-Gruppen-Typ
 */
void Hardware::setGroupType(QString groupType)
{
    if (m_groupType == groupType)
        return;

    m_groupType = groupType;
    emit groupTypeChanged(m_groupType);
}

/**
 * @brief Hardware::setLocation Setze Hardware-Standplatz
 * @param location Hardware-Standplatz
 */
void Hardware::setLocation(QString location)
{
    if (m_location == location)
        return;

    m_location = location;
    emit locationChanged(m_location);
}

/**
 * @brief Hardware::setVersion Setze Hardware-Version
 * @param version Hardware-Version
 */
void Hardware::setVersion(QString version)
{
    if (m_version == version)
        return;

    m_version = version;
    emit versionChanged(m_version);
}

/**
 * @brief Hardware::setPlanedonPlaylist Setze Playlisten_ID welche die Hardware nutzt
 * @param planedonPlaylist Playlisten-ID
 */
void Hardware::setPlanedonPlaylist(int planedonPlaylist)
{
    if (m_planedonPlaylist == planedonPlaylist)
        return;

    m_planedonPlaylist = planedonPlaylist;
    emit planedonPlaylistChanged(m_planedonPlaylist);
}


