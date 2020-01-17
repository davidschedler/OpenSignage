#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QDebug>
#include <QPluginLoader>
#include <QtPlugin>
#include <QDir>

#include "plugininterface.h"

QT_FORWARD_DECLARE_CLASS(CoreLibrary);
QT_FORWARD_DECLARE_CLASS(PluginInterface);

class PluginManager : public QObject
{
    Q_OBJECT

public:
    explicit PluginManager(CoreLibrary *corelibrary, QObject *parent = nullptr);

    /* gerneral */
    void setupDirectories();

    /* load plugins */
    void loadAllPlugins(QString pluginLocation);
    void loadPlugin(QString fileName);

    /* plugin handling */
    PluginInterface* getPlugin(QString pid);
    bool containsPlugin(QString pid);
    QList<PluginInterface *> getAllPlugins();

private:
    /* plugin handling*/
    CoreLibrary            *m_coreLibrary;
    QList<PluginInterface*> m_pluginList;
};

#endif // PLUGINMANAGER_H
