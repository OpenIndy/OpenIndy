#include "oiemitter.h"

OiEmitter::OiEmitter(QObject *parent) :
    QObject(parent)
{
}

void OiEmitter::emitSendDataMap(QVariantMap*m){
    emit sendDataMap(m);
}

void OiEmitter::emitSendString(QString s){

    emit sendString(s);

}
