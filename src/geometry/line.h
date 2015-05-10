#ifndef LINE_H
#define LINE_H

#include <QObject>
#include <QtXml>

#include "geometry.h"
#include "position.h"
#include "direction.h"

/*!
 * \brief The Line class
 */
class Line : public Geometry
{
    Q_OBJECT
public:
    Line(const bool &isNominal, QObject *parent = 0);
    Line(const bool &isNominal, const Position &xyz, const Direction &axis, QObject *parent = 0);

    Line(const Line &copy, QObject *parent = 0);

    Line &operator=(const Line &copy);

    ~Line();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum LineUnknowns{
        unknownPositionX,
        unknownPositionY,
        unknownPositionZ,
        unknownAxisI,
        unknownAxisJ,
        unknownAxisK
    };

    //###################################################################
    //check wether this geometry has a direction, a position and a radius
    //###################################################################

    bool hasDirection() const;
    bool hasPosition() const;

    //##########################
    //get or set line parameters
    //##########################

    const Direction &getDirection() const;
    const Position &getPosition() const;

    void setLine(const Position &xyz, const Direction &axis);

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

private:

    //###############
    //line attributes
    //###############

    Position xyz; //an arbitrary point on the line
    Direction axis; //direction vector

};

#endif // LINE_H
