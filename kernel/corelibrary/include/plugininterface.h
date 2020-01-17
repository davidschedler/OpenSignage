#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#define FIRSTINIT        0
#define EXTENDEDINIT     1
#define EXIT             2

#define PID         0
#define NAME        1
#define PTYPE       2
#define PSETTINGS   3
#define VERSION     4

#include <QObject>
#include <QtPlugin>
#include <QSettings>
#include <QDir>

#include "customevent.h"

QT_FORWARD_DECLARE_CLASS(CoreLibrary);
QT_FORWARD_DECLARE_CLASS(CustomEvent);

class PluginInterface : public QObject
{
    Q_OBJECT

    friend class PluginManager;

public:
    explicit PluginInterface();
    virtual ~PluginInterface();

    /* to access core */
    CoreLibrary *corelibrary;

    /* general */
    QSettings *settings;

    /* interface functions */
    virtual QString get(int type) const = 0;
    virtual int onInit(int type) = 0;
    virtual bool onEvent(CustomEvent *customEvent) = 0;
    virtual void loadSettings() = 0;
    virtual void saveSettings() = 0;

    /* redirect qevent to onEvent */
    bool event(QEvent *customEvent);

private:

};

Q_DECLARE_INTERFACE(PluginInterface, "PluginInterface_iid")

#endif // PLUGININTERFACE_H
