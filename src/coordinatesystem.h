#ifndef COORDINATESYSTEM_H
#define COORDINATESYSTEM_H

#include <QObject>
#include <QList>
#include <QPointer>

#include "feature.h"
#include "position.h"
#include "direction.h"

class Observation;
class TrafoParam;
class Geometry;
class Station;

/*!
 * \brief The CoordinateSystem class
 */
class CoordinateSystem : public Feature
{
    Q_OBJECT

public:
    explicit CoordinateSystem(QObject *parent = 0);
    explicit CoordinateSystem(const QPointer<Station> &station, QObject *parent = 0);

    CoordinateSystem(const CoordinateSystem &copy, QObject *parent = 0);

    CoordinateSystem &operator=(const CoordinateSystem &copy);

    ~CoordinateSystem();

    //#######################################
    //get or set coordinate system attributes
    //#######################################

    const bool &getIsActiveCoordinateSystem() const;
    void setActiveCoordinateSystemState(const bool &isActiveCoordinateSystem);

    const bool &getIsStationSystem() const;
    const QPointer<Station> &getStation() const;

    const Position &getOrigin() const;
    const Direction &getXAxis() const;
    const Direction &getYAxis() const;
    const Direction &getZAxis() const;

    void setCoordinateSystem(const Position &origin, const Direction &xAxis, const Direction &yAxis, const Direction &zAxis);

    const Position &getExpansionOrigin() const;
    void setExpansionOrigin(const Position &expansionOrigin);

    //##################################################
    //get or set observations, trafo params and nominals
    //##################################################

    const QList<QPointer<Observation> > &getObservations() const;
    QPointer<Observation> getObservation(const int &observationId) const;
    bool addObservation(const QPointer<Observation> &observation);

    const QList<QPointer<TrafoParam> > &getTransformationParameters() const;
    const QList<QPointer<TrafoParam> > getTransformationParameters(const QPointer<CoordinateSystem> &to) const;
    bool addTransformationParameter(const QPointer<TrafoParam> &trafoParam);
    bool removeTransformationParameter(const QPointer<TrafoParam> &trafoParam);

    const QList<QPointer<FeatureWrapper> > &getNominals() const;
    bool addNominal(const QPointer<FeatureWrapper> &nominal);
    bool addNominals(const QList< QPointer<FeatureWrapper> > &nominals);
    bool removeNominal(const QPointer<FeatureWrapper> &nominal);
    bool removeNominal(const int &featureId);

    //###########################
    //reexecute the function list
    //###########################

    void recalc();

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc);
    bool fromOpenIndyXML(QDomElement &xmlElem);

signals:

    //#################################################
    //signals to inform about coordinate system changes
    //#################################################

    void observationsChanged(const int &coordId, const int &obsId);
    void transformationParametersChanged(const int &coordId);
    void nominalsChanged(const int &coordId);
    void activeCoordinateSystemChanged(const int &coordId);

private:

    //####################################
    //general coordinate system attributes
    //####################################

    Position origin; //origin of the coordinate system in the current display coordinate system

    Direction xAxis; //x axis of the coordinate system in the current display coordinate system
    Direction yAxis; //y axis of the coordinate system in the current display coordinate system
    Direction zAxis; //z axis of the coordinate system in the current display coordinate system

    bool isStationSystem; //true if the coordinate system is a station system

    bool isActiveCoordinateSystem;

    Position expansionOrigin; //represents the point from which the Part object expands with temperature

    QPointer<Station> station; //holds the station object (only if this is a station system)

    //#######################################
    //observations, trafo params and nominals
    //#######################################

    QList< QPointer<Observation> > observationsList;
    QMap< int, QPointer<Observation> > observationsMap; //same observations but id as key

    QList< QPointer<TrafoParam> > trafoParams;

    QList< QPointer<FeatureWrapper> > nominalsList;
    QMap< int, QPointer<FeatureWrapper> > nominalsMap; //same geometries but id as key


    //QString getDisplayX() const;
    //QString getDisplayY() const;
    //QString getDisplayZ() const;
    //QString getDisplaySolved() const;
    //QString getDisplayExpansionOriginX() const;
    //QString getDisplayExpansionOriginY() const;
    //QString getDisplayExpansionOriginZ() const;

};

#endif // COORDINATESYSTEM_H
