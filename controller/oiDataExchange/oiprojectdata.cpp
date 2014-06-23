#include "oiprojectdata.h"

OiProjectData::OiProjectData(QObject *parent) :
    QObject(parent)
{

    device = NULL;
    activeCoordSystem = NULL;
    activeStation = NULL;

}

OiProjectData::~OiProjectData(){
    try{

        if(this->device != NULL){
            if(this->device->isOpen()){
                this->device->close();
            }
            delete this->device;
        }

    }catch(exception &e){
        Console::addLine(e.what());
    }
}

/*!
 * \brief OiProjectData::getDevice
 * Returns the currently selected device or NULL
 * \return
 */
QIODevice *OiProjectData::getDevice(){
    return this->device;
}

/*!
 * \brief OiProjectData::setDevice
 * Sets the current device and if needed deletes the old one
 * \return
 */
bool OiProjectData::setDevice(QIODevice *device){

    //delete the old device
    try{
        if(this->device != NULL){
            if(this->device->isOpen()){
                this->device->close();
                delete this->device;
            }
        }
    }catch(exception &e){
        Console::addLine(e.what());
    }

    //set new device
    if(device != NULL){
        this->device = device;
        return true;
    }

    return false;

}
