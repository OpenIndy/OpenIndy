#ifndef ELLIPSOID_H
#define ELLIPSOID_H

#include <QObject>
#include <QtXml>

#include "geometry.h"
#include "position.h"
#include "direction.h"

/*!
 * \brief The Ellipsoid class
 * Defines an ellipsoid of rotation
 */
class Ellipsoid : public Geometry
{
    Q_OBJECT

public:
    Ellipsoid(const bool &isNominal, QObject *parent = 0);
    Ellipsoid(const bool &isNominal, const Position &center, const Direction &majorAxis,
              const double &a, const double &b, QObject *parent = 0);

    Ellipsoid(const Ellipsoid &copy, QObject *parent = 0);

    Ellipsoid &operator=(const Ellipsoid &copy);

    ~Ellipsoid();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum EllipsoidUnknowns{
        unknownCenterX = 0,
        unknownCenterY,
        unknownCenterZ,
        unknownMajorAxisI,
        unknownMajorAxisJ,
        unknownMajorAxisK,
        unknownA,
        unknownB
    };

    //###################################################################
    //check wether this geometry has a direction, a position and a radius
    //###################################################################

    bool hasDirection() const;
    bool hasPosition() const;

    //###############################
    //get or set ellipsoid parameters
    //###############################

    const Direction &getDirection() const;
    const Position &getPosition() const;

    const double &getA() const;
    const double &getB() const;

    void setEllipsoid(const Position &center, const Direction &majorAxis,
                      const double &a, const double &b);

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

    QString getDisplayX(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    QString getDisplayY(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    QString getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    QString getDisplayPrimaryI(const int &digits, const bool &showDiff = false) const;
    QString getDisplayPrimaryJ(const int &digits, const bool &showDiff = false) const;
    QString getDisplayPrimaryK(const int &digits, const bool &showDiff = false) const;
    QString getDisplayA(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    QString getDisplayB(const UnitType &type, const int &digits, const bool &showDiff = false) const;

private:

    //####################
    //ellipsoid attributes
    //####################

    Position center; //center
    Direction majorAxis; //major axis vector of the ellipsoid (semi-major axis)
    double a; //semi-major axis
    double b; //semi-minor axis


    //bool saveSimulationData();

    //static MeasurementConfig defaultMeasurementConfig; //used as a default for this geometry type

};

#endif // ELLIPSOID_H
