#ifndef WATCHWINDOWLISTENER_H
#define WATCHWINDOWLISTENER_H

#include <QObject>
#include <QCheckBox>
#include <QMap>
#include <QVariantMap>
#include <QVariant>
#include <QThread>
//#include "oifeaturestate.h"

class WatchWindowListener : public QObject
{
    Q_OBJECT
public:
    explicit WatchWindowListener(QObject *parent = 0);

    bool isSettingsReady;
    bool isCheckBoxReady;
    bool isGUIReady;
    int digitCount;

signals:

    void sendAttributeNames(QStringList);
    void sendData(QVariantMap m);
    
public slots:
    void setSettingsReady(bool b);
    void setCheckBoxReady(bool b);
    void setGUIReady(bool b);
    void setDigitCount(int count);

    void setLCDNumber(QVariantMap);

private:
    QVariantMap lastMap;

    bool compareMaps(QVariantMap m);

    
};

#endif // WATCHWINDOWLISTENER_H
