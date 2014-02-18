#include "observation.h"

#include "geometry.h"
#include "station.h"
#include "reading.h"


Observation::Observation(Reading *r, Station *s) : myReading(r), myStation(s), myXyz(4), myOriginalXyz(4), sigmaXyz(4)
{
    this->id = Configuration::generateID();
}

Observation::~Observation(){

}
