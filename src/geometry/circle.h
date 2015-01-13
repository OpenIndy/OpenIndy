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
    double getRadius() const;

    void recalc();

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

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
    QString getDisplayI(bool showDiff) const;
    QString getDisplayJ(bool showDiff) const;
    QString getDisplayK(bool showDiff) const;

    bool saveSimulationData();

    static MeasurementConfig defaultMeasurementConfig; //used as a default for this geometry type

};

#endif // CIRCLE_H
