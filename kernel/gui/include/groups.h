#ifndef GROUPS_H
#define GROUPS_H

#include <QObject>
#include "users.h"

class Groups : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString groupName READ groupName WRITE setGroupName NOTIFY groupNameChanged)
    Q_PROPERTY(QString groupType READ groupType WRITE setGroupType NOTIFY groupTypeChanged)
    Q_PROPERTY(int userInGroup READ userInGroup WRITE setUserInGroup NOTIFY userInGroupChanged)
    Q_PROPERTY(QList<QObject*> usersList READ usersList WRITE setUsersList NOTIFY usersListChanged)
    Q_PROPERTY(int id_grouptype READ id_grouptype WRITE setId_grouptype NOTIFY id_grouptypeChanged)

private:
    int m_id;
    QString m_groupName;
    QString m_groupType;
    int m_userInGroup;
    QList<QObject*> m_usersList;
    int m_id_grouptype;

public:
    Groups(QObject *parent = nullptr);
    Groups(int id, QString groupName, QString groupType, int id_groupType, QObject *parent = nullptr);
    ~Groups();

    int id() const { return m_id; }
    QString groupName() const { return m_groupName; }
    QString groupType() const { return m_groupType; }
    int userInGroup() const { return m_usersList.count(); }
    QList<QObject*> usersList() const { return m_usersList; }
    int id_grouptype() const { return m_id_grouptype; }

    /* helper functions */
    Users* getUserById(int id);
    void addUser(Users *addUser);
    void deleteUser(int idx);
    void clearUserList();

signals:
    void idChanged(int id);
    void groupNameChanged(QString groupName);
    void groupTypeChanged(QString groupType);
    void userInGroupChanged(int userInGroup);
    void usersListChanged(QList<QObject*> usersList);
    void id_grouptypeChanged(int id_grouptype);

public slots:
    void setId(int id);
    void setGroupName(QString groupName);
    void setGroupType(QString groupType);
    void setUserInGroup(int userInGroup);
    void setUsersList(QList<QObject*> usersList);
    void setId_grouptype(int id_grouptype);
};

#endif // GROUPS_H
