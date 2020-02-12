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

#include "include/content.h"

/**
 * @brief Content::Content Default-Konstruktor
 * @param parent
 */
Content::Content(QObject *parent) :
    QObject(parent)
{

}

/**
 * @brief Content::Content Default-Konstruktor für ein gültigen Content
 * @param id Content-ID
 * @param name Content-Name
 * @param md5 Content-MD5
 * @param filePath Content-Dateipfad
 * @param fileType Content-Typ (Bild/Video)
 * @param reso Content-Auflösung
 * @param size Content-Dateigröße
 * @param time Content-Zeit
 * @param parent QObject
 */
Content::Content(int id, QString name, QString md5, QString filePath, QString fileType, QString reso, QString size, QString time, QObject *parent) :
    QObject(parent),
    m_id(id),
    m_name(name),
    m_md5(md5),
    m_filePath(filePath),
    m_fileType(fileType),
    m_reso(reso),
    m_size(size),
    m_time(time)
{

}

/**
 * @brief Content::getTimeAsDurationInMS Wandle die textuelle Content-Zeit (z.B 1 min 23 s 542 ms) in absoluten Milisekunden-Wert um
 * @return Die Zeit des Contents in MS
 */
int Content::getTimeAsDurationInMS()
{
    QString duration = time();
    int durationInt = -1;
    QStringList durationList = duration.split(" ");
    for (int i = 0 ; i < durationList.count()-1 ; i++) {
        QString time = durationList.at(i);
        QString type = durationList.at(i+1);

        if (type == "h") {
            durationInt = durationInt + (3600000 * time.toInt());
        }
        if (type == "min") {
            durationInt = durationInt + (60000 * time.toInt());
        }
        if (type == "s") {
            durationInt = durationInt + (1000 * time.toInt());
        }
        if (type == "ms") {
            durationInt = durationInt + (time.toInt());
        }
        i++;
    }
    return durationInt;
}

/**
 * @brief Content::setId Setze die Content-ID
 * @param id Content-ID
 */
void Content::setId(int id)
{
    if (m_id == id)
        return;

    m_id = id;
    emit idChanged(m_id);
}

/**
 * @brief Content::setName Setze den Content-Name
 * @param name Content-Name
 */
void Content::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

/**
 * @brief Content::setMd5 Setze den Content-MD5
 * @param md5 Content-MD5
 */
void Content::setMd5(QString md5)
{
    if (m_md5 == md5)
        return;

    m_md5 = md5;
    emit md5Changed(m_md5);
}

/**
 * @brief Content::setFilePath Setze den Content-Dateipfad
 * @param filePath Content-Dateipfad
 */
void Content::setFilePath(QString filePath)
{
    if (m_filePath == filePath)
        return;

    m_filePath = filePath;
    emit filePathChanged(m_filePath);
}

/**
 * @brief Content::setFileType Setze den Content-Typ
 * @param fileType Content-Typ
 */
void Content::setFileType(QString fileType)
{
    if (m_fileType == fileType)
        return;

    m_fileType = fileType;
    emit fileTypeChanged(m_fileType);
}

/**
 * @brief Content::setReso Setze die Content-Auflösung
 * @param reso Content-Auflösung
 */
void Content::setReso(QString reso)
{
    if (m_reso == reso)
        return;

    m_reso = reso;
    emit resoChanged(m_reso);
}

/**
 * @brief Content::setSize Setze die Content-Dateigröße
 * @param size Content-Dateigröße
 */
void Content::setSize(QString size)
{
    if (m_size == size)
        return;

    m_size = size;
    emit sizeChanged(m_size);
}

/**
 * @brief Content::setTime Setze die Content-Zeit
 * @param time Content-Zeit
 */
void Content::setTime(QString time)
{
    if (m_time == time)
        return;

    m_time = time;
    emit timeChanged(m_time);
}
