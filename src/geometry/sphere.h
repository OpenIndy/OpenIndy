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
        unknownCenterX = eUnknownX,
        unknownCenterY = eUnknownY,
        unknownCenterZ = eUnknownZ,
        unknownRadius = eUnknownRadiusA
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

private:

    //#################
    //sphere attributes
    //#################

    Position center; //center
    Radius radius; //radius

    //bool saveSimulationData();

    //QString getDisplayX(bool showDiff) const;
    //QString getDisplayY(bool showDiff) const;
    //QString getDisplayZ(bool showDiff) const;
    //QString getDisplayRadius(bool showDiff) const;
    //QString getDisplayIsCommon() const;
    //QString getDisplayIsNominal() const;
    //QString getDisplayObs() const;
    //QString getDisplaySolved() const;
    //QString getDisplayMConfig() const;
    //QString getDisplayStdDev() const;

    //static MeasurementConfig defaultMeasurementConfig; //used as a default for this geometry type
};

#endif // SPHERE_H
