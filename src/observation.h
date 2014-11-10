#ifndef OBSERVATION_H
#define OBSERVATION_H

#include <QList>
#include <math.h>
#include "element.h"
#include "configuration.h"
#include "geometry.h"
#include "oivec.h"
#include "statistic.h"

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

    Observation(Reading*, Station*, bool isActiveCoordSys);
    Observation();
    ~Observation();

    Station *myStation;
    QList<Geometry*> myTargetGeometries;
    Reading *myReading;

    OiVec myXyz;
    OiVec myOriginalXyz;

    Statistic myStatistic;
    Statistic myOriginalStatistic;


    OiVec sigmaXyz;

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

    virtual ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

    bool writeProxyObservations(QXmlStreamWriter& stream);

    void calcFromReading();

    void setIsValid(bool isValid);
    bool getIsValid();

    void setIsSolved(bool isSolved);
    bool getIsSolved();

    bool getUseState();

 private:

    bool isActiveCoordSys; //indicates if the observation coord system is the current
        //coordinate system at time of creation

    bool isValid; //defines if the reading can be transformed, without any function, to a xyz observation

    bool isSolved; //defines wether this observation is valid in current coordinate system

    Reading* readReading(QXmlStreamReader& xml);
};

#endif // OBSERVATION_H
