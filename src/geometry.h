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
    virtual ~Geometry();
//attributes
    bool isCommon;
    bool isNominal;
    QList<Geometry*> nominals;
    //TODO QMap with bool isUsed
    QList<Observation*> myObservations;
    CoordinateSystem* myNominalCoordSys;
    QMap<Configuration::ReadingTypes,QString> usedReadingTypes;


    MeasurementConfig mConfig;

    Statistic myStatistic;

//method
    void insertReadingType(Configuration::ReadingTypes readingType, QString displayName);

    virtual bool toOpenIndyXML(QXmlStreamWriter& stream) = 0;
    virtual bool fromOpenIndyXML(QXmlStreamReader& xml) = 0;

};

#endif // GEOMETRY_H
