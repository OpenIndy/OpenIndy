#ifndef SORTLISTBYNAME_H
#define SORTLISTBYNAME_H

#include <QMap>
#include <QPointer>

#include "point.h"
#include "line.h"
#include "plane.h"
#include "sphere.h"
#include "scalarentitydistance.h"
#include "scalarentityangle.h"

struct LocRefBool{
    bool loc;
    bool ref;
    int locIndex;
    int refIndex;
};

/*!
 * \brief The SortListByName class
 * Helper class that sorts input features of trafo params by name
 */
class SortListByName
{
public:
    SortListByName();

    void addRefPoint(Point p);
    void addLocPoint(Point p);
    QList<Point> getRefPoints();
    QList<Point> getLocPoints();
    void addRefLine(Line l);
    void addLocLine(Line l);
    QList<Line> getRefLines();
    QList<Line> getLocLines();
    void addRefPlane(Plane p);
    void addLocPlane(Plane p);
    QList<Plane> getRefPlanes();
    QList<Plane> getLocPlanes();
    void addRefSphere(Sphere s);
    void addLocSphere(Sphere s);
    QList<Sphere> getRefSpheres();
    QList<Sphere> getLocSpheres();
    void addRefScalarEntityDistance(ScalarEntityDistance s);
    void addLocScalarEntityDistance(ScalarEntityDistance s);
    QList<ScalarEntityDistance> getRefScalarEntityDistances();
    QList<ScalarEntityDistance> getLocScalarEntityDistances();
    void addRefScalarEntityAngle(ScalarEntityAngle s);
    void addLocScalarEntityAngle(ScalarEntityAngle s);
    QList<ScalarEntityAngle> getRefScalarEntityAngles();
    QList<ScalarEntityAngle> getLocScalarEntityAngles();

private:
    QList<Point> refPoints;
    QList<Point> locPoints;
    QMap<QString, LocRefBool> point;
    QList<Line> refLines;
    QList<Line> locLines;
    QMap<QString, LocRefBool> line;
    QList<Plane> refPlanes;
    QList<Plane> locPlanes;
    QMap<QString, LocRefBool> plane;
    QList<Sphere> refSpheres;
    QList<Sphere> locSpheres;
    QMap<QString, LocRefBool> sphere;
    QList<ScalarEntityDistance> refScalarEntityDistances;
    QList<ScalarEntityDistance> locScalarEntityDistances;
    QMap<QString, LocRefBool> scalarEntityDistance;
    QList<ScalarEntityAngle> refScalarEntityAngles;
    QList<ScalarEntityAngle> locScalarEntityAngles;
    QMap<QString, LocRefBool> scalarEntityAngle;
};

#endif // SORTLISTBYNAME_H
