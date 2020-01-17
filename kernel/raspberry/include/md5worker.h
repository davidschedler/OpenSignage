#ifndef MD5WORKER_H
#define MD5WORKER_H

#include <QObject>
#include <QFile>
#include <QDebug>
#include <QCryptographicHash>

class MD5Worker : public QObject
{
    Q_OBJECT
public:
    explicit MD5Worker(QString contentPath, QObject *parent = nullptr);

    enum WorkerState { IDLING=0, WORKING };

public slots:
    void process();
    void generateMD5Sum(QString id_content, QString filePath);

signals:
    void finished();
    void readyGeneratedMD5(QString id_content, QString filePath, QString md5Sum);
    void error(QString err);

private:
    QString m_contentPath;
    QStringList m_jobList;
};

#endif // MD5WORKER_H


