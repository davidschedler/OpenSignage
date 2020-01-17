#ifndef CUSTOMEVENT_H
#define CUSTOMEVENT_H

#include <QEvent>
#include <QHash>

#define SENDER      "SENDER"
#define COMMAND     "COMMAND"
#define TYPE        "TYPE"
#define IDENT       "IDENT"
#define PWD         "PWD"
#define DATA        "DATA"

class Q_DECL_EXPORT CustomEvent : public QEvent
{

public:
    CustomEvent();
    CustomEvent(CustomEvent *mySelf);

    virtual ~CustomEvent();

    /* creation and deleation */
    void addCustomEvent(QString key, QString value);
    void delCustomEvent(QString key);

    /* task handling */
    QString getCustomEvent(QString key, QString value = "");
    QStringList getCustomEventList();
    bool containsCustomEvent(QString key);
    bool isCustomEventEmpty();

protected:
    /* structure of a task => value => key */
    QHash<QString, QString> p_customEvent;
};

#endif // CUSTOMEVENT_H
