#ifndef TORUS_H
#define TORUS_H

#include <QObject>
#include <QtXml>

#include "geometry.h"
#include "position.h"
#include "radius.h"
#include "direction.h"

/*!
 * \brief The Torus class
 * Defines a torus of rotation
 */
class Torus : public Geometry
{
    Q_OBJECT

public:
    Torus(const bool &isNominal, QObject *parent = 0);
    Torus(const bool &isNominal, const Position &center, const Direction &normal, const Radius &radiusA, const Radius &radiusB, QObject *parent = 0);

    Torus(const Torus &copy, QObject *parent = 0);

    Torus &operator=(const Torus &copy);

    ~Torus();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum TorusUnknowns{
        unknownCenterX = 0,
        unknownCenterY,
        unknownCenterZ,
        unknownNormalI,
        unknownNormalJ,
        unknownNormalK,
        unknownRadiusA,
        unknownRadiusB
    };

    //###################################################################
    //check wether this geometry has a direction, a position and a radius
    //###################################################################

    bool hasDirection() const;
    bool hasPosition() const;
    bool hasRadius() const;

    //###########################
    //get or set torus parameters
    //###########################

    const Radius &getRadius() const;
    const Direction &getDirection() const;
    const Position &getPosition() const;

    const Radius &getSmallRadius() const;

    void setTorus(const Position &center, const Direction &normal, const Radius &radiusA, const Radius &radiusB);

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
    QString getDisplayRadiusB(const UnitType &type, const int &digits, const bool &showDiff = false) const;

private:

    //################
    //torus attributes
    //################

    Position center; //center of the torus
    Direction normal; //normal vector
    Radius radiusA; //distance of the center to the center curve of the torus
    Radius radiusB; //distance of the center curve to the torus surface (radiusA > radiusB)

};

#endif // TORUS_H
