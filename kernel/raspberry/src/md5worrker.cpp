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

