#ifndef SPHERE_H
#define SPHERE_H

#include "geometry.h"
#include "oivec.h"

/*!
 * \brief The Sphere class
 *
 */
class Sphere : public Geometry
{
    Q_OBJECT
public:
    Sphere(bool isNominal, QObject *parent = 0);
    Sphere(const Sphere &copy);
    ~Sphere();

    OiVec xyz;
    double radius;

    OiVec getXYZ() const;
    double getRadius() const;

    void recalc();

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

    bool saveSimulationData();

    QString getDisplayX(bool showDiff) const;
    QString getDisplayY(bool showDiff) const;
    QString getDisplayZ(bool showDiff) const;
    QString getDisplayRadius(bool showDiff) const;
    QString getDisplayIsCommon() const;
    //QString getDisplayIsNominal() const;
    //QString getDisplayObs() const;
    QString getDisplaySolved() const;
    QString getDisplayMConfig() const;
    QString getDisplayStdDev() const;

    static MeasurementConfig defaultMeasurementConfig; //used as a default for this geometry type
};

#endif // SPHERE_H
