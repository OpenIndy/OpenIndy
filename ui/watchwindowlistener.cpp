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

    /*if(!isCheckBoxReady){

        emit sendAttributeNames(attributeNames);
    }else{
        emit sendData(m);
    }*/

    if(!compareMaps(m)){
        this->lastMap = m;
        isGUIReady = false;
        emit sendAttributeNames(attributeNames);
    }else{
        emit sendData(m);
    }


}

bool WatchWindowListener::compareMaps(QVariantMap m)
{
    if(m.size() != lastMap.size()){
        return false;
    }
    QList<QString> keys = m.keys();

    int keyCount = lastMap.size();
    int keyFound = 0;

    if(keyCount == 0){
        return false;
    }

    QMapIterator<QString, QVariant> j(this->lastMap);
    while(j.hasNext()){
        j.next();

        if(keys.contains(j.key())){
            keyFound++;
        }
    }

    if(keyCount == keyFound){
        return true;
    }else{
        return false;
    }
}

