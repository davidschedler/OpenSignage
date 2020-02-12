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

#include "include/md5worker.h"

/**
 * @brief MD5Worker::MD5Worker Default-Konstruktor
 * @param contentPath Arbeitsverzeichnis
 * @param parent QObject
 */
MD5Worker::MD5Worker(QString contentPath, QObject *parent) :
    QObject(parent),
    m_contentPath(contentPath)

{

}

/**
 * @brief MD5Worker::process Starte Thread
 */
void MD5Worker::process()
{

}

/**
 * @brief MD5Worker::generateMD5Sum Generiere einen MD5-Hash eines gewünschten Contents
 * @param id_content ID des Contents
 * @param filePath Pfad des Contents
 */
void MD5Worker::generateMD5Sum(QString id_content, QString filePath)
{
    QByteArray finalHash;
    QFile f(filePath);
    if (f.open(QFile::ReadOnly)) {
        QCryptographicHash hash(QCryptographicHash::Md5);
        if (hash.addData(&f)) {
            finalHash = hash.result().toHex();
        }
    }

   emit readyGeneratedMD5(id_content, filePath, finalHash);
}

