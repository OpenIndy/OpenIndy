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
        unknownPositionX = eUnknownX,
        unknownPositionY = eUnknownY,
        unknownPositionZ = eUnknownZ,
        unknownAxisI = eUnknownPrimaryI,
        unknownAxisJ = eUnknownPrimaryJ,
        unknownAxisK = eUnknownPrimaryK
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

private:

    //###############
    //line attributes
    //###############

    Position xyz; //an arbitrary point on the line
    Direction axis; //direction vector

    //bool saveSimulationData();

    //QString getDisplayX(bool showDiff) const;
    //QString getDisplayY(bool showDiff) const;
    //QString getDisplayZ(bool showDiff) const;
    //QString getDisplayI(bool showDiff) const;
    //QString getDisplayJ(bool showDiff) const;
    //QString getDisplayK(bool showDiff) const;
    //QString getDisplayIsCommon() const;
    //QString getDisplayIsNominal() const;
    //QString getDisplayObs() const;
    //QString getDisplaySolved() const;
    //QString getDisplayMConfig() const;
    //QString getDisplayStdDev() const;

    //static MeasurementConfig defaultMeasurementConfig; //used as a default for this geometry type
};

#endif // LINE_H
