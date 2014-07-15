#ifndef CIRCLE_H
#define CIRCLE_H

#include "geometry.h"

class Circle : public Geometry
{
    Q_OBJECT
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

    Circle(bool isNominal, QObject *parent = 0);
    Circle(const Circle &copy);

    OiVec xyz;
    OiVec ijk;
    double radius;
    double dist2origin;
    OiVec getXYZ() const;
    OiVec getIJK() const;

    void recalc();

    bool toOpenIndyXML(QXmlStreamWriter& stream);
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

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
    QString getDisplayI() const;
    QString getDisplayJ() const;
    QString getDisplayK() const;

    bool saveSimulationData();

};

#endif // CIRCLE_H
