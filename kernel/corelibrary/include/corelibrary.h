#ifndef CORELIBRARY_H
#define CORELIBRARY_H

#define ERROR   0
#define WARNING 1
#define SUCCESS 2

#define CORE    "CORE"

#include <QObject>

#include "customevent.h"
#include "eventmanager.h"
#include "plugininterface.h"
#include "pluginmanager.h"
#include "corelibrary_global.h"

QT_FORWARD_DECLARE_CLASS(CustomEvent);

class CORELIBRARYSHARED_EXPORT CoreLibrary : public QObject
{
    Q_OBJECT

    friend class CustomEvent;

public:
    explicit CoreLibrary(QString appPath, QObject *parent = nullptr);

    /* gerneral */
    QDir getAppDir();

    /* init plugins */
    void initPlugins(QString pluginLocation);

    /* event handling */
    void subscribeEvent(QString message, PluginInterface *plugin);
    void subscribeEvents(QStringList messages, PluginInterface *plugin);
    void addEvent(CustomEvent *customEvent);

private:
    /* general */
    QString m_appPath;

    /* corelibrary handling */
    PluginManager   *m_pluginManager;
    EventManager    *m_eventManager;
};

#endif // CORELIBRARY_H
