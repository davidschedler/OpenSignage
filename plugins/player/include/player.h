#ifndef PLAYER_H
#define PLAYER_H

#include <QUrl>
#include <QThread>

#include "plugininterface.h"
#include "corelibrary.h"
#include "playerconfig.h"
#include "player_global.h"
#include "vlc/vlc.h"

#define qtu( i ) ((i).toUtf8().constData())

class PLAYERSHARED_EXPORT Player : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Player_iid")
    Q_INTERFACES(PluginInterface)

public:
    Player();
    ~Player();

    /* interface implementation */
    QString get(int type) const;
    int onInit(int type);
    bool onEvent(CustomEvent *customEvent);
    void loadSettings();
    void saveSettings();

    uint32_t getWinID() const;
    void setWinID(const uint32_t &winID);

Q_SIGNALS:

public Q_SLOTS:


private Q_SLOTS:


private:
    /* player handling */
    libvlc_instance_t *vlcInstance;
    libvlc_media_player_t *vlcPlayer;

    uint32_t m_winID;

    /* settings */
    PlayerConfig *m_config;
};

#endif // PLAYER_H
