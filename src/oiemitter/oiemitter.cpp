#include "oiemitter.h"

OiEmitter::OiEmitter(QObject *parent) :
    QObject(parent)
{
}


void OiEmitter::emitSendString(QString s) const{

    emit this->sendString(s);

}




