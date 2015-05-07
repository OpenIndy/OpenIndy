#ifndef CYLINDER_H
#define CYLINDER_H

#include <QObject>
#include <QtXml>

#include "geometry.h"
#include "position.h"
#include "radius.h"
#include "direction.h"

/*!
 * \brief The Cylinder class
 * Defines a right circular cylinder
 */
class Cylinder : public Geometry
{
    Q_OBJECT

public:
    Cylinder(const bool &isNominal, QObject *parent = 0);
    Cylinder(const bool &isNominal, const Position &axisPoint, const Direction &axis, const Radius &radius, QObject *parent = 0);

    Cylinder(const Cylinder &copy, QObject *parent = 0);

    Cylinder &operator=(const Cylinder &copy);

    ~Cylinder();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum CylinderUnknowns{
        unknownAxisPointX = 0,
        unknownAxisPointY,
        unknownAxisPointZ,
        unknownAxisI,
        unknownAxisJ,
        unknownAxisK,
        unknownRadius
    };

    //###################################################################
    //check wether this geometry has a direction, a position and a radius
    //###################################################################

    bool hasDirection() const;
    bool hasPosition() const;
    bool hasRadius() const;

    //##############################
    //get or set cylinder parameters
    //##############################

    const Radius &getRadius() const;
    const Direction &getDirection() const;
    const Position &getPosition() const;

    void setCylinder(const Position &axisPoint, const Direction &axis, const Radius &radius);

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
    QString getDisplayRadiusA(const UnitType &type, const int &digits, const bool &showDiff = false) const;

private:

    //###################
    //cylinder attributes
    //###################

    Position axisPoint; //an arbitrary point on the cylinder axis
    Direction axis; //axis vector
    Radius radius; //radius

};

#endif // CYLINDER_H
