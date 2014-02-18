#include "pluginmetadata.h"

PluginMetaData::PluginMetaData(QObject *parent) :
    QObject(parent)
{


}

void PluginMetaData::emitSendMe(){
    emit sendMe(this);
}
