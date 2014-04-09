#include "geometry.h"

#include "coordinatesystem.h"
#include "observation.h"
#include "station.h"

Geometry::Geometry() : myActual(NULL){

}

Geometry::~Geometry(){

    if(this->isNominal){
        //delete this geometry from the nominal list of its actual
        if(this->myActual != NULL){
            int index = -1;
            int k = 0;
            foreach(Geometry *checkNominal, this->myActual->nominals){
                if(checkNominal != NULL && checkNominal->id == this->id){
                    index = k;
                    break;
                }
                k++;
            }
            if(index >= 0){
                this->myActual->nominals.removeAt(index);
            }
        }

        //delete this geometry from list of nominals in myNominalCoordSys
        if(this->myNominalCoordSys != NULL){
            this->myNominalCoordSys->nominals.removeOne(this);
        }
    }else{
        //delete observations that only belong to this geometry
        for(int i = 0; i < this->myObservations.size(); i++){
            Observation *myObs = this->myObservations.at(i);
            if(myObs->myTargetGeometries.size() == 1){
                Station *myStation = myObs->myStation;
                if(myStation != NULL && myStation->coordSys != NULL){
                    myStation->coordSys->observations.removeOne(myObs);
                }
            }
            delete myObs;
        }

        //delete property myActual from all nominal geometries which belong to this actual geometry
        foreach(Geometry *checkNominal, this->nominals){
            if(checkNominal != NULL){
                checkNominal->myActual = NULL;
            }
        }
    }

}

void Geometry::insertReadingType(Configuration::ReadingTypes readingType, QString displayName){

    QMap<Configuration::ReadingTypes,QString>::const_iterator i = usedReadingTypes.find(readingType);

    if (i != usedReadingTypes.end() && i.key() != readingType) {
        usedReadingTypes.insert(readingType,displayName);
    }
}
