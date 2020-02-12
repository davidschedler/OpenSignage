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

#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <QObject>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QCryptographicHash>
#include <QFileSystemWatcher>
#include <QProcess>

class FileWatcher : public QObject
{
    Q_OBJECT
public:
    explicit FileWatcher(QString pathToWatch, QStringList m_contentFromDb, QObject *parent = nullptr);

    /* helper functions */
    void scanDir(QDir dir, QString extendedBy);
    void generateMd5(QString filePath);
    bool isFileSizeIncreasing(QString filePath);

public slots:
    void process();
    void showModifiedDirectory(QString modifiedDirectory);

signals:
    void finished();
    void contentChecked();
    void detectedNewContent(QStringList data);
    void error(QString err);

private:
    /* MD5 handling */
    QString m_watcherPath;
    QStringList m_contentFromDb;
    QStringList m_contentFromHdd;
    QStringList m_contentModified;
    QFileSystemWatcher *m_watcher;
};

#endif // FILEWATCHER_H
