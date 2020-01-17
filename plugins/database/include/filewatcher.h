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
