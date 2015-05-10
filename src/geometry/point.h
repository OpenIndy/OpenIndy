#ifndef POINT_H
#define POINT_H

#include <QObject>
#include <QtXml>

#include "geometry.h"
#include "position.h"

/*!
 * \brief The Point class
 */
class Point : public Geometry
{
    friend class Station;
    Q_OBJECT

public:
    Point(const bool &isNominal, QObject *parent = 0);
    Point(const bool &isNominal, const Position &xyz, QObject *parent = 0);

    Point(const Point &copy, QObject *parent = 0);

    Point &operator=(const Point &copy);

    ~Point();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum PointUnknowns{
        unknownCenterX = 0,
        unknownCenterY,
        unknownCenterZ
    };

    //###################################################################
    //check wether this geometry has a direction, a position and a radius
    //###################################################################

    bool hasPosition() const;

    //###########################
    //get or set point parameters
    //###########################

    const Position &getPosition() const;

    void setPoint(const Position &xyz);

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

private:

    //################
    //point attributes
    //################

    Position xyz; //position

};

#endif // POINT_H
