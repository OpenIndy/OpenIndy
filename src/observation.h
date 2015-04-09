#ifndef OBSERVATION_H
#define OBSERVATION_H

#include <QList>
#include <math.h>
#include <QPointer>

#include "element.h"
#include "geometry.h"
#include "oivec.h"
#include "statistic.h"

class Geometry;
class Reading;
class Station;

class Observation : public Element
{
public:
    //Observation(Reading*, Station*, bool isActiveCoordSys);
    Observation();
    ~Observation();

    //#########################################
    //get and set XYZ coordinates and statistic
    //#########################################

    const OiVec &getXYZ() const;

    const OiVec &getOriginalXYZ() const;

    const Statistic &getStatistic() const;

    const Statistic &getOriginalStatistic() const;

    const OiVec &getSigmaXYZ() const;

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
    void setReading(const QPointer<Reading> &reading);

    //##########################
    //get and set state booleans
    //##########################

    void setIsValid(const bool &isValid);
    bool getIsValid() const;

    void setIsSolved(const bool &isSolved);
    bool getIsSolved() const;

    void setIsUsed(const bool &use);
    bool getIsUsed() const;

    bool getUseState();

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

private:

    //####################################################
    //references to station, reading and target geometries
    //####################################################

    QPointer<Station> myStation;
    QList< QPointer<Geometry> > myTargetGeometries;
    QPointer<Reading> myReading;

    //###################################################
    //booleans that indicate the state of the observation
    //###################################################

    //TODO maybe some of these values are not necessary

    bool isActiveCoordSys; //indicates wether the observation coord system is the current coordinate system at time of creation

    bool isValid; //defines if the reading can be transformed, without any function, to a xyz observation
    bool isSolved; //defines wether this observation is valid in current coordinate system
    bool isUsed; //defines if the observation is and should be used for functions

    //#############################
    //xyz coordinates and statistic
    //#############################

    //TODO maybe sigmaXyz is not necessary

    OiVec myXyz;
    OiVec myOriginalXyz;

    Statistic myStatistic;
    Statistic myOriginalStatistic;

    OiVec sigmaXyz;

    //##############
    //helper methods
    //##############

    void calcFromReading();

};

#endif // OBSERVATION_H
