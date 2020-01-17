#include "include/filewatcher.h"

/**
 * @brief FileWatcher::FileWatcher Default-Konstruktor
 * @param pathToWatch Pfad der zu beobachten ist
 * @param m_contentFromDb Bereits in der Datenbank vorhandener Content
 * @param parent QObject
 */
FileWatcher::FileWatcher(QString pathToWatch, QStringList m_contentFromDb, QObject *parent) :
    QObject(parent),
    m_watcherPath(pathToWatch),
    m_contentFromDb(m_contentFromDb),
    m_watcher(new QFileSystemWatcher(this))
{

}

/**
 * @brief FileWatcher::scanDir Überprüft rekursiv den zu überprüfenden Ordner
 * @param dir Der zu überprüfende Ordner
 * @param extendedBy Unterordner
 */
void FileWatcher::scanDir(QDir dir, QString extendedBy)
{
    dir.setNameFilters(QStringList("*.*"));
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    qInfo() << + "FileWatcher Scanning: " << dir.path();

    QStringList fileList = dir.entryList();
    for (int i=0; i<fileList.count(); i++)
    {
        if(fileList[i] != "main.nut" &&
                fileList[i] != "info.nut")
        {
            m_contentFromHdd.append(dir.absolutePath() + "/" + fileList[i]);
        }
    }

    dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList dirList = dir.entryList();
    for (int i=0; i<dirList.size(); ++i)
    {
        QString newPath = QString("%1/%2").arg(dir.absolutePath()).arg(dirList.at(i));
        m_watcher->addPath(newPath);
        scanDir(QDir(newPath),dirList.at(i));
    }
}

/**
 * @brief FileWatcher::process Starte Thread indem er die Differenz zwischen Content in der Datenbank und Content auf der HDD überprüft.
 * Für den Content der auf der HDD ist, jedoch nicht in der Datenbank muss der MD5 generiert werden und Content-Informationen bezogen werden.
 * Danach kann der Content der Datenbank hinzugefügt werden.
 */
void FileWatcher::process()
{
    qInfo() << "FileWatcher STARTET";
    connect(m_watcher, &QFileSystemWatcher::directoryChanged, this, &FileWatcher::showModifiedDirectory);

    m_watcher->addPath(m_watcherPath);
    qInfo() << "Watching PATH: " + m_watcherPath;

    // fill hddcontent
    QDir dir(m_watcherPath);
    scanDir(dir,"");

    qDebug() << "HDD FILES" << m_contentFromHdd;
    qDebug() << "DB FILES" << m_contentFromDb;
    QStringList connentMissingOnHdd;
    QStringList contentMissingOnDb;

    for (int i = 0 ; i < m_contentFromHdd.count() ; i ++) {
        if (!m_contentFromDb.contains(m_contentFromHdd.at(i))) {
            contentMissingOnDb.append(m_contentFromHdd.at(i));
        }
    }
    for (int j = 0 ; j < m_contentFromDb.count() ; j ++) {
        if (!m_contentFromHdd.contains(m_contentFromDb.at(j))) {
            connentMissingOnHdd.append(m_contentFromDb.at(j));
        }
    }

    foreach (QString filePath, contentMissingOnDb) {
        generateMd5(filePath);
    }

}

/**
 * @brief FileWatcher::generateMd5 Generiert den MD5-Hash eines Contents und bezieht via MediaInfo die zusätzlichen Content-Informationen
 * @param filePath Pfad des Contents
 */
void FileWatcher::generateMd5(QString filePath)
{
    qDebug() << "Start genarating md5 for db add" +filePath;
    QByteArray finalHash;
    QFile f(filePath);
    if (f.open(QFile::ReadOnly)) {
        QCryptographicHash hash(QCryptographicHash::Md5);
        if (hash.addData(&f)) {
            finalHash = hash.result().toHex();
        }
    }

    QProcess mediaInfo;
    mediaInfo.start("mediainfo",QStringList() << filePath);
    mediaInfo.waitForFinished(500);

    QByteArray result = mediaInfo.readAll();

    QStringList resultList = QString(result).split("\n");
    QString width, height, duration = "-";

    foreach (QString singleResult, resultList) {
        if (singleResult.contains("Width")) {
            QString temp = singleResult.mid(singleResult.lastIndexOf(": ")+2);
            temp = temp.left(temp.lastIndexOf("pixels")-1);
            temp.replace(" ","");
            width = temp;
        }
        if (singleResult.contains("Height")) {
            QString temp = singleResult.mid(singleResult.lastIndexOf(": ")+2);
            temp = temp.left(temp.lastIndexOf("pixels")-1);
            temp.replace(" ","");
            height = temp;
        }
        if (singleResult.contains("Duration")) {
            QString temp = singleResult.mid(singleResult.lastIndexOf(": ")+2);
            //temp = temp.left(temp.lastIndexOf("pixels")-1);
            //temp.replace(" ","");
            duration = temp;
        }
    }

    QStringList data;
    data.append(QString("Name=") + filePath.mid(filePath.lastIndexOf("/")+1));
    data.append(QString("MD5=") + finalHash);
    data.append(QString("Reso=") + width + "x" + height);
    data.append(QString("Size=") + QString::number(f.size()));
    data.append(QString("Time=") + duration);
    data.append(QString("Path=") + filePath);

    emit detectedNewContent(data);
}

bool FileWatcher::isFileSizeIncreasing(QString filePath)
{
    bool ret = false;

    QFileInfo initInfo(filePath);
    qint64 initSize = initInfo.size();

    for (int i = 0 ; i < 3 ; i ++) {
        QFileInfo info(filePath);
        qint64 newSize = info.size();
        if (newSize != initSize) {
            initSize = newSize;
            ret = true;
        }
        else {
            ret = false;
        }
    }
    return ret;
}

/**
 * @brief FileWatcher::showModifiedDirectory Falls eine änderung in einem Ordner vorliegt muss das neue File erkannt und der MD5-Prozess gestartet werden
 * @param modifiedDirectory Pfad des modifizierten Ordners
 */
void FileWatcher::showModifiedDirectory(QString modifiedDirectory)
{
    QDir dir(modifiedDirectory);
    dir.setNameFilters(QStringList("*.*"));
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    QStringList filesToCheck;
    QStringList fileList = dir.entryList();

    foreach (QString file, m_contentFromHdd) {
        QString modifiedFileDirectory = file.left(file.lastIndexOf("/")+1);
        if (modifiedDirectory == modifiedFileDirectory) {
            filesToCheck.append(file.mid(file.lastIndexOf("/")+1));
        }
    }

    QSet<QString> subtraction = fileList.toSet().subtract(filesToCheck.toSet());

    for (int i = 0 ; i < subtraction.toList().count() ; i ++) {
        QString searchString = subtraction.toList().at(i);
        bool found = false;

        for (int j = 0 ; j < m_contentFromHdd.count() ; j ++) {
            QString matchString = m_contentFromHdd.at(j);
            matchString = matchString.mid(matchString.lastIndexOf("/")+1);
            if (searchString == matchString) {
                found = true;
            }
        }
        if (found) {
            found = false;
        }
        else {
            bool alreadyDone = false;
            for (int i = 0 ; i < m_contentFromHdd.count() ; i ++) {
                QString temp = m_contentFromHdd.at(i).mid(m_contentFromHdd.at(i).lastIndexOf("/")+1);
                if (temp == searchString) {
                    alreadyDone = true;
                }
            }
            if (!alreadyDone) {
                m_contentModified.append(searchString);
                m_contentFromHdd.append(searchString);
            }
        }
    }

    QStringList copyContentModified = m_contentModified;

    while (!m_contentModified.isEmpty()) {
        if (m_contentModified.isEmpty()) {
            return;
        }
        for (int k = 0 ; k < copyContentModified.count() ; k++) {
            bool shouldCheckMD5 = isFileSizeIncreasing(modifiedDirectory + "/" + copyContentModified.at(k));

            if (!shouldCheckMD5) {
                generateMd5(modifiedDirectory + "/" + copyContentModified.at(k));
                m_contentModified.removeAt(k);
            }
        }
        copyContentModified = m_contentModified;
    }



    //    foreach (QString sub, subtraction.toList()) {
    //        m_contentFromHdd.append(modifiedDirectory + "/" + sub);
    //        generateMd5(modifiedDirectory + "/" + sub);
    //    }


}

