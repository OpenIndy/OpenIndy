#ifndef HYPERBOLOID_H
#define HYPERBOLOID_H

#include <QObject>
#include <QtXml>

#include "geometry.h"
#include "position.h"
#include "direction.h"

/*!
 * \brief The Hyperboloid class
 * Defines a single shell hyperboloid of rotation of the form 1 = x^2/a^2 + y^2/b^2 - z^2/c^2, a=b>0, c>0
 */
class Hyperboloid : public Geometry
{
    Q_OBJECT

public:
    Hyperboloid(const bool &isNominal, QObject *parent = 0);
    Hyperboloid(const bool &isNominal, const Position &center, const Direction &axis, const double &a, const double &c, QObject *parent = 0);

    Hyperboloid(const Hyperboloid &copy, QObject *parent = 0);

    Hyperboloid &operator=(const Hyperboloid &copy);

    ~Hyperboloid();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum HyperboloidUnknowns{
        unknownCenterX = 0,
        unknownCenterY,
        unknownCenterZ,
        unknownAxisI,
        unknownAxisJ,
        unknownAxisK,
        unknownA,
        unknownC
    };

    //###################################################################
    //check wether this geometry has a direction, a position and a radius
    //###################################################################

    bool hasDirection() const;
    bool hasPosition() const;

    //#################################
    //get or set hyperboloid parameters
    //#################################

    const Direction &getDirection() const;
    const Position &getPosition() const;

    const double &getA() const;
    const double &getC() const;

    void setHyperboloid(const Position &center, const Direction &axis, const double &a, const double &c);

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
    QString getDisplayC(const UnitType &type, const int &digits, const bool &showDiff = false) const;

    //bool saveSimulationData();

    //static MeasurementConfig defaultMeasurementConfig; //used as a default for this geometry type

private:

    //######################
    //hyperboloid attributes
    //######################

    Position center; //center
    Direction axis; //the rotation axis
    double a;
    double c;

};

#endif // HYPERBOLOID_H
