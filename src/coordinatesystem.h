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
class TrafoController;
class ProjectExchanger;

/*!
 * \brief The CoordinateSystem class
 */
class CoordinateSystem : public Feature
{
    friend class Station;
    friend class OiJob;
    friend class TrafoController;
    friend class ProjectExchanger;
    friend class Observation;
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

    //###############
    //display methods
    //###############

    QString getDisplayX(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    QString getDisplayY(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    QString getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff = false) const;

    QString getDisplayExpansionOriginX(const UnitType &type, const int &digits) const;
    QString getDisplayExpansionOriginY(const UnitType &type, const int &digits) const;
    QString getDisplayExpansionOriginZ(const UnitType &type, const int &digits) const;
    QString getDisplayXAxisI(const int &digits) const;
    QString getDisplayXAxisJ(const int &digits) const;
    QString getDisplayXAxisK(const int &digits) const;
    QString getDisplayYAxisI(const int &digits) const;
    QString getDisplayYAxisJ(const int &digits) const;
    QString getDisplayYAxisK(const int &digits) const;
    QString getDisplayZAxisI(const int &digits) const;
    QString getDisplayZAxisJ(const int &digits) const;
    QString getDisplayZAxisK(const int &digits) const;

signals:

    //#################################################
    //signals to inform about coordinate system changes
    //#################################################

    void observationsChanged(const int &coordId, const int &obsId);
    void transformationParametersChanged(const int &coordId);
    void nominalsChanged(const int &coordId);
    void activeCoordinateSystemChanged(const int &coordId);

protected:

    //######################################
    //update feature id when the job was set
    //######################################

    void setUpFeatureId();

private slots:

    //##############
    //helper methods
    //##############

    void removeObservation(const QPointer<Observation> &obs);

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

};

#endif // COORDINATESYSTEM_H
