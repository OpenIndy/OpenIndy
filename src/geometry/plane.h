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

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc);
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

    bool saveSimulationData();

    QString getDisplayX() const;
    QString getDisplayY() const;
    QString getDisplayZ() const;
    QString getDisplayI() const;
    QString getDisplayJ() const;
    QString getDisplayK() const;
    QString getDisplayIsCommon() const;
    QString getDisplayIsNominal() const;
    //QString getDisplayObs() const;
    QString getDisplaySolved() const;
    QString getDisplayMConfig() const;
    QString getDisplayStdDev() const;
};

#endif // PLANE_H
