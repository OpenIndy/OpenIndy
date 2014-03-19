#ifndef OBSERVATION_H
#define OBSERVATION_H

#include <QList>
#include <math.h>
#include "element.h"
#include "configuration.h"
#include "geometry.h"
#include "oivec.h"

class Geometry;
class Reading;
class Station;

/*!
 * \brief The Observation class
 * Contains all observations derived by a rading.
 */

class Observation : public Element
{
public:

//constructor
    Observation(Reading*, Station*);
    ~Observation();

//attributes
    Station *myStation;
    QList<Geometry*> myTargetGeometries;
    Reading *myReading;

    OiVec myXyz;
    OiVec myOriginalXyz;

    bool isValid; //defines wether this observation is valid in current coordinate system
    OiVec sigmaXyz;
    //TODO myoriginal sigma für Drehung bei Transformation

//methods
    virtual bool toOpenIndyXML(QXmlStreamWriter& stream);

    virtual ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

    bool writeProxyObservations(QXmlStreamWriter& stream);

 private:
    Reading* readReading(QXmlStreamReader& xml);
};

#endif // OBSERVATION_H
