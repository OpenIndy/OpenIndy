#ifndef OBSERVATION_H
#define OBSERVATION_H

#include <QObject>
#include <QList>
#include <math.h>
#include <QPointer>

#include "element.h"
#include "geometry.h"
#include "oivec.h"
#include "statistic.h"

class Reading;
class Station;
class CoordinateSystem;
class TrafoController;

class Observation : public Element
{
    friend class Reading;
    friend class CoordinateSystem;
    friend class TrafoController;
    Q_OBJECT

public:
    explicit Observation(QObject *parent = 0);

    Observation(const Observation &copy, QObject *parent = 0);

    Observation &operator=(const Observation &copy);

    ~Observation();

    //##################################
    //get or set observations attributes
    //##################################

    const OiVec &getXYZ() const;
    void setXYZ(const OiVec &xyz);

    const OiVec &getOriginalXYZ() const;

    const Statistic &getStatistic() const;
    const Statistic &getOriginalStatistic() const;

    //const OiVec &getSigmaXYZ() const;

    //##################################################
    //get and set station, reading and target geometries
    //##################################################

    const QPointer<Station> &getStation() const;
    void setStation(const QPointer<Station> &station);

    const QList<QPointer<Geometry> > &getTargetGeometries() const;
    void addTargetGeometry(const QPointer<Geometry> &targetGeometry);
    void removeTargetGeometry(const QPointer<Geometry> &targetGeometry);
    void removeTargetGeometry(const int &geomId);

    const QPointer<Reading> &getReading() const;

    //##########################
    //get and set state booleans
    //##########################

    const bool &getIsValid() const;

    void setIsSolved(const bool &isSolved);
    const bool &getIsSolved() const;

    //###############
    //display methods
    //###############

    QString getDisplayId() const;
    QString getDisplayStation() const;
    QString getDisplayTargetGeometries() const;
    QString getDisplayX(const UnitType &type, const int &digits) const;
    QString getDisplayY(const UnitType &type, const int &digits) const;
    QString getDisplayZ(const UnitType &type, const int &digits) const;
    QString getDisplaySigmaX(const UnitType &type, const int &digits) const;
    QString getDisplaySigmaY(const UnitType &type, const int &digits) const;
    QString getDisplaySigmaZ(const UnitType &type, const int &digits) const;
    QString getDisplayIsValid() const;
    QString getDisplayIsSolved() const;

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

private:

    //####################################################
    //references to station, reading and target geometries
    //####################################################

    QPointer<Station> station;

    QList< QPointer<Geometry> > targetGeometriesList;
    QMap<int, QPointer<Geometry> > targetGeometriesMap;

    QPointer<Reading> reading;

    //###################################################
    //booleans that indicate the state of the observation
    //###################################################

    //bool isActiveCoordSys; //indicates wether the observation coord system is the current coordinate system at time of creation

    bool isValid; //defines if the reading can be transformed, without any function, to a xyz observation
    bool isSolved; //defines wether this observation is valid in current coordinate system

    //#############################
    //xyz coordinates and statistic
    //#############################

    OiVec xyz; //the homogeneous coordinate vector in the current display coordinate system
    OiVec originalXyz; //the homogeneous coordinate vector in the station system that produced this observations

    Statistic statistic;
    Statistic originalStatistic;

    //OiVec sigmaXyz;

    //##############
    //helper methods
    //##############

    //void calcFromReading();

};

Q_DECLARE_METATYPE( Observation* )

#endif // OBSERVATION_H
