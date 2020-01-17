#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H

#include <QObject>
#include <QDate>
#include <QTime>

class PlaylistItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id_content READ id_content WRITE setId_content NOTIFY id_contentChanged)
    Q_PROPERTY(QString validPeriod READ validPeriod WRITE setValidPeriod NOTIFY validPeriodChanged)
    Q_PROPERTY(QString weekdays READ weekdays WRITE setWeekdays NOTIFY weekdaysChanged)
    Q_PROPERTY(QString validTime READ validTime WRITE setValidTime NOTIFY validTimeChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString contentName READ contentName WRITE setContentName NOTIFY contentNameChanged)
    Q_PROPERTY(QString contentMD5 READ contentMD5 WRITE setContentMD5 NOTIFY contentMD5Changed)
    Q_PROPERTY(int id_order READ id_order WRITE setId_order NOTIFY id_orderChanged)
    Q_PROPERTY(int id_planning READ id_planning WRITE setId_planning NOTIFY id_planningChanged)
    Q_PROPERTY(bool isDownloaded READ isDownloaded WRITE setIsDownloaded NOTIFY isDownloadedChanged)
    Q_PROPERTY(bool isMD5Correct READ isMD5Correct WRITE setIsMD5Correct NOTIFY isMD5CorrectChanged)
    Q_PROPERTY(int duration READ duration WRITE setDuration NOTIFY durationChanged)

private:
    int m_id_content;
    QString m_validPeriod;
    QString m_weekdays;
    QString m_validTime;
    QString m_type;
    QString m_contentName;
    QString m_contentMD5;
    int m_id_order;
    int m_id_planning;
    bool m_isDownloaded;
    bool m_isMD5Correct;
    int m_duration;

public:
    explicit PlaylistItem(int id_content, QString validPeriod, QString weekdays, QString validTime, QString type,
                          QString contentName, QString contentMD5, int id_order, int id_planning,int duration, QObject *parent = nullptr);

    int id_content() const{ return m_id_content; }
    QString validPeriod() const { return m_validPeriod;}
    QString weekdays() const  {  return m_weekdays; }
    QString validTime() const { return m_validTime; }
    QString type() const{ return m_type; }
    QString contentName() const { return m_contentName; }
    QString contentMD5() const { return m_contentMD5; }
    int id_order() const{ return m_id_order; }
    bool isDownloaded() const  {  return m_isDownloaded; }
    bool isMD5Correct() const { return m_isMD5Correct;  }
    int id_planning() const { return m_id_planning; }
    int duration() const {  return m_duration;}

    bool isPlaylistItemValid();
    bool isVideo();

    signals:

        void id_contentChanged(int id_content);
    void validPeriodChanged(QString validPeriod);
    void weekdaysChanged(QString weekdays);
    void validTimeChanged(QString validTime);
    void typeChanged(QString type);
    void contentNameChanged(QString contentName);
    void contentMD5Changed(QString contentMD5);
    void id_orderChanged(int id_order);
    void isDownloadedChanged(bool isDownloaded);
    void isMD5CorrectChanged(bool isMD5Correct);
    void id_planningChanged(int id_planning);
    void durationChanged(int duration);

public slots:
    void setId_content(int id_content);
    void setValidPeriod(QString validPeriod);
    void setWeekdays(QString weekdays);
    void setValidTime(QString validTime);
    void setType(QString type);
    void setContentName(QString contentName);
    void setContentMD5(QString contentMD5);
    void setId_order(int id_order);
    void setIsDownloaded(bool isDownloaded);
    void setIsMD5Correct(bool isMD5Correct);
    void setId_planning(int id_planning);
    void setDuration(int duration);
};

#endif // PLAYLISTITEM_H
