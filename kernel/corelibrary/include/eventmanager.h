#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <QObject>
#include <QHash>
#include <QCoreApplication>

#include "customevent.h"
#include "plugininterface.h"
#include "pluginmanager.h"

QT_FORWARD_DECLARE_CLASS(CustomEvent);
QT_FORWARD_DECLARE_CLASS(PluginManagerInterface);
QT_FORWARD_DECLARE_CLASS(PluginManager);

class EventManager : public QObject
{
    Q_OBJECT

public:
    explicit EventManager(PluginManager *pluginManager, QObject *parent = nullptr);

    /* event handling */
    void addEvent(CustomEvent *customEvent);
    void subscribeEvent(QString message, PluginInterface *receiver);
    void subscribeEvents(QStringList messages, PluginInterface *receiver);

private:
    /* plugin handling */
    PluginManager  *m_pluginManager;

    // Plugins can subscribe to certain MediaEvents.
    // This hash table holds for every message a list of plugin UUIDs, which subscribed to the message.
    QHash<QString, QList<PluginInterface*> *> m_subscriptions;
};

#endif // EVENTMANAGER_H
