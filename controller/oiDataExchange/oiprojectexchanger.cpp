#include "oiprojectexchanger.h"

#include <QFile>

QList<Observation*> oiProjectExchanger::observations;
QList<FeatureWrapper*> oiProjectExchanger::geometries;
QList<FeatureWrapper*> oiProjectExchanger::stations;
QList<FeatureWrapper*> oiProjectExchanger::coordSystems;
QList<FeatureWrapper*> oiProjectExchanger::trafoParams;
QList<FeatureWrapper*> oiProjectExchanger::features;
QList<ElementDependencies> oiProjectExchanger::dependencies;

QList<int> oiProjectExchanger::stationElements;

ProjectRestorer* oiProjectExchanger::restorer = new ProjectRestorer();

oiProjectExchanger::oiProjectExchanger()
{

    restorer->moveToThread(&workingThread);

    workingThread.start();

}


bool oiProjectExchanger::saveProject(oiProjectData &data){

    return restorer->saveProject(data);

}

bool oiProjectExchanger::loadProject(oiProjectData &data){

    return restorer->loadProject(data);
}


bool oiProjectExchanger::saveSettings(QString name){


    return false;
}

//can be deleted... logic will be solved by the restorer
bool oiProjectExchanger::regenerateRelations(oiProjectData &data){

    foreach (FeatureWrapper *fg, oiProjectExchanger::geometries){


        if(oiProjectExchanger::regenerateFeature(fg->getFeature())){
            //TODO error handling
        }

        for(int i = 0; i< fg->getGeometry()->getObservations().size();i++){
            Observation* proxyObs = oiProjectExchanger::findObservation(fg->getGeometry()->getObservations().at(i)->getId());
            if(proxyObs != NULL){
               fg->getGeometry()->getObservations().replace(i,proxyObs);
            }
        }

        for(int i = 0; i< fg->getGeometry()->getMyNominals().size();i++){
            Geometry* proxyGeom = oiProjectExchanger::findGeometrie(fg->getGeometry()->getMyNominals().at(i)->getId());
            if(proxyGeom != NULL){
               fg->getGeometry()->getMyNominals().replace(i,proxyGeom);
            }
        }

        if(fg->getGeometry()->getNominalSystem() != NULL){
            CoordinateSystem* proxyCoord= oiProjectExchanger::findCoordSys(fg->getGeometry()->getNominalSystem()->getId());
            if(proxyCoord != NULL){
               fg->getGeometry()->setNominalSystem(proxyCoord);
            }
        }

        if(!oiProjectExchanger::stationElements.contains(fg->getGeometry()->getId())){
            data.features.append(fg);
        }


    }

    foreach (FeatureWrapper *fs, oiProjectExchanger::stations){



        if(oiProjectExchanger::regenerateFeature(fs->getFeature())){
            //TODO error handling
        }

        FeatureWrapper* proxyPoint= oiProjectExchanger::findFeature(fs->getStation()->position->getId());
        if(proxyPoint != NULL){
           fs->getStation()->position = proxyPoint->getPoint();
        }

        if(fs->getStation()->coordSys!=NULL){
            CoordinateSystem* proxyCoord= oiProjectExchanger::findCoordSys(fs->getStation()->coordSys->getId());
            if(proxyCoord != NULL){
               fs->getStation()->coordSys = proxyCoord;
            }
        }


        data.features.append(fs);

        data.stations.append(fs->getStation());
    }

    foreach (Observation *obs, oiProjectExchanger::observations){

        Station* proxyStation= oiProjectExchanger::findStation(obs->myStation->getId());
        if(proxyStation != NULL){
           obs->myStation = proxyStation;
           obs->myReading->instrument = proxyStation->sensorPad->instrument;
        }

    }

    foreach (FeatureWrapper *fc, oiProjectExchanger::coordSystems){


        if(oiProjectExchanger::regenerateFeature(fc->getFeature())){
            //TODO error handling
        }

        for(int i = 0; i< fc->getCoordinateSystem()->getObservations().size();i++){
            Observation* proxyObs = oiProjectExchanger::findObservation(fc->getCoordinateSystem()->getObservations().at(i)->getId());
            if(proxyObs != NULL){
               //fc->getCoordinateSystem()->getObservations().replace(i,proxyObs);
            }
        }

        for(int i = 0; i< fc->getCoordinateSystem()->getNominals().size();i++){
            Geometry* proxyGeom = oiProjectExchanger::findGeometrie(fc->getCoordinateSystem()->getNominals().at(i)->getId());
            if(proxyGeom != NULL){
               //fc->getCoordinateSystem()->getNominals().replace(i,proxyGeom);
            }
        }

        for(int i = 0; i< fc->getCoordinateSystem()->getTransformationParameters().size();i++){
            TrafoParam* proxyTrafoParam = oiProjectExchanger::findTrafoParam(fc->getCoordinateSystem()->getTransformationParameters().at(i)->getId());
            if(proxyTrafoParam != NULL){
               //fc->getCoordinateSystem()->getTransformationParameters(.replace(i,proxyTrafoParam);
            }
        }


        if(!oiProjectExchanger::stationElements.contains(fc->getCoordinateSystem()->getId())){
            data.features.append(fc);
            data.coordSystems.append(fc->getCoordinateSystem());
        }


    }

    foreach (FeatureWrapper *ft, oiProjectExchanger::trafoParams){



        if(oiProjectExchanger::regenerateFeature(ft->getFeature())){
            //TODO error handling
        }


        if(ft->getTrafoParam()->getStartSystem() !=NULL){
            CoordinateSystem* proxyCoord= oiProjectExchanger::findCoordSys(ft->getTrafoParam()->getStartSystem()->getId());
            if(proxyCoord != NULL){
               //ft->getTrafoParam()->from = proxyCoord;
            }
        }

        if(ft->getTrafoParam()->getDestinationSystem() !=NULL){
            CoordinateSystem* proxyCoord= oiProjectExchanger::findCoordSys(ft->getTrafoParam()->getDestinationSystem()->getId());
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

Observation* oiProjectExchanger::findObservation(int id){

    foreach(Observation *obs, oiProjectExchanger::observations){
        if (obs->getId() == id){
            return obs;
        }
    }
    return NULL;
}

Geometry* oiProjectExchanger::findGeometrie(int id){

    foreach(FeatureWrapper *g, oiProjectExchanger::geometries){
        if (g->getGeometry()->getId() == id){
            return g->getGeometry();
        }
    }
    return NULL;
}

CoordinateSystem* oiProjectExchanger::findCoordSys(int id){
    foreach(FeatureWrapper *g, oiProjectExchanger::coordSystems){
        if (g->getCoordinateSystem()->getId() == id){
            return g->getCoordinateSystem();
        }
    }
    return NULL;
}

TrafoParam* oiProjectExchanger::findTrafoParam(int id){
    foreach(FeatureWrapper *g, oiProjectExchanger::trafoParams){
        if (g->getTrafoParam()->getId() == id){
            return g->getTrafoParam();
        }
    }
    return NULL;
}

FeatureWrapper* oiProjectExchanger::findFeature(int id){

    foreach(FeatureWrapper *f, oiProjectExchanger::features){
        if (f->getFeature()->getId() == id){
            return f;
        }
    }
    return NULL;
}

Station* oiProjectExchanger::findStation(int id){

    foreach(FeatureWrapper *f, oiProjectExchanger::stations){
        if (f->getStation()->getId() == id){
            return f->getStation();
        }
    }
    return NULL;

}

bool oiProjectExchanger::regenerateFeature(Feature *f){

    for(int i = 0;i<f->usedFor.size();i++){
        FeatureWrapper *proxyFw = oiProjectExchanger::findFeature(f->usedFor.at(i)->getFeature()->getId());
        if(proxyFw != NULL){
            f->usedFor.replace(i, proxyFw);
        }
    }

    for(int i = 0;i<f->previouslyNeeded.size();i++){
        FeatureWrapper *proxyFw = oiProjectExchanger::findFeature(f->previouslyNeeded.at(i)->getFeature()->getId());
        if(proxyFw != NULL){
            f->previouslyNeeded.replace(i,proxyFw);
        }
    }

    return true;
}
