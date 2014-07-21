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
