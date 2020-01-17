#ifndef CONTENT_H
#define CONTENT_H

#include <QObject>

class Content : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString md5 READ md5 WRITE setMd5 NOTIFY md5Changed)
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged)
    Q_PROPERTY(QString fileType READ fileType WRITE setFileType NOTIFY fileTypeChanged)
    Q_PROPERTY(QString reso READ reso WRITE setReso NOTIFY resoChanged)
    Q_PROPERTY(QString size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(QString time READ time WRITE setTime NOTIFY timeChanged)

private:
    int m_id;
    QString m_name;
    QString m_md5;
    QString m_filePath;
    QString m_fileType;
    QString m_reso;
    QString m_size;
QString m_time;

public:
    Content(QObject *parent = nullptr);
    Content(int id, QString name, QString md5, QString filePath, QString fileType,
            QString reso, QString size, QString time, QObject *parent = nullptr);

    int id() const { return m_id; }
    QString name() const { return m_name; }
    QString md5() const { return m_md5; }
    QString filePath() const { return m_filePath; }
    QString fileType() const { return m_fileType; }
    QString reso() const { return m_reso; }
    QString size() const { return m_size; }
    QString time() const { return m_time; }

        /* helper functions */
    int getTimeAsDurationInMS();

signals:
    void idChanged(int id);
    void nameChanged(QString name);
    void md5Changed(QString md5);
    void filePathChanged(QString filePath);
    void fileTypeChanged(QString fileType);
    void resoChanged(QString reso);
    void sizeChanged(QString size);
    void timeChanged(QString time);

public slots:
    void setId(int id);
    void setName(QString name);
    void setMd5(QString md5);
    void setFilePath(QString filePath);
    void setFileType(QString fileType);
    void setReso(QString reso);
    void setSize(QString size);
    void setTime(QString time);
};

#endif // CONTENT_H
