#ifndef POINT_H
#define POINT_H

#include "oivec.h"
#include "geometry.h"
#include <QDebug>

/*!
 * \brief The Point class
 * Specific point class with xyz vector.
 */
class Point : public Geometry
{
    Q_OBJECT
public:
    enum PointUnknowns{
        unknownX,
        unknownY,
        unknownZ
    };

    Point(bool isNominal, QObject *parent = 0);
    Point(const Point &copy);
    ~Point();

    OiVec xyz;

    OiVec getXYZ() const;

    void recalc();

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc);
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

    bool saveSimulationData();

    QString getDisplayX() const;
    QString getDisplayY() const;
    QString getDisplayZ() const;
    QString getDisplayIsCommon() const;
    QString getDisplayIsNominal() const;
    //QString getDisplayObs() const;
    QString getDisplaySolved() const;
    QString getDisplayMConfig() const;
    QString getDisplayStdDev() const;
};

#endif // POINT_H
