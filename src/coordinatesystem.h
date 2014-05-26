#ifndef COORDINATESYSTEM_H
#define COORDINATESYSTEM_H

#include <QList>
#include "feature.h"
#include "oivec.h"

class Observation;
class TrafoParam;
class Geometry;

/*!
 * \brief The CoordinateSystem class
 * This class contains a list of observations associated with this coordinate system.
 * Also it contains all valid transformation parameters from this system to other systems.
 * The nominals list contains the nominal features in this coordinate system.
 */
class CoordinateSystem : public Feature
{
public:
    CoordinateSystem();
    virtual ~CoordinateSystem();

    OiVec origin;

    QList<Observation*> observations;
    QList<TrafoParam*> trafoParams;
    QList<Geometry*> nominals;

    void recalc();
    QString getDisplayX() const;
    QString getDisplayY() const;
    QString getDisplayZ() const;
    QString getDisplaySolved() const;

    virtual bool toOpenIndyXML(QXmlStreamWriter& stream);
    virtual ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

    //bool transformObservations(CoordinateSystem *to);
    //void setObservationState(bool valid);

    //TrafoParam* findTrafoParam(CoordinateSystem *searchToSystem);
};

#endif // COORDINATESYSTEM_H
