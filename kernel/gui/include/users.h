#ifndef USERS_H
#define USERS_H

#include <QObject>

class Users : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY lastNameChanged)
    Q_PROPERTY(QString groupName READ groupName WRITE setGroupName NOTIFY groupNameChanged)
    Q_PROPERTY(QString groupType READ groupType WRITE setGroupType NOTIFY groupTypeChanged)
    Q_PROPERTY(QString eMail READ eMail WRITE setEMail NOTIFY eMailChanged)

private:
    int m_id;
    QString m_firstName;
    QString m_lastName;
    QString m_groupName;
    QString m_groupType;
    QString m_eMail;

public:
    Users(QObject *parent = nullptr);
    Users(int id, QString firstName, QString lastName, QString groupName,
          QString groupType, QString eMail, QObject *parent = nullptr);
    ~Users();

    int id() const { return m_id; }
    QString firstName() const { return m_firstName; }
    QString lastName() const { return m_lastName; }
    QString groupName() const { return m_groupName; }
    QString groupType() const { return m_groupType; }
    QString eMail() const { return m_eMail; }

signals:
    void idChanged(int id);
    void firstNameChanged(QString firstName);
    void lastNameChanged(QString lastName);
    void groupNameChanged(QString groupName);
    void groupTypeChanged(QString groupType);
    void eMailChanged(QString eMail);

public slots:
    void setId(int id);
    void setFirstName(QString firstName);
    void setLastName(QString lastName);
    void setGroupName(QString groupName);
    void setGroupType(QString groupType);
    void setEMail(QString eMail);
};

#endif // USERS_H
