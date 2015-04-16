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
        unknownCenterX = eUnknownX,
        unknownCenterY = eUnknownY,
        unknownCenterZ = eUnknownZ
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

private:

    //################
    //point attributes
    //################

    Position xyz; //position


    //bool saveSimulationData();

    //QString getDisplayX(bool showDiff) const;
    //QString getDisplayY(bool showDiff) const;
    //QString getDisplayZ(bool showDiff) const;
    //QString getDisplayIsCommon() const;
    //QString getDisplayIsNominal() const;
    //QString getDisplayObs() const;
    //QString getDisplaySolved() const;
    //QString getDisplayMConfig() const;
    //QString getDisplayStdDev() const;

    //static MeasurementConfig defaultMeasurementConfig; //used as a default for this geometry type
};

#endif // POINT_H
