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

    virtual OiVec* getXYZ(){
        return NULL;
    }
    virtual OiVec* getIJK(){
        return NULL;
    }


    MeasurementConfig mConfig;

    Statistic myStatistic;

//method
    void insertReadingType(Configuration::ReadingTypes readingType, QString displayName);

    virtual bool toOpenIndyXML(QXmlStreamWriter& stream) = 0;
    virtual ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml) = 0;

protected:
    bool writeGeometryAttributes(QXmlStreamWriter& stream);
    bool readGeometryAttributes(QXmlStreamReader &xml ,ElementDependencies &d);

};

#endif // GEOMETRY_H
