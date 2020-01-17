#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QTimer>

#include <algorithm>
#include <QtAlgorithms>

#include "websocketclient.h"
#include "groups.h"
#include "users.h"
#include "hardware.h"
#include "content.h"
#include "planning.h"
#include "playlist.h"

class Engine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ident READ ident WRITE setIdent NOTIFY identChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(bool isAuthenticated READ isAuthenticated WRITE setIsAuthenticated NOTIFY isAuthenticatedChanged)
    Q_PROPERTY(int navigationMenuIdx READ navigationMenuIdx WRITE setNavigationMenuIdx NOTIFY navigationMenuIdxChanged)
    Q_PROPERTY(int hardwareIdx READ hardwareIdx WRITE setHardwareIdx NOTIFY hardwareIdxChanged)
    Q_PROPERTY(int groupsIdx READ groupsIdx WRITE setGroupsIdx NOTIFY groupsIdxChanged)
    Q_PROPERTY(int usersIdx READ usersIdx WRITE setUsersIdx NOTIFY usersIdxChanged)
    Q_PROPERTY(int contentIdx READ contentIdx WRITE setContentIdx NOTIFY contentIdxChanged)
    Q_PROPERTY(QList<QObject*> groupsList READ groupsList WRITE setGroupsList NOTIFY groupsListChanged)
    Q_PROPERTY(QList<QObject*> hardwareList READ hardwareList WRITE setHardwareList NOTIFY hardwareListChanged)
    Q_PROPERTY(QList<QObject*> contentList READ contentList WRITE setContentList NOTIFY contentListChanged)
    Q_PROPERTY(QList<QObject*> playlistList READ playlistList WRITE setPlaylistList NOTIFY playlistListChanged)
    Q_PROPERTY(int playlistIdx READ playlistIdx WRITE setPlaylistIdx NOTIFY playlistIdxChanged)
    Q_PROPERTY(int planningIdx READ planningIdx WRITE setPlanningIdx NOTIFY planningIdxChanged)
    Q_PROPERTY(int groupNameComboIdx READ groupNameComboIdx WRITE setGroupNameComboIdx NOTIFY groupNameComboIdxChanged)
    Q_PROPERTY(int groupTypeComboIdx READ groupTypeComboIdx WRITE setGroupTypeComboIdx NOTIFY groupTypeComboIdxChanged)
    Q_PROPERTY(QString currentUserGroup READ currentUserGroup WRITE setCurrentUserGroup NOTIFY currentUserGroupChanged)
    Q_PROPERTY(QStringList defaultGroupTypes READ defaultGroupTypes WRITE setDefaultGroupTypes NOTIFY defaultGroupTypesChanged)
    Q_PROPERTY(QString nextStackItem READ nextStackItem WRITE setNextStackItem NOTIFY nextStackItemChanged)
    Q_PROPERTY(QStringList contentTypes READ contentTypes WRITE setContentTypes NOTIFY contentTypesChanged)
    Q_PROPERTY(int showContentToAddPlanning READ showContentToAddPlanning WRITE setShowContentToAddPlanning NOTIFY showContentToAddPlanningChanged)
    Q_PROPERTY(QString searchContent READ searchContent WRITE setSearchContent NOTIFY searchContentChanged)
    Q_PROPERTY(int freeClientsComboIdx READ freeClientsComboIdx WRITE setFreeClientsComboIdx NOTIFY freeClientsComboIdxChanged)
    Q_PROPERTY(int planningTypeComboIdx READ planningTypeComboIdx WRITE setPlanningTypeComboIdx NOTIFY planningTypeComboIdxChanged)
    Q_PROPERTY(QString loginMessage READ loginMessage WRITE setLoginMessage NOTIFY loginMessageChanged)
    Q_PROPERTY(QStringList freeClientsToPlanOn READ freeClientsToPlanOn WRITE setFreeClientsToPlanOn NOTIFY freeClientsToPlanOnChanged)
    Q_PROPERTY(QStringList usedClientsToPlanOn READ usedClientsToPlanOn WRITE setUsedClientsToPlanOn NOTIFY usedClientsToPlanOnChanged)
    Q_PROPERTY(bool isWebsocketConnected READ isWebsocketConnected WRITE setIsWebsocketConnected NOTIFY isWebsocketConnectedChanged)

public:
    explicit Engine(QObject *parent = nullptr);

    Q_INVOKABLE void getDatabaseData(QString command, QString type, QString data = "");
    Q_INVOKABLE void createDatabaseEntry(QString command, QString type, QString data = "");
    Q_INVOKABLE void updateDatabaseEntry(QString command, QString type, QString data = "");

    Q_INVOKABLE QStringList getGroupNames();
    Q_INVOKABLE QStringList getGroupTypes();
    Q_INVOKABLE QString getGroupIdByGroupName(QString groupName);
    Q_INVOKABLE QString getGroupTypeIdByGroupType(QString groupType);
    Q_INVOKABLE int getGroupNameIdxByGroupName(QString groupName);
    Q_INVOKABLE int getGroupTypeIdxByGroupType(QString groupType);
    Q_INVOKABLE int getPlaylistIdWithIdx(int idx);
    Q_INVOKABLE int getPlaylistIdWithPlaylistName(QString name);
    Q_INVOKABLE void deleteHardware();
    Q_INVOKABLE void deleteUser(int type = 0);
    Q_INVOKABLE bool deleteGroup(int type = 0);
    Q_INVOKABLE void deletePlanning(int type = 0);
    Q_INVOKABLE int calculateDashboardInfo(QString target);
    Q_INVOKABLE bool deletePlaylist(int type =0);
    Q_INVOKABLE void sortAlgorithm(int sortTarget,int sortType, int sortOrder);
    Q_INVOKABLE void searchAlgorithm(int sortTarget, int sortType, QString searchtString);
    Q_INVOKABLE bool findContentByName(QString name);
    Q_INVOKABLE QStringList getPlaylistNames();
    Q_INVOKABLE int getContentIdByContentName(QString name);
    Q_INVOKABLE int calculateListviewModelDataCount(QString targetName, int targetID);
    Q_INVOKABLE QString groupCreatedby();
    Q_INVOKABLE bool isOwnGrp();
    Q_INVOKABLE bool isHardwareInUse();
    Q_INVOKABLE void createClientModels(QString type);
    Q_INVOKABLE void moveClientModel(QString type, int idx);
    Q_INVOKABLE void writeClientModel();
    Q_INVOKABLE QString generateWeekDays(bool mo, bool di, bool mi, bool don, bool fr, bool sa, bool so);
    Q_INVOKABLE int clientsToPlanOnCount(QString target);
    Q_INVOKABLE int nextPlanningOrder();
    Q_INVOKABLE void changePlanningPosition(QString destination);
    Q_INVOKABLE int getDurationFromContent(int id_content, int id_planning, int id_playlist);

    enum SortTarget { CONTENT=0, GROUPS, USERS, PLAYLISTS, PLANNINGS, HARDWARE };
    enum SortContent { CID=0, CNAME, CTYPE };
    enum SortGroups { GID=0, GNAME, GTYPE, GCOUNT };
    enum SortUsers { UID=0, CFNAME, CLNAME, CMAIL, CGNAME };
    enum SortPlaylist { PLID=0, PLNAME, PLGNAME, PLORIE, PLCOUNT };
    enum SortPlanning { PID=0, PNAME, PVP, PWEEK, PVT, PTYPE };
    enum SortHardware { MAC=0, LOCATION, VERSION, HGNAME };
    enum SortOrder { UP=0, DOWN };

    QString ident() const { return m_ident; }
    QString password() const { return m_password; }
    bool isAuthenticated() const { return m_isAuthenticated; }
    int navigationMenuIdx() const { return m_navigationMenuIdx; }
    int hardwareIdx() const { return m_hardwareIdx;  }
    int groupsIdx() const{return m_groupsIdx; }
    int usersIdx() const { return m_usersIdx; }
    QList<QObject*> groupsList() const { return m_groupsList; }
    QList<QObject*> hardwareList() const { return m_hardwareList; }
    QList<QObject*> contentList() const  { return m_contentList; }
    QList<QObject*> playlistList() const { return m_playlistList; }
    int playlistIdx() const { return m_playlistIdx; }
    int planningIdx() const { return m_planningIdx; }
    int groupNameComboIdx() const { return m_groupNameComboIdx; }
    int groupTypeComboIdx() const { return m_groupTypeComboIdx; }
    QString currentUserGroup() const { return m_currentUserGroup; }
    QStringList defaultGroupTypes() const { return m_defaultGroupTypes; }
    int contentIdx() const { return m_contentIdx; }
    QString nextStackItem() const { return m_nextStackItem; }
    QList<QObject *> getContentListBACKUP() const;
    void setContentListBACKUP(const QList<QObject *> &contentListBACKUP);
    QStringList contentTypes() const { return m_contentTypes; }
    int showContentToAddPlanning() const { return m_showContentToAddPlanning; }
    QString searchContent() const { return m_searchContent; }
    int freeClientsComboIdx() const { return m_freeClientsComboIdx; }
    int planningTypeComboIdx() const { return m_planningTypeComboIdx; }
    QString loginMessage() const{return m_loginMessage; }
    QStringList usedClientsToPlanOn() const { return m_usedClientsToPlanOn; }
    QStringList freeClientsToPlanOn() const { return m_freeClientsToPlanOn; }
    bool isWebsocketConnected() const {  return m_isWebsocketConnected; }

signals:
    void identChanged(QString ident);
    void passwordChanged(QString password);
    void isAuthenticatedChanged(bool isAuthenticated);
    void navigationMenuIdxChanged(int navigationMenuIdx);
    void hardwareIdxChanged(int hardwareIdx);
    void groupsIdxChanged(int groupsIdx);
    void usersIdxChanged(int usersIdx);
    void groupsListChanged(QList<QObject*> groupsList);
    void hardwareListChanged(QList<QObject*> hardwareList);
    void contentListChanged(QList<QObject*> contentList);
    void playlistListChanged(QList<QObject*> playlistList);
    void playlistIdxChanged(int playlistIdx);
    void planningIdxChanged(int planningIdx);
    void groupNameComboIdxChanged(int groupNameComboIdx);
    void groupTypeComboIdxChanged(int groupTypeComboIdx);
    void currentUserGroupChanged(QString currentUserGroup);
    void defaultGroupTypesChanged(QStringList defaultGroupTypes);
    void contentIdxChanged(int contentIdx);
    void nextStackItemChanged(QString nextStackItem);
    void contentTypesChanged(QStringList contentTypes);
    void showContentToAddPlanningChanged(int showContentToAddPlanning);
    void searchContentChanged(QString searchContent);
    void freeClientsComboIdxChanged(int freeClientsComboIdx);
    void planningTypeComboIdxChanged(int planningTypeComboIdx);
    void loginMessageChanged(QString loginMessage);
    void freeClientsToPlanOnChanged(QStringList freeClientsToPlanOn);
    void usedClientsToPlanOnChanged(QStringList usedClientsToPlanOn);
    void refreshModelDataCount();
    void isWebsocketConnectedChanged(bool isWebsocketConnected);

public slots:
    void setIdent(QString ident);
    void setPassword(QString password);
    void setIsAuthenticated(bool isAuthenticated);
    void setNavigationMenuIdx(int navigationMenuIdx);
    void setGroupsList(QList<QObject*> groupsList);
    void setHardwareList(QList<QObject*> hardwareList);
    void incommingData(QByteArray message);
    void setHardwareIdx(int hardwareIdx);
    void setGroupsIdx(int groupsIdx);
    void setUsersIdx(int usersIdx);
    void setContentList(QList<QObject*> contentList);
    void setPlaylistList(QList<QObject*> playlistList);
    void setPlaylistIdx(int playlistIdx);
    void setPlanningIdx(int planningIdx);
    void setGroupNameComboIdx(int groupNameComboIdx);
    void setGroupTypeComboIdx(int groupTypeComboIdx);
    void setCurrentUserGroup(QString currentUserGroup);
    void setDefaultGroupTypes(QStringList defaultGroupTypes);
    void setContentIdx(int contentIdx);
    void setNextStackItem(QString nextStackItem);
    void setContentTypes(QStringList contentTypes);
    void setShowContentToAddPlanning(int showContentToAddPlanning);
    void setSearchContent(QString searchContent);
    void setFreeClientsComboIdx(int freeClientsComboIdx);
    void setPlanningTypeComboIdx(int planningTypeComboIdx);
    void setLoginMessage(QString loginMessage);
    void setFreeClientsToPlanOn(QStringList freeClientsToPlanOn);
    void setUsedClientsToPlanOn(QStringList usedClientsToPlanOn);
    void websocketDisconnected();
    void websocketConnected();
    void setIsWebsocketConnected(bool isWebsocketConnected);

private:
    WebSocketClient m_webSocketClient;
    QString m_ident;
    QString m_password;
    bool m_isAuthenticated;
    int m_navigationMenuIdx;
    QList<QObject*> m_groupsList;
    QList<QObject*> m_hardwareList;
    int m_hardwareIdx;
    int m_groupsIdx;
    int m_usersIdx;
    int m_contentIdx;
    QList<QObject*> m_contentList;
    QList<QObject*> m_playlistList;
    int m_playlistIdx;
    int m_planningIdx;
    int m_groupNameComboIdx;
    int m_groupTypeComboIdx;
    QString m_currentUserGroup;
    QStringList m_defaultGroupTypes;
    QString m_nextStackItem;
    QList<QObject*> m_contentListBACKUP;
    QStringList m_contentTypes;
    int m_showContentToAddPlanning;
    QString m_searchContent;
    int m_freeClientsComboIdx;
    int m_planningTypeComboIdx;
    QString m_loginMessage;
    QStringList m_freeClientsToPlanOn;
    QStringList m_usedClientsToPlanOn;
    QStringList m_movedClientsForDbSync;
    QTimer *m_writeTimer;
    bool m_isWebsocketConnected;

    QByteArray generateByteMessage(QString command, QString type, QString data = "");
    Groups* getGroupById(int id);
    Groups* getGroupByName(QString name);
    Hardware* getHardwareByMac(QString mac);
    Content* getContentById(int id);
    Playlist* getPlaylistById(int id);
    void sortContent(int sortType, int sortOrder);
    void sortGroups(int sortType, int sortOrder);
    void sortUsers(int sortType, int sortOrder);
    void sortPlaylists(int sortType, int sortOrder);
    void sortPlanning(int sortType, int sortOrder);
    void sortHardware(int sortType, int sortOrder);
    bool checkDeleteGroup();
};

#endif // ENGINE_H
