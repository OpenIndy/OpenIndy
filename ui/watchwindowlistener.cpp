#include "watchwindowlistener.h"

WatchWindowListener::WatchWindowListener(QObject *parent) :
    QObject(parent)
{
    this->isSettingsReady = false;
    this->isCheckBoxReady = false;
    this->isGUIReady = false;
    digitCount = 4;
}

void WatchWindowListener::setSettingsReady(bool b)
{
    this->isSettingsReady = b;
}

void WatchWindowListener::setCheckBoxReady(bool b)
{
    this->isCheckBoxReady = b;
}

void WatchWindowListener::setGUIReady(bool b)
{
    this->isGUIReady = b;
}

void WatchWindowListener::setDigitCount(int count)
{
    this->digitCount = count;
}

void WatchWindowListener::setLCDNumber(QVariantMap m)
{
    if(!isSettingsReady){
        return;
    }
    QStringList attributeNames;

    QMapIterator<QString, QVariant> j(m);
    while(j.hasNext()){
        j.next();

        attributeNames.append(j.key());
    }
    if(!isCheckBoxReady){
        emit sendAttributeNames(attributeNames);
    }else{
        emit sendData(m);
    }
}

