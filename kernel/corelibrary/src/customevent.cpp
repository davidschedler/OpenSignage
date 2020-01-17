#include "customevent.h"

/**
 * @brief CustomEvent::CustomEvent Default-Konstruktor
 */
CustomEvent::CustomEvent() :
    QEvent (QEvent::Type(QEvent::User+1))
{

}

/**
 * @brief CustomEvent::CustomEvent Konstruktor auf sich selbst
 * @param mySelf
 */
CustomEvent::CustomEvent(CustomEvent *mySelf) :
    QEvent (QEvent::Type(QEvent::User+1))
{
    if (mySelf)
        p_customEvent = mySelf->p_customEvent;
}

/**
 * @brief CustomEvent::~CustomEvent Default-Destruktor
 */
CustomEvent::~CustomEvent()
{

}

/**
 * @brief CustomEvent::addCustomEvent Füge benutzerdefiniertes Event hinzu
 * @param key Event-Key (Anweisung)
 * @param value Event-Value (Daten)
 */
void CustomEvent::addCustomEvent(QString key, QString value)
{
    p_customEvent.insert(key, value);
}

/**
 * @brief CustomEvent::delCustomEvent Lösche benutzerdefiniertes Event
 * @param key Event-Key (Anweisung)
 */
void CustomEvent::delCustomEvent(QString key)
{
    p_customEvent.remove(key);
}

/**
 * @brief CustomEvent::getCustomEvent Hole Daten von benutzerdefiniertes Event
 * @param key Event-Key (Anweisung)
 * @param value Event-Value (Daten)
 * @return Daten des benutzerdefinierten Events anhand des Event-Keys
 */
QString CustomEvent::getCustomEvent(QString key, QString value)
{
    if (p_customEvent.contains(key))
        return p_customEvent.value(key);
    else
        return value;
}

/**
 * @brief CustomEvent::getCustomEventList Hole Liste aller benutzerdefiniertens Event-Keys
 * @return alle Event-Keys
 */
QStringList CustomEvent::getCustomEventList()
{
    QStringList list;
    QString key;
    foreach (key, p_customEvent.keys())
    {
        list.append(key);
    }
    return list;
}

/**
 * @brief CustomEvent::containsCustomEvent Prüfe ob benutzerdefiniertes Event den Event-Key beinhaltet
 * @param key Event-Key
 * @return onFail = false, onSuccess = true
 */
bool CustomEvent::containsCustomEvent(QString key)
{
    if (p_customEvent.contains(key))
        return true;
    else
        return false;
}

/**
 * @brief CustomEvent::isCustomEventEmpty Prüfe ob benutzerdefiniertes Event leer ist
 * @return empty = false, !empty = true
 */
bool CustomEvent::isCustomEventEmpty()
{
    return p_customEvent.isEmpty();
}
