#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "feature.h"
#include "measurementconfig.h"
#include "statistic.h"

class Observation;
class CoordinateSystem;

/*!
 * \brief The Geometry class
 *
 */
class Geometry : public Feature
{
public:
//constructor
    Geometry();
    virtual ~Geometry();
//attributes
    bool isCommon;
    bool isNominal;
    QList<Geometry*> nominals;
    Geometry *myActual;
    //TODO QMap with bool isUsed
    QList<Observation*> myObservations;
    CoordinateSystem* myNominalCoordSys;
    QMap<Configuration::ReadingTypes,QString> usedReadingTypes;


    MeasurementConfig mConfig;

    Statistic myStatistic;

//method
    void insertReadingType(Configuration::ReadingTypes readingType, QString displayName);


};

#endif // GEOMETRY_H
