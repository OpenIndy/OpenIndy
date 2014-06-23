#ifndef FEATUREWRAPPER_H
#define FEATUREWRAPPER_H

#include "configuration.h"
#include "feature.h"
#include "geometry.h"
#include "coordinatesystem.h"
#include "trafoparam.h"
#include "station.h"
#include "point.h"
#include "line.h"
#include "plane.h"
#include "sphere.h"
#include "circle.h"
#include "cone.h"
#include "cylinder.h"
#include "ellipsoid.h"
#include "paraboloid.h"
#include "hyperboloid.h"
#include "pointcloud.h"
#include "nurbs.h"
#include "scalarentityangle.h"
#include "scalarentitydistance.h"
#include "scalarentitytemperature.h"
#include "scalarentitymeasurementseries.h"


class FeatureWrapper
{
public:
    FeatureWrapper();

private:
    Feature *myFeature;
    Geometry *myGeometry;

    CoordinateSystem *myCoordinateSystem;
    TrafoParam *myTrafoParam;

    Station *myStation;

    Point *myPoint;
    Line *myLine;
    Plane *myPlane;
    Sphere *mySphere;
    Circle *myCircle;
    Cone *myCone;
    Cylinder *myCylinder;
    Ellipsoid *myEllipsoid;
    Hyperboloid *myHyperboloid;
    Paraboloid *myParaboloid;
    PointCloud *myPointCloud;
    Nurbs *myNurbs;
    ScalarEntityAngle *myScalarEntityAngle;
    ScalarEntityDistance *myScalarEntityDistance;
    ScalarEntityTemperature *myScalarEntityTemperature;
    ScalarEntityMeasurementSeries *myScalarEntityMeasurementSeries;

    Configuration::FeatureTypes typeOfFeature;

public:
    QString returnFeatureType() const;

//getter
    Configuration::FeatureTypes getTypeOfFeature();
    Feature* getFeature();
    Geometry* getGeometry();
    CoordinateSystem* getCoordinateSystem();
    TrafoParam* getTrafoParam();
    Station* getStation();
    Point* getPoint();
    Line* getLine();
    Plane* getPlane();
    Sphere* getSphere();
    Circle* getCircle();
    Cone* getCone();
    Cylinder* getCylinder();
    Ellipsoid* getEllipsoid();
    Hyperboloid* getHyperboloid();
    Paraboloid* getParaboloid();
    PointCloud* getPointCloud();
    Nurbs* getNurbs();
    ScalarEntityAngle* getScalarEntityAngle();
    ScalarEntityDistance* getScalarEntityDistance();
    ScalarEntityTemperature* getScalarEntityTemperature();
    ScalarEntityMeasurementSeries* getScalarEntityMeasurementSeries();

//setter
     void setCoordinateSystem(CoordinateSystem *coordSys);
     void setTrafoParam(TrafoParam *trafoPara);
     void setStation(Station *s);
     void setPoint(Point *p);
     void setLine(Line *l);
     void setPlane(Plane *p);
     void setSphere(Sphere *s);
     void setCircle(Circle *c);
     void setCone(Cone *c);
     void setCylinder(Cylinder *c);
     void setEllipsoid(Ellipsoid *e);
     void setHyperboloid(Hyperboloid *h);
     void setParaboloid(Paraboloid *p);
     void setPointCloud(PointCloud *p);
     void setNurbs(Nurbs *n);
     void setScalarEntityAngle(ScalarEntityAngle *sEA);
     void setScalarEntityDistance(ScalarEntityDistance *sED);
     void setScalarEntityTemperature(ScalarEntityTemperature *SET);
     void setScalarEntityMeasurementSeries(ScalarEntityMeasurementSeries *SEMS);
};

#endif // FEATUREWRAPPER_H
