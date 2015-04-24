#ifndef SCALARENTITYDISTANCE_H
#define SCALARENTITYDISTANCE_H

#include <QObject>
#include <QtXml>

#include "geometry.h"

/*!
 * \brief The ScalarEntityDistance class
 */
class ScalarEntityDistance : public Geometry
{
    Q_OBJECT

public:
    ScalarEntityDistance(const bool &isNominal, QObject *parent = 0);
    ScalarEntityDistance(const bool &isNominal, const double &distance, QObject *parent = 0);

    ScalarEntityDistance(const ScalarEntityDistance &copy, QObject *parent = 0);

    ScalarEntityDistance &operator=(const ScalarEntityDistance &copy);

    ~ScalarEntityDistance();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum DistanceUnknowns{
        unknownDistance = eUnknownDistance
    };

    //##############################
    //get or set distance parameters
    //##############################

    const double &getDistance() const;

    void setDistance(const double &distance);

    //###########################
    //reexecute the function list
    //###########################

    void recalc();

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

    //###############
    //display methods
    //###############

    QString getDisplayDistance(const UnitType &type, const int &digits, const bool &showDiff = false) const;

private:

    //###################
    //distance attributes
    //###################

    double distance;

    //QString getDisplayIsCommon() const;
    //QString getDisplayIsNominal() const;
    //QString getDisplayObs() const;
    //QString getDisplaySolved() const;
    //QString getDisplayMConfig() const;
    //QString getDisplayStdDev() const;
    //QString getDisplayScalarAngleValue() const;

    //static MeasurementConfig defaultMeasurementConfig; //used as a default for this geometry type


};

#endif // SCALARENTITYDISTANCE_H
