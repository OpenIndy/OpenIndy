#ifndef COORDINATESYSTEM_H
#define COORDINATESYSTEM_H

#include <QList>
#include <QPointer>

#include "featurewrapper.h"
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

    const QList< QPointer<Observation> > &getObservations() const;
    QPointer<Observation> getObservation(const int &observationId) const;
    bool addObservation(QPointer<Observation> const &observation);

    const QList< QPointer<TrafoParam> > &getTransformationParameters() const;
    const QList< QPointer<TrafoParam> > getTransformationParameters(QPointer<CoordinateSystem> const &to) const;
    bool addTransformationParameter(QPointer<TrafoParam> const &trafoParam);
    bool removeTransformationParameter(QPointer<TrafoParam> const &trafoParam);

    const QList< QPointer<FeatureWrapper> > &getNominals() const;
    bool addNominal(QPointer<FeatureWrapper> const &nominal);
    bool addNominals(const QList< QPointer<FeatureWrapper> > &nominals);
    bool removeNominal(QPointer<FeatureWrapper> const &nominal);
    bool removeNominal(const int &featureId);

    bool getIsActiveCoordinateSystem() const;
    void setActiveCoordinateSystemState(const bool &isActiveCoordinateSystem);

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

    bool isStationSystem;

private:

    /*
    This point represents the point from which the Part object expans with temperature
    Necessary for movement transformations (watch trafocontroller.h and trafocontroller.cpp)
    */
    OiVec expansionOrigin;

signals:
    void observationsChanged(const int &coordId, const int &obsId);
    void transformationParametersChanged(const int &coordId);
    void nominalsChanged(const int &coordId);
    void activeCoordinateSystemChanged(const int &coordId);

protected:
    QList< QPointer<Observation> > observations;
    QList< QPointer<TrafoParam> > trafoParams;
    QList< QPointer<FeatureWrapper> > nominals;

    bool isActiveCoordinateSystem;
};

#endif // COORDINATESYSTEM_H
