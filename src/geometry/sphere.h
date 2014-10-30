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
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

    bool saveSimulationData();

    QString getDisplayX() const;
    QString getDisplayY() const;
    QString getDisplayZ() const;
    QString getDisplayRadius() const;
    QString getDisplayIsCommon() const;
    QString getDisplayIsNominal() const;
    //QString getDisplayObs() const;
    QString getDisplaySolved() const;
    QString getDisplayMConfig() const;
    QString getDisplayStdDev() const;
};

#endif // SPHERE_H
