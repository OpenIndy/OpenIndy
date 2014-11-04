#include "oiprojectexchanger.h"

#include <QFile>

/*QList<Observation*> OiProjectExchanger::observations;
QList<FeatureWrapper*> OiProjectExchanger::geometries;
QList<FeatureWrapper*> OiProjectExchanger::stations;
QList<FeatureWrapper*> OiProjectExchanger::coordSystems;
QList<FeatureWrapper*> OiProjectExchanger::trafoParams;
QList<FeatureWrapper*> OiProjectExchanger::features;
QList<ElementDependencies> OiProjectExchanger::dependencies;

QList<int> OiProjectExchanger::stationElements;

ProjectRestorer* OiProjectExchanger::restorer = new ProjectRestorer();*/

/*OiProjectExchanger *OiProjectExchanger::exchanger = NULL;

OiProjectExchanger::OiProjectExchanger()
{
}*/

/*!
 * \brief OiProjectExchanger::getInstance
 * \return
 */
/*OiProjectExchanger *OiProjectExchanger::getInstance(){
    if(OiProjectExchanger::exchanger == NULL){
        OiProjectExchanger::exchanger = new OiProjectExchanger();
    }
    return OiProjectExchanger::exchanger;
}*/

/*OiProjectExchanger::OiProjectExchanger()
{

    restorer->moveToThread(&workingThread);

    workingThread.start();

}*/




/*bool OiProjectExchanger::saveProject(OiProjectData &data){

    return restorer->saveProject(data);

}

bool OiProjectExchanger::loadProject(OiProjectData &data){

    return restorer->loadProject(data);
}


bool OiProjectExchanger::saveSettings(QString name){


    return false;
}



Observation* OiProjectExchanger::findObservation(int id){

    foreach(Observation *obs, OiProjectExchanger::observations){
        if (obs->getId() == id){
            return obs;
        }
    }
    return NULL;
}

Geometry* OiProjectExchanger::findGeometrie(int id){

    foreach(FeatureWrapper *g, OiProjectExchanger::geometries){
        if (g->getGeometry()->getId() == id){
            return g->getGeometry();
        }
    }
    return NULL;
}

CoordinateSystem* OiProjectExchanger::findCoordSys(int id){
    foreach(FeatureWrapper *g, OiProjectExchanger::coordSystems){
        if (g->getCoordinateSystem()->getId() == id){
            return g->getCoordinateSystem();
        }
    }
    return NULL;
}

TrafoParam* OiProjectExchanger::findTrafoParam(int id){
    foreach(FeatureWrapper *g, OiProjectExchanger::trafoParams){
        if (g->getTrafoParam()->getId() == id){
            return g->getTrafoParam();
        }
    }
    return NULL;
}

FeatureWrapper* OiProjectExchanger::findFeature(int id){

    foreach(FeatureWrapper *f, OiProjectExchanger::features){
        if (f->getFeature()->getId() == id){
            return f;
        }
    }
    return NULL;
}

Station* OiProjectExchanger::findStation(int id){

    foreach(FeatureWrapper *f, OiProjectExchanger::stations){
        if (f->getStation()->getId() == id){
            return f->getStation();
        }
    }
    return NULL;

}

bool OiProjectExchanger::regenerateFeature(Feature *f){

    for(int i = 0;i<f->usedFor.size();i++){
        FeatureWrapper *proxyFw = OiProjectExchanger::findFeature(f->usedFor.at(i)->getFeature()->getId());
        if(proxyFw != NULL){
            f->usedFor.replace(i, proxyFw);
        }
    }

    for(int i = 0;i<f->previouslyNeeded.size();i++){
        FeatureWrapper *proxyFw = OiProjectExchanger::findFeature(f->previouslyNeeded.at(i)->getFeature()->getId());
        if(proxyFw != NULL){
            f->previouslyNeeded.replace(i,proxyFw);
        }
    }

    return true;
}*/

/*!
 * \brief OiProjectExchanger::saveProject
 * \return
 */
QDomDocument OiProjectExchanger::saveProject(){

    QDomDocument project;

    if(!OiProjectData::getIsValid()){
        Console::addLine("No project available");
        return project;
    }

    //get current date and time
    QString dateTime = QDateTime::currentDateTime().toString(Qt::ISODate);

    //create XML root and set general project attributes
    QDomElement root = project.createElement("oiProjectData");
    root.setAttribute("name", OiProjectData::getProjectName());
    root.setAttribute("date", dateTime);
    root.setAttribute("idCount", QString::number(Configuration::idCount));
    project.appendChild(root);

    //add active station and active coordinate system
    QDomElement activeSystem = project.createElement("activeCoordinateSystem");
    QDomElement activeStation = project.createElement("activeStation");
    int activeSystemId, activeStationId = -1;
    if(OiFeatureState::getActiveCoordinateSystem() != NULL){
        activeSystemId = OiFeatureState::getActiveCoordinateSystem()->getId();
    }
    if(OiFeatureState::getActiveStation() != NULL){
        activeStationId = OiFeatureState::getActiveStation()->getId();
    }
    activeSystem.setAttribute("ref", activeSystemId);
    activeStation.setAttribute("ref", activeStationId);
    root.appendChild(activeSystem);
    root.appendChild(activeStation);

    //add stations
    QDomElement stations = project.createElement("stations");
    foreach(Station *s, OiFeatureState::getStations()){
        if(s != NULL){
            QDomElement station = s->toOpenIndyXML(project);
            if(!station.isNull()){
                stations.appendChild(station);
            }
        }
    }
    root.appendChild(stations);

    //add coordinate systems
    QDomElement coordinateSystems = project.createElement("coordinateSystems");
    foreach(CoordinateSystem *c, OiFeatureState::getCoordinateSystems()){
        if(c != NULL){
            QDomElement coordinateSystem = c->toOpenIndyXML(project);
            if(!coordinateSystem.isNull()){
                coordinateSystems.appendChild(coordinateSystem);
            }
        }
    }
    root.appendChild(coordinateSystems);

    //add trafo params
    QDomElement trafoParams = project.createElement("transformationParameters");
    foreach(TrafoParam *tp, OiFeatureState::getTransformationParameters()){
        if(tp != NULL){
            QDomElement trafoParam = tp->toOpenIndyXML(project);
            if(!trafoParam.isNull()){
                trafoParams.appendChild(trafoParam);
            }
        }
    }
    root.appendChild(trafoParams);

    //add geometries
    QDomElement geometries = project.createElement("geometries");
    foreach(FeatureWrapper *geom, OiFeatureState::getGeometries()){ //normal geometries
        if(geom != NULL){
            QDomElement geometry = geom->getGeometry()->toOpenIndyXML(project);
            if(!geometry.isNull()){
                geometries.appendChild(geometry);
            }
        }
    }
    foreach(Station *s, OiFeatureState::getStations()){ //station points
        if(s != NULL && s->position != NULL){
            QDomElement geometry = s->position->toOpenIndyXML(project);
            if(!geometry.isNull()){
                geometries.appendChild(geometry);
            }
        }
    }
    root.appendChild(geometries);

    //add observations (search the station coordinate systems for observations)
    QDomElement observations = project.createElement("observations");
    foreach(Station *s, OiFeatureState::getStations()){
        if(s != NULL && s->coordSys != NULL && s->coordSys->getObservations().size() > 0){
            foreach(Observation *obs, s->coordSys->getObservations()){
                if(obs != NULL){
                    QDomElement observation = obs->toOpenIndyXML(project);
                    if(!observation.isNull()){
                        observations.appendChild(observation);
                    }
                }
            }
        }
    }
    root.appendChild(observations);

    return project;

}

/*!
 * \brief OiProjectExchanger::loadProject
 * \param project
 * \return
 */
bool OiProjectExchanger::loadProject(QDomDocument project){

    //delete old features (from a previous project)
    OiFeatureState::resetFeatureLists();

    //load all observations
    QList<Observation*> myObservations;
    QDomElement observations = project.firstChildElement("observations");
    if(!observations.isNull()){
        QDomNodeList observationList = observations.childNodes();
        for(int i = 0; i < observationList.size(); i++){
            Observation *obs = new Observation();
            if(!obs->fromOpenIndyXML(observationList.at(i).toElement())){
                delete obs;
                continue;
            }
            myObservations.append(obs);
        }
    }

    //load all stations
    QList<Station*> myStations;

    //load all coordinate systems

    //load all transformation parameters

    //load all geometries

    //set active station and active coordinate system

    //set general project data

    return true;

}
