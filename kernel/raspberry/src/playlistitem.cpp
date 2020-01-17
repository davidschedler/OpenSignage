#include "include/playlistitem.h"

/**
 * @brief PlaylistItem::PlaylistItem Default-Konstruktor eines gültigen Playlist-Items
 * @param id_content Content-ID
 * @param validPeriod Content-Gültiger-Zeitraum
 * @param weekdays Content-Gültige-Wochentage
 * @param validTime Content-Gültiger-Zeitpunkt
 * @param type Content-Typ
 * @param contentName Content-Name
 * @param contentMD5 Content-MD5
 * @param id_order Content-Platzierung
 * @param id_planning Planungs-ID
 * @param duration Content-Dauer
 * @param parent QObject
 */
PlaylistItem::PlaylistItem(int id_content, QString validPeriod, QString weekdays, QString validTime, QString type, QString contentName, QString contentMD5, int id_order, int id_planning, int duration, QObject *parent) :
    QObject(parent),
    m_id_content(id_content),
    m_validPeriod(validPeriod),
    m_weekdays(weekdays),
    m_validTime(validTime),
    m_type(type),
    m_contentName(contentName),
    m_contentMD5(contentMD5),
    m_id_order(id_order),
    m_id_planning(id_planning),
    m_isDownloaded(false),
    m_isMD5Correct(false),
    m_duration(duration)
{

}

/**
 * @brief PlaylistItem::isPlaylistItemValid Prüfe ob Item aktuell gültig ist
 * @return true = gültig, false = ungültig
 */
bool PlaylistItem::isPlaylistItemValid()
{
    bool ret = false;
    bool date = false;
    bool time = false;

    QString startPeriod = m_validPeriod.left(m_validPeriod.lastIndexOf("-"));
    QString endPeriod = m_validPeriod.right(m_validPeriod.lastIndexOf("-"));
    QDate start_period = QDate::fromString(startPeriod,"dd.MM.yyyy");
    QDate end_period = QDate::fromString(endPeriod,"dd.MM.yyyy");

    QString startTime = m_validTime.left(m_validTime.lastIndexOf("-"));
    QString endTime = m_validTime.right(m_validTime.lastIndexOf("-"));
    QTime start_Time = QTime::fromString(startTime,"hh:mm:ss");
    QTime end_Time = QTime::fromString(endTime,"hh:mm:ss");

    QDate currentDate = QDate::currentDate();
    if (start_period <= currentDate && end_period >= currentDate)
        date = true;

    QTime currentTime = QTime::currentTime();
    if (start_Time <= currentTime && end_Time >= currentTime)
        time = true;

    if (isDownloaded() && isMD5Correct() && date && time)
        ret = true;

    return ret;
}

/**
 * @brief PlaylistItem::isVideo Prüfe ob Datei ein Video oder ein Bild ist
 * @return true = Video, false = Bild
 */
bool PlaylistItem::isVideo()
{
    bool ret = false;
    QString type = m_contentName.mid(m_contentName.lastIndexOf(".")+1);
    if (type == "mkv" || type == "mp4" || type == "mov" || type == "avi") {
        ret = true;
    }
    return ret;
}


/**
 * @brief PlaylistItem::setId_content Setze Content-ID
 * @param id_content Content-ID
 */
void PlaylistItem::setId_content(int id_content)
{
    if (m_id_content == id_content)
        return;

    m_id_content = id_content;
    emit id_contentChanged(m_id_content);
}

/**
 * @brief PlaylistItem::setValidPeriod Setze Content-Gültigkeit-Zeitraums
 * @param validPeriod Gültiger-Zeitraum
 */
void PlaylistItem::setValidPeriod(QString validPeriod)
{
    if (m_validPeriod == validPeriod)
        return;

    m_validPeriod = validPeriod;
    emit validPeriodChanged(m_validPeriod);
}

/**
 * @brief PlaylistItem::setWeekdays Setze Content-Gültigkeits-Wochentage
 * @param weekdays Gültige-Wochentage
 */
void PlaylistItem::setWeekdays(QString weekdays)
{
    if (m_weekdays == weekdays)
        return;

    m_weekdays = weekdays;
    emit weekdaysChanged(m_weekdays);
}

/**
 * @brief PlaylistItem::setValidTime Setze Content-Gültigkeits-Zeitpunkt
 * @param validTime Gültiger-Zeitpunkt
 */
void PlaylistItem::setValidTime(QString validTime)
{
    if (m_validTime == validTime)
        return;

    m_validTime = validTime;
    emit validTimeChanged(m_validTime);
}

/**
 * @brief PlaylistItem::setType Setze Content-Typ
 * @param type Content-Typ
 */
void PlaylistItem::setType(QString type)
{
    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged(m_type);
}

/**
 * @brief PlaylistItem::setContentName Setze Content-Name
 * @param contentName Content-Name
 */
void PlaylistItem::setContentName(QString contentName)
{
    if (m_contentName == contentName)
        return;

    m_contentName = contentName;
    emit contentNameChanged(m_contentName);
}

/**
 * @brief PlaylistItem::setContentMD5 Setze Content-MD5
 * @param contentMD5 Content-MD5
 */
void PlaylistItem::setContentMD5(QString contentMD5)
{
    if (m_contentMD5 == contentMD5)
        return;

    m_contentMD5 = contentMD5;
    emit contentMD5Changed(m_contentMD5);
}

/**
 * @brief PlaylistItem::setId_order Setze Content-Order
 * @param id_order Content-Order
 */
void PlaylistItem::setId_order(int id_order)
{
    if (m_id_order == id_order)
        return;

    m_id_order = id_order;
    emit id_orderChanged(m_id_order);
}

/**
 * @brief PlaylistItem::setIsDownloaded Setze Content-isDownloaded (Wenn Content bereits herunter geladen wurde9
 * @param isDownloaded Content-isDownloaded
 */
void PlaylistItem::setIsDownloaded(bool isDownloaded)
{
    if (m_isDownloaded == isDownloaded)
        return;

    m_isDownloaded = isDownloaded;
    emit isDownloadedChanged(m_isDownloaded);
}

/**
 * @brief PlaylistItem::setIsMD5Correct Setze Content-isMD5Correct (wenn Content-MD5 bereits geprüft wurde)
 * @param isMD5Correct Content-isMD5Correct
 */
void PlaylistItem::setIsMD5Correct(bool isMD5Correct)
{
    if (m_isMD5Correct == isMD5Correct)
        return;

    m_isMD5Correct = isMD5Correct;
    emit isMD5CorrectChanged(m_isMD5Correct);
}

/**
 * @brief PlaylistItem::setId_planning  Setze Planing-ID
 * @param id_planning Planing-ID
 */
void PlaylistItem::setId_planning(int id_planning)
{
    if (m_id_planning == id_planning)
        return;

    m_id_planning = id_planning;
    emit id_planningChanged(m_id_planning);
}

/**
 * @brief PlaylistItem::setDuration Setze Dauer des Contents
 * @param duration Dauer des Contents
 */
void PlaylistItem::setDuration(int duration)
{
    if (m_duration == duration)
        return;

    m_duration = duration;
    emit durationChanged(m_duration);
}

