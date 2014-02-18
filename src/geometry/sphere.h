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
public:
    Sphere();
    Sphere(const Sphere &copy);
    ~Sphere();

    OiVec xyz;
    double radius;

    void recalc();

    QString getDisplayX() const;
    QString getDisplayY() const;
    QString getDisplayZ() const;
    QString getDisplayRadius() const;
    QString getDisplayIsCommon() const;
    QString getDisplayIsNominal() const;
    QString getDisplayObs() const;
    QString getDisplaySolved() const;
    QString getDisplayMConfig() const;
    QString getDisplayStdDev() const;
};

#endif // SPHERE_H
