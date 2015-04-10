#ifndef PARABOLOID_H
#define PARABOLOID_H

#include <QObject>
#include <QtXml>

#include "geometry.h"
#include "position.h"
#include "direction.h"

/*!
 * \brief The EllipticParaboloid class
 * Defines an elliptic paraboloid of rotation of the form z = x^2/a^2 + y^2/a^2, a > 0
 */
class Paraboloid : public Geometry
{
    Q_OBJECT

public:
    Paraboloid(const bool &isNominal, QObject *parent = 0);
    Paraboloid(const bool &isNominal, const Position &apex, const Direction &axis, const double &a, QObject *parent = 0);

    Paraboloid(const Paraboloid &copy, QObject *parent = 0);

    Paraboloid &operator=(const Paraboloid &copy);

    ~Paraboloid();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum ParaboloidUnknowns{
        unknownApexX,
        unknownApexY,
        unknownApexZ,
        unknownAxisI,
        unknownAxisJ,
        unknownAxisK,
        unknownA
    };

    //###################################################################
    //check wether this geometry has a direction, a position and a radius
    //###################################################################

    bool hasDirection() const;
    bool hasPosition() const;

    //################################
    //get or set paraboloid parameters
    //################################

    const Direction &getDirection() const;
    const Position &getPosition() const;

    const double &getA() const;

    void setParaboloid(const Position &apex, const Direction &axis, const double &a);

    //###########################
    //reexecute the function list
    //###########################

    void recalc();

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

private:

    //#####################
    //paraboloid attributes
    //#####################

    Position apex; //the peak of the paraboloid
    Direction axis; //the axis vector that points from the apex inside the paraboloid (rotation axis)
    double a; //compression or streatch parameter

    //bool saveSimulationData();

    //static MeasurementConfig defaultMeasurementConfig; //used as a default for this geometry type

};

#endif // PARABOLOID_H
