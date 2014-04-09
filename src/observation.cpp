#include "observation.h"

#include "geometry.h"
#include "station.h"
#include "reading.h"
#include "function.h"


Observation::Observation(Reading *r, Station *s) : myReading(r), myStation(s), myXyz(4), myOriginalXyz(4), sigmaXyz(4)
{
    this->id = Configuration::generateID();
}

Observation::~Observation(){

    //delete corresponding reading
    if(this->myReading != NULL){
        delete this->myReading;
    }

    //delete this observation from target geometries
    foreach(Geometry *myGeom, this->myTargetGeometries){
        if(myGeom != NULL){
            myGeom->myObservations.removeOne(this);
            foreach(Function *myFunc, myGeom->functionList){
                if(myFunc != NULL){
                    myFunc->removeObservation(this->id);
                }
            }
        }
    }

}
