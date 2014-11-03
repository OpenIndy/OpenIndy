#include "oiprojectdata.h"
#include "oiprojectexchanger.h"

OiProjectData *OiProjectData::activeProject = NULL;

OiProjectData::OiProjectData(QObject *parent) :
    QObject(parent)
{
    this->device = NULL;
    this->isSaved = true;
    this->projectName = "";
}

/*!
 * \brief OiProjectData::getInstance
 * \return
 */
OiProjectData *OiProjectData::getInstance(){
    if(OiProjectData::activeProject == NULL){
        OiProjectData::activeProject = new OiProjectData();
    }
    return OiProjectData::activeProject;
}

/*!
 * \brief OiProjectData::getDevice
 * Returns the currently selected device or NULL
 * \return
 */
QIODevice *OiProjectData::getDevice(){
    return OiProjectData::activeProject->device;
}

/*!
 * \brief OiProjectData::getProjectName
 * \return
 */
QString OiProjectData::getProjectName(){
    return OiProjectData::activeProject->projectName;
}

/*!
 * \brief OiProjectData::getIsValid
 * Returns true if the project has a name
 * \return
 */
bool OiProjectData::getIsValid(){
    if(OiProjectData::getInstance()->projectName.compare("") != 0){
        return true;
    }
    return false;
}

/*!
 * \brief OiProjectData::getIsSaved
 * \return
 */
bool OiProjectData::getIsSaved(){
    return OiProjectData::getInstance()->isSaved;
}

/*!
 * \brief OiProjectData::setActiveProject
 * \param name
 * \param device
 * \return
 */
bool OiProjectData::setActiveProject(QString name, QIODevice *device){
    try{

        //delete the old device
        if(OiProjectData::getInstance()->device != NULL){
            if(OiProjectData::getInstance()->device->isOpen()){
                OiProjectData::getInstance()->device->close();
            }
            delete OiProjectData::getInstance()->device;
        }

        //set new project
        OiProjectData::getInstance()->projectName = name;
        OiProjectData::getInstance()->device = device;

        return true;

    }catch(exception &e){
        Console::addLine(e.what());
    }
    return false;
}
