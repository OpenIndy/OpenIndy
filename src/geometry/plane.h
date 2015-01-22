#ifndef PLANE_H
#define PLANE_H

#include "oivec.h"
#include "geometry.h"

/*!
 * \brief The Plane class
 * Specific class of plane with xyz and ijk vector.
 */

class Plane : public Geometry
{
    Q_OBJECT
public:
    enum PlaneUnknowns{
        unknownX,
        unknownY,
        unknownZ,
        unknownI,
        unknownJ,
        unknownK
    };

    Plane(bool isNominal, QObject *parent = 0);
    Plane(const Plane &copy);
    ~Plane();

    OiVec xyz;
    OiVec ijk;

    OiVec getXYZ() const;
    OiVec getIJK() const;

    void recalc();

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

    bool saveSimulationData();

    QString getDisplayX(bool showDiff) const;
    QString getDisplayY(bool showDiff) const;
    QString getDisplayZ(bool showDiff) const;
    QString getDisplayI(bool showDiff) const;
    QString getDisplayJ(bool showDiff) const;
    QString getDisplayK(bool showDiff) const;
    QString getDisplayIsCommon() const;
    //QString getDisplayIsNominal() const;
    //QString getDisplayObs() const;
    QString getDisplaySolved() const;
    QString getDisplayMConfig() const;
    QString getDisplayStdDev() const;

    static MeasurementConfig defaultMeasurementConfig; //used as a default for this geometry type
};

#endif // PLANE_H
