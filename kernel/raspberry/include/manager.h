#ifndef MANAGER_H
#define MANAGER_H

#include <QWebSocket>
#include <QProcess>
#include <QWebSocketServer>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QThread>
#include <QTimer>
#include <QRegularExpression>
#include <QNetworkInterface>

#include <algorithm>

#include "plugininterface.h"
#include "corelibrary.h"
#include "managerconfig.h"
#include "md5worker.h"
#include "playlistitem.h"

class Q_DECL_EXPORT Manager : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Manager_iid")
    Q_INTERFACES(PluginInterface)

    Q_PROPERTY(QList<QObject *> playlist READ playlist WRITE setPlaylist NOTIFY playlistChanged)
    Q_PROPERTY(QString currentPicture READ currentPicture WRITE setCurrentPicture NOTIFY currentPictureChanged)
    Q_PROPERTY(QString currentVideo READ currentVideo WRITE setCurrentVideo NOTIFY currentVideoChanged)


public:
    Manager();

    enum PlayerState { WAIT=0, READY };

    Q_INVOKABLE void requestPlaylistFromMac();
    Q_INVOKABLE void checkThis();
    //QJsonDocument generateJsonDocument(QString command, QString array, QList<QStringList> data);
    QByteArray generateByteMessage(QString command, QString type, QString data);
    void newCommunicationDataToParse(QByteArray newData);
    void newDownloadUrl(CustomEvent event);

    /* interface implementation */
    QString get(int type) const;
    int onInit(int type);
    bool onEvent(CustomEvent *customEvent);
    void loadSettings();
    void saveSettings();

    QList<QObject *> playlist() const { return m_playlist; }
    QString currentPicture() const { return m_currentPicture;  }

    QString currentVideo() const
    {
        return m_currentVideo;
    }

public slots:
    void setPlaylist(QList<QObject *> playlist);
    void incomingCheckedMD5(QString id_content, QString filePath, QString md5Sum);
    void setCurrentPicture(QString currentPicture);
        void requestPlayliste();
            void playerStateMachine();

        void setCurrentVideo(QString currentVideo)
        {
            if (m_currentVideo == currentVideo)
                return;

            m_currentVideo = currentVideo;
            emit currentVideoChanged(m_currentVideo);
        }

Q_SIGNALS:
    void playlistChanged(QList<QObject *> playlist);
    void checkMD5Sum(QString id_content, QString filePath);
    void currentPictureChanged(QString currentPicture);

    void currentVideoChanged(QString currentVideo);

private Q_SLOTS:

private:
    /* settings */
    ManagerConfig *m_config;

    /* ContentWorker */
    MD5Worker *m_md5Worker;
    QThread *m_contentThread;

    /* player */
    int m_playerState;
    int m_playerPosition;

    QTimer m_playerStateTimer;

    /* helpfer */
    QList<QObject *> m_playlist;
    QList<QObject*> m_newParsedPlaylist;
    QList<QObject*> m_playlistToDownload;
    QList<QObject*> m_playlistToCheckMD5;
    PlaylistItem* getPlaylistItemByID(int id_planning);
    bool itemAvailableToCheckMD5();
    QJsonDocument generateJsonDocument(QString command, QString array, QList<QStringList> data);
    QTimer m_refreshPlayliste;
    QString m_currentPicture;
    QString m_currentVideo;
};

#endif // MANAGER_H
