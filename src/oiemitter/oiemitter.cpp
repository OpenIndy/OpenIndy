#include "oiemitter.h"

OiEmitter::OiEmitter(QObject *parent) :
    QObject(parent)
{
}


void OiEmitter::emitSendString(QString s){

    emit sendString(s);

}




