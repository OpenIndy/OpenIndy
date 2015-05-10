#ifndef SPHERE_H
#define SPHERE_H

#include <QObject>
#include <QtXml>

#include "geometry.h"
#include "position.h"
#include "radius.h"
#include "direction.h"

/*!
 * \brief The Sphere class
 */
class Sphere : public Geometry
{
    Q_OBJECT

public:
    Sphere(const bool &isNominal, QObject *parent = 0);
    Sphere(const bool &isNominal, const Position &center, const Radius &radius, QObject *parent = 0);

    Sphere(const Sphere &copy, QObject *parent = 0);

    Sphere &operator=(const Sphere &copy);

    ~Sphere();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum SphereUnknowns{
        unknownCenterX = 0,
        unknownCenterY,
        unknownCenterZ,
        unknownRadius
    };

    //###################################################################
    //check wether this geometry has a direction, a position and a radius
    //###################################################################

    bool hasPosition() const;
    bool hasRadius() const;

    //############################
    //get or set sphere parameters
    //############################

    const Radius &getRadius() const;
    const Position &getPosition() const;

    void setSphere(const Position &center, const Radius &radius);

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
    QString getDisplayRadiusA(const UnitType &type, const int &digits, const bool &showDiff = false) const;

private:

    //#################
    //sphere attributes
    //#################

    Position center; //center
    Radius radius; //radius

};

#endif // SPHERE_H
