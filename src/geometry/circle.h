#ifndef CIRCLE_H
#define CIRCLE_H

#include <QObject>
#include <QtXml>

#include "geometry.h"
#include "position.h"
#include "radius.h"
#include "direction.h"

/*!
 * \brief The Circle class
 */
class Circle : public Geometry
{
    Q_OBJECT

public:
    Circle(const bool &isNominal, QObject *parent = 0);
    Circle(const bool &isNominal, const Position &center, const Direction &normal, const Radius &radius, QObject *parent = 0);

    Circle(const Circle &copy, QObject *parent = 0);

    Circle &operator=(const Circle &copy);

    ~Circle();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum CircleUnknowns{
        unknownCenterX = 0,
        unknownCenterY,
        unknownCenterZ,
        unknownNormalI,
        unknownNormalJ,
        unknownNormalK,
        unknownRadius
    };

    //###################################################################
    //check wether this geometry has a direction, a position and a radius
    //###################################################################

    bool hasDirection() const;
    bool hasPosition() const;
    bool hasRadius() const;

    //############################
    //get or set circle parameters
    //############################

    const Radius &getRadius() const;
    const Direction &getDirection() const;
    const Position &getPosition() const;

    void setCircle(const Position &center, const Direction &normal, const Radius &radius);

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

    //#################
    //circle attributes
    //#################

    Position center; //center
    Direction normal; //normal vector
    Radius radius; //radius

};

#endif // CIRCLE_H
