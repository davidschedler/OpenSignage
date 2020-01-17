#include "pluginmanager.h"

/**
 * @brief PluginManager::PluginManager Default-Konstruktor
 * @param corelibrary Pointer auf die Corelibrary
 * @param parent QObject
 */
PluginManager::PluginManager(CoreLibrary *corelibrary, QObject *parent) :
    QObject(parent),
    m_coreLibrary(corelibrary)
{

}

/**
 * @brief PluginManager::setupDirectories Setze Ordner
 */
void PluginManager::setupDirectories()
{

}

/**
 * @brief PluginManager::loadAllPlugins Lade alle Plugins von einem Verzeichnis
 * @param pluginLocation Plugin-Verzeichnis
 */
void PluginManager::loadAllPlugins(QString pluginLocation)
{
    QDir path(pluginLocation);
    qDebug() << "Loading plugins from: " << path.path();

    QString fileName;
    foreach (fileName, path.entryList(QStringList("*.so"),QDir::Files))
    {
        loadPlugin(path.absolutePath() + "/" + fileName);
    }
}

/**
 * @brief PluginManager::loadPlugin Lade ein Plugin mit spezifischen Namen
 * @param fileName Name des Plugins
 */
void PluginManager::loadPlugin(QString fileName)
{
    qDebug() << "Loading plugin named: " << fileName;

    QPluginLoader loader(fileName);
    QObject *possiblePlugin = loader.instance();

    if (possiblePlugin)
    {
        PluginInterface *plugin = qobject_cast<PluginInterface*>(possiblePlugin);
        if (plugin)
        {
            qInfo() << "Loaded plugin: " << plugin->get(NAME);

            plugin->corelibrary = this->m_coreLibrary;
            plugin->onInit(FIRSTINIT);
            plugin->onInit(EXTENDEDINIT);
            m_pluginList.append(plugin);
        }
    }
    else
    {
        qInfo() << "Can't load plugin: " << fileName << " = " << loader.errorString();
    }
}

/**
 * @brief PluginManager::getPlugin Hole Plugin
 * @param pid Plugin PID (Identifikations ID)
 * @return onFail = nullptr, onSuccess = Plugin
 */
PluginInterface *PluginManager::getPlugin(QString pid)
{
    PluginInterface *plugin = nullptr;
    if (!m_pluginList.isEmpty())
    {
        foreach (plugin, m_pluginList)
        {
            if (plugin->get(PID) == pid)
            {
                return plugin;
            }
        }
    }
    return plugin;
}

/**
 * @brief PluginManager::containsPlugin Prüfe ob Plugin vorhanden ist
 * @param pid Plugin PID (Identifikations ID)
 * @return onFail = nullptr, onSuccess = Plugin
 */
bool PluginManager::containsPlugin(QString pid)
{
    PluginInterface *plugin = nullptr;
    if (!m_pluginList.isEmpty())
    {
        foreach (plugin, m_pluginList)
        {
            if (plugin->get(PID) == pid)
            {
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief PluginManager::getAllPlugins Hole alle geladenen Plugins
 * @return Lister aller geladenen Plugins
 */
QList<PluginInterface *> PluginManager::getAllPlugins()
{
    return m_pluginList;
}
