#include "oiprojectdata.h"

OiProjectData::OiProjectData(QObject *parent) :
    QObject(parent)
{

    device = NULL;
    this->isSaved = true;

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

/*!
 * \brief OiProjectData::getProjectName
 * \return
 */
QString OiProjectData::getProjectName(){
    return this->projectName;
}

/*!
 * \brief OiProjectData::setProjectName
 * \param name
 */
void OiProjectData::setProjectName(QString name){
    this->projectName = name;
}

/*!
 * \brief OiProjectData::getIsValid
 * Returns true if the project has a name and a device
 * \return
 */
bool OiProjectData::getIsValid(){
    if(this->projectName.compare("") != 0 && this->device != NULL){
        return true;
    }
    return false;
}

/*!
 * \brief OiProjectData::save
 * \return
 */
bool OiProjectData::save(){
    if(this->device != NULL){

        bool isSuccessful = false;
        isSuccessful = OiProjectExchanger::saveProject(*this);
        return isSuccessful;

    }else{
        Console::addLine("No device selected");
        return false;
    }
}

/*!
 * \brief OiProjectData::getIsSaved
 * \return
 */
bool OiProjectData::getIsSaved(){
    return this->isSaved;
}
