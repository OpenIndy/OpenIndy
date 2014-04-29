#ifndef CIRCLE_H
#define CIRCLE_H

#include "geometry.h"

class Circle : public Geometry
{
public:
    enum CircleUnknowns{
        unknownX,
        unknownY,
        unknownZ,
        unknownI,
        unknownJ,
        unknownK,
        unknownR,
        unknownD
    };

    Circle();
    Circle(const Circle &copy);

    OiVec xyz;
    OiVec ijk;
    double radius;
    double dist2origin;

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
    QString getDisplayI() const;
    QString getDisplayJ() const;
    QString getDisplayK() const;

};

#endif // CIRCLE_H
