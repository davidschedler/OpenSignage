#ifndef PLANNING_H
#define PLANNING_H

#include <QObject>

class Planning : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(int id_content READ id_content WRITE setId_content NOTIFY id_contentChanged)
    Q_PROPERTY(QString contentName READ contentName WRITE setContentName NOTIFY contentNameChanged)
    Q_PROPERTY(QString validPeriod READ validPeriod WRITE setValidPeriod NOTIFY validPeriodChanged)
    Q_PROPERTY(QString weekdays READ weekdays WRITE setWeekdays NOTIFY weekdaysChanged)
    Q_PROPERTY(QString validTime READ validTime WRITE setValidTime NOTIFY validTimeChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(int id_order READ id_order WRITE setId_order NOTIFY id_orderChanged)
    Q_PROPERTY(int duration READ duration WRITE setDuration NOTIFY durationChanged)

private:
    int m_id;
    int m_id_content;
    QString m_contentName;
    QString m_validPeriod;
    QString m_weekdays;
    QString m_validTime;
    QString m_type;
    int m_id_order;
    int m_duration;

public:
    Planning(QObject *parent = nullptr);
    Planning(int id, int id_content, QString contentName, QString validPeriod, QString weekdays,
              QString validTime, QString type, int id_order, int duration, QObject *parent = nullptr);

    int id() const { return m_id; }
    int id_content() const { return m_id_content; }
    QString contentName() const { return m_contentName;  }
    QString validPeriod() const {  return m_validPeriod; }
    QString weekdays() const { return m_weekdays; }
    QString validTime() const  { return m_validTime; }
    QString type() const { return m_type; }
    int id_order() const { return m_id_order; }
    int duration() const { return m_duration; }

signals:
    void idChanged(int id);
    void id_contentChanged(int id_content);
    void contentNameChanged(QString contentName);
    void validPeriodChanged(QString validPeriod);
    void weekdaysChanged(QString weekdays);
    void validTimeChanged(QString validTime);
    void typeChanged(QString type);
    void id_orderChanged(int id_order);
    void durationChanged(int duration);

public slots:
    void setId(int id);
    void setId_content(int id_content);
    void setContentName(QString contentName);
    void setValidPeriod(QString validPeriod);
    void setWeekdays(QString weekdays);
    void setValidTime(QString validTime);
    void setType(QString type);
    void setId_order(int id_order);
    void setDuration(int duration);
};

#endif // PLANNING_H
