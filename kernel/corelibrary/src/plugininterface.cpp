#include "plugininterface.h"

/**
 * @brief PluginInterface::PluginInterface Default-Konstruktor
 */
PluginInterface::PluginInterface() :
    QObject(),
    corelibrary(nullptr)
{

}

/**
 * @brief PluginInterface::~PluginInterface Default-Destruktor
 */
PluginInterface::~PluginInterface()
{

}

/**
 * @brief PluginInterface::event Prüfe ob Event ein CustomEvent ist
 * @param customEvent Benutzerdefiniertes Event
 * @return onFail = in Eventloop lasse, onSuccess = in onEvent leiten
 */
bool PluginInterface::event(QEvent *customEvent)
{
    if (customEvent->type() == QEvent::User+1)
        return onEvent((CustomEvent*)customEvent);
    return QObject::event(customEvent);
}
