#ifndef COORDINATESYSTEM_H
#define COORDINATESYSTEM_H

#include <QList>

#include "feature.h"
#include "oivec.h"
#include "oiemitter.h"

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
    Q_OBJECT
public:
    explicit CoordinateSystem(QObject *parent = 0);
    virtual ~CoordinateSystem();

    const QList<Observation *> &getObservations() const;
    Observation * const getObservation(int observationId) const;
    bool addObservation(Observation * const observation);

    const QList<TrafoParam *> &getTransformationParameters() const;
    const QList<TrafoParam *> getTransformationParameters(CoordinateSystem * const to) const;
    bool addTransformationParameter(TrafoParam * const trafoParam);
    bool removeTransformationParameter(TrafoParam * const trafoParam);

    const QList<Geometry *> &getNominals() const;
    bool addNominal(Geometry * const nominal);
    bool removeNominal(Geometry * const nominal);

    bool getIsActiveCoordinateSystem() const;
    void setActiveCoordinateSystemState(bool isActiveCoordinateSystem);

    OiVec origin;

    OiVec getExpansionOrigin();
    void setExpansionOrigin(OiVec expOri);
    void setExpansionOriginX(double x);
    void setExpansionOriginY(double y);
    void setExpansionOriginZ(double z);

    void recalc();

    QString getDisplayX() const;
    QString getDisplayY() const;
    QString getDisplayZ() const;
    QString getDisplaySolved() const;
    QString getDisplayExpansionOriginX() const;
    QString getDisplayExpansionOriginY() const;
    QString getDisplayExpansionOriginZ() const;

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc);
    bool fromOpenIndyXML(QDomElement &xmlElem);
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

    bool isStationSystem;

private:

    /*
    This point represents the point from which the Part object expans with temperature
    Necessary for movement transformations (watch trafocontroller.h and trafocontroller.cpp)
    */
    OiVec expansionOrigin;

signals:
    void observationsChanged(int coordId);
    void transformationParametersChanged(int coordId);
    void nominalsChanged(int coordId);
    void activeCoordinateSystemChanged(int coordId);

protected:
    QList<Observation*> observations;
    QList<TrafoParam*> trafoParams;
    QList<Geometry*> nominals;

    bool isActiveCoordinateSystem;
};

#endif // COORDINATESYSTEM_H
