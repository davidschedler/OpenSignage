#include "corelibrary.h"

/**
 * @brief CoreLibrary::CoreLibrary Default-Konstruktor
 * @param appPath Programmpfad
 * @param parent QObject
 */
CoreLibrary::CoreLibrary(QString appPath, QObject *parent) :
    QObject (parent),
    m_appPath(appPath),
    m_pluginManager(new PluginManager(this)),
    m_eventManager(new EventManager(m_pluginManager))
{

}

/**
 * @brief CoreLibrary::getAppDir Hole Programmpfad
 * @return Programmpfad
 */
QDir CoreLibrary::getAppDir()
{
    return m_appPath;
}

/**
 * @brief CoreLibrary::initPlugins PluginManager lade Plugin
 * @param pluginLocation Pluginpfad
 */
void CoreLibrary::initPlugins(QString pluginLocation)
{
    m_pluginManager->loadAllPlugins(pluginLocation);
}

/**
 * @brief CoreLibrary::subscribeEvent Plugin abonniert Thema (Command)
 * @param message Thema (Command)
 * @param plugin Plugin, welches ein Thema abonnieren möchte
 */
void CoreLibrary::subscribeEvent(QString message, PluginInterface *plugin)
{
    m_eventManager->subscribeEvent(message, plugin);
}

/**
 * @brief CoreLibrary::subscribeEvents Plugin abonniert Themen (Commands)
 * @param messages Themen (Commands)
 * @param plugin Plugins, welches Themen abonnieren möchte
 */
void CoreLibrary::subscribeEvents(QStringList messages, PluginInterface *plugin)
{
    QString message;
    foreach (message, messages)
    {
        m_eventManager->subscribeEvent(message, plugin);
    }
}

/**
 * @brief CoreLibrary::addEvent EventManager füge Benutzerdefiniertes Event der Eventloop hinzu.
 * @param customEvent Benutzerdefiniertes Event
 */
void CoreLibrary::addEvent(CustomEvent *customEvent)
{
    m_eventManager->addEvent(customEvent);
}
