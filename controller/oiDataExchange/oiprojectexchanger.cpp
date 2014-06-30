#include "oiprojectexchanger.h"

#include <QFile>

QList<Observation*> OiProjectExchanger::observations;
QList<FeatureWrapper*> OiProjectExchanger::geometries;
QList<FeatureWrapper*> OiProjectExchanger::stations;
QList<FeatureWrapper*> OiProjectExchanger::coordSystems;
QList<FeatureWrapper*> OiProjectExchanger::trafoParams;
QList<FeatureWrapper*> OiProjectExchanger::features;
QList<ElementDependencies> OiProjectExchanger::dependencies;

QList<int> OiProjectExchanger::stationElements;

ProjectRestorer* OiProjectExchanger::restorer = new ProjectRestorer();

OiProjectExchanger::OiProjectExchanger()
{

    restorer->moveToThread(&workingThread);

    workingThread.start();

}


bool OiProjectExchanger::saveProject(OiProjectData &data){

    return restorer->saveProject(data);

}

bool OiProjectExchanger::loadProject(OiProjectData &data){

    return restorer->loadProject(data);
}


bool OiProjectExchanger::saveSettings(QString name){


    return false;
}

//can be deleted... logic will be solved by the restorer
bool OiProjectExchanger::regenerateRelations(OiProjectData &data){

    foreach (FeatureWrapper *fg, OiProjectExchanger::geometries){


        if(OiProjectExchanger::regenerateFeature(fg->getFeature())){
            //TODO error handling
        }

        for(int i = 0; i< fg->getGeometry()->getObservations().size();i++){
            Observation* proxyObs = OiProjectExchanger::findObservation(fg->getGeometry()->getObservations().at(i)->getId());
            if(proxyObs != NULL){
               fg->getGeometry()->getObservations().replace(i,proxyObs);
            }
        }

        for(int i = 0; i< fg->getGeometry()->getMyNominals().size();i++){
            Geometry* proxyGeom = OiProjectExchanger::findGeometrie(fg->getGeometry()->getMyNominals().at(i)->getId());
            if(proxyGeom != NULL){
               fg->getGeometry()->getMyNominals().replace(i,proxyGeom);
            }
        }

        if(fg->getGeometry()->getNominalSystem() != NULL){
            CoordinateSystem* proxyCoord= OiProjectExchanger::findCoordSys(fg->getGeometry()->getNominalSystem()->getId());
            if(proxyCoord != NULL){
               fg->getGeometry()->setNominalSystem(proxyCoord);
            }
        }

        if(!OiProjectExchanger::stationElements.contains(fg->getGeometry()->getId())){
            data.features.append(fg);
        }


    }

    foreach (FeatureWrapper *fs, OiProjectExchanger::stations){



        if(OiProjectExchanger::regenerateFeature(fs->getFeature())){
            //TODO error handling
        }

        FeatureWrapper* proxyPoint= OiProjectExchanger::findFeature(fs->getStation()->position->getId());
        if(proxyPoint != NULL){
           fs->getStation()->position = proxyPoint->getPoint();
        }

        if(fs->getStation()->coordSys!=NULL){
            CoordinateSystem* proxyCoord= OiProjectExchanger::findCoordSys(fs->getStation()->coordSys->getId());
            if(proxyCoord != NULL){
               fs->getStation()->coordSys = proxyCoord;
            }
        }


        data.features.append(fs);

        data.stations.append(fs->getStation());
    }

    foreach (Observation *obs, OiProjectExchanger::observations){

        Station* proxyStation= OiProjectExchanger::findStation(obs->myStation->getId());
        if(proxyStation != NULL){
           obs->myStation = proxyStation;
           obs->myReading->instrument = proxyStation->sensorPad->instrument;
        }

    }

    foreach (FeatureWrapper *fc, OiProjectExchanger::coordSystems){


        if(OiProjectExchanger::regenerateFeature(fc->getFeature())){
            //TODO error handling
        }

        for(int i = 0; i< fc->getCoordinateSystem()->getObservations().size();i++){
            Observation* proxyObs = OiProjectExchanger::findObservation(fc->getCoordinateSystem()->getObservations().at(i)->getId());
            if(proxyObs != NULL){
               //fc->getCoordinateSystem()->getObservations().replace(i,proxyObs);
            }
        }

        for(int i = 0; i< fc->getCoordinateSystem()->getNominals().size();i++){
            Geometry* proxyGeom = OiProjectExchanger::findGeometrie(fc->getCoordinateSystem()->getNominals().at(i)->getId());
            if(proxyGeom != NULL){
               //fc->getCoordinateSystem()->getNominals().replace(i,proxyGeom);
            }
        }

        for(int i = 0; i< fc->getCoordinateSystem()->getTransformationParameters().size();i++){
            TrafoParam* proxyTrafoParam = OiProjectExchanger::findTrafoParam(fc->getCoordinateSystem()->getTransformationParameters().at(i)->getId());
            if(proxyTrafoParam != NULL){
               //fc->getCoordinateSystem()->getTransformationParameters(.replace(i,proxyTrafoParam);
            }
        }


        if(!OiProjectExchanger::stationElements.contains(fc->getCoordinateSystem()->getId())){
            data.features.append(fc);
            data.coordSystems.append(fc->getCoordinateSystem());
        }


    }

    foreach (FeatureWrapper *ft, OiProjectExchanger::trafoParams){



        if(OiProjectExchanger::regenerateFeature(ft->getFeature())){
            //TODO error handling
        }


        if(ft->getTrafoParam()->getStartSystem() !=NULL){
            CoordinateSystem* proxyCoord= OiProjectExchanger::findCoordSys(ft->getTrafoParam()->getStartSystem()->getId());
            if(proxyCoord != NULL){
               //ft->getTrafoParam()->from = proxyCoord;
            }
        }

        if(ft->getTrafoParam()->getDestinationSystem() !=NULL){
            CoordinateSystem* proxyCoord= OiProjectExchanger::findCoordSys(ft->getTrafoParam()->getDestinationSystem()->getId());
            if(proxyCoord != NULL){
               //ft->getTrafoParam()->to = proxyCoord;
            }
        }


        data.features.append(ft);

    }

    //sort
    //qSort(data.features.begin(), data.features.end(), sortID);

    return true;
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
}
