#ifndef FEATUREWRAPPER_H
#define FEATUREWRAPPER_H

#include <QObject>
#include <QPointer>

#include "types.h"
#include "util.h"
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
#include "ellipse.h"
#include "slottedhole.h"
#include "torus.h"
#include "scalarentityangle.h"
#include "scalarentitydistance.h"
#include "scalarentitytemperature.h"
#include "scalarentitymeasurementseries.h"

class FeatureWrapper : public QObject
{
    Q_OBJECT

public:
    explicit FeatureWrapper(QObject *parent = 0);

    bool operator==(const FeatureWrapper &other);

    //################
    //type information
    //################

    QString getFeatureTypeString() const;
    const FeatureTypes &getFeatureTypeEnum() const;

    //###################
    //get feature pointer
    //###################

    const QPointer<Feature> &getFeature() const;
    const QPointer<Geometry> &getGeometry() const;

    const QPointer<CoordinateSystem> &getCoordinateSystem() const;
    const QPointer<TrafoParam> &getTrafoParam() const;
    const QPointer<Station> &getStation() const;

    const QPointer<Point> &getPoint() const;
    const QPointer<Line> &getLine() const;
    const QPointer<Plane> &getPlane() const;
    const QPointer<Sphere> &getSphere() const;
    const QPointer<Circle> &getCircle() const;
    const QPointer<Cone> &getCone() const;
    const QPointer<Cylinder> &getCylinder() const;
    const QPointer<Ellipsoid> &getEllipsoid() const;
    const QPointer<Hyperboloid> &getHyperboloid() const;
    const QPointer<Paraboloid> &getParaboloid() const;
    const QPointer<PointCloud> &getPointCloud() const;
    const QPointer<Nurbs> &getNurbs() const;
    const QPointer<Ellipse> &getEllipse() const;
    const QPointer<SlottedHole> &getSlottedHole() const;
    const QPointer<Torus> &getTorus() const;

    const QPointer<ScalarEntityAngle> &getScalarEntityAngle() const;
    const QPointer<ScalarEntityDistance> &getScalarEntityDistance() const;
    const QPointer<ScalarEntityTemperature> &getScalarEntityTemperature() const;
    const QPointer<ScalarEntityMeasurementSeries> &getScalarEntityMeasurementSeries() const;

    //###################
    //set feature pointer
    //###################

    void setCoordinateSystem(const QPointer<CoordinateSystem> &coordSystem);
    void setTrafoParam(const QPointer<TrafoParam> &trafoParam);
    void setStation(const QPointer<Station> &station);

    void setPoint(const QPointer<Point> &point);
    void setLine(const QPointer<Line> &line);
    void setPlane(const QPointer<Plane> &plane);
    void setSphere(const QPointer<Sphere> &sphere);
    void setCircle(const QPointer<Circle> &circle);
    void setCone(const QPointer<Cone> &cone);
    void setCylinder(const QPointer<Cylinder> &cylinder);
    void setEllipsoid(const QPointer<Ellipsoid> &ellipsoid);
    void setHyperboloid(const QPointer<Hyperboloid> &hyperboloid);
    void setParaboloid(const QPointer<Paraboloid> &paraboloid);
    void setPointCloud(const QPointer<PointCloud> &pointCloud);
    void setNurbs(const QPointer<Nurbs> &nurbs);
    void setEllipse(const QPointer<Ellipse> &ellipse);
    void setSlottedHole(const QPointer<SlottedHole> &slottedHole);
    void setTorus(const QPointer<Torus> &torus);

    void setScalarEntityAngle(const QPointer<ScalarEntityAngle> &angle);
    void setScalarEntityDistance(const QPointer<ScalarEntityDistance> &distance);
    void setScalarEntityTemperature(const QPointer<ScalarEntityTemperature> &temperature);
    void setScalarEntityMeasurementSeries(const QPointer<ScalarEntityMeasurementSeries> &measurementSeries);

private:

    //#################################################################
    //private feature pointer that hold a feature or are set to invalid
    //#################################################################

    QPointer<Feature> feature;
    QPointer<Geometry> geometry;

    QPointer<CoordinateSystem> coordinateSystem;
    QPointer<TrafoParam> trafoParam;

    QPointer<Station> station;

    QPointer<Point> point;
    QPointer<Line> line;
    QPointer<Plane> plane;
    QPointer<Sphere> sphere;
    QPointer<Circle> circle;
    QPointer<Cone> cone;
    QPointer<Cylinder> cylinder;
    QPointer<Ellipsoid> ellipsoid;
    QPointer<Hyperboloid> hyperboloid;
    QPointer<Paraboloid> paraboloid;
    QPointer<PointCloud> pointCloud;
    QPointer<Nurbs> nurbs;
    QPointer<Ellipse> ellipse;
    QPointer<SlottedHole> slottedHole;
    QPointer<Torus> torus;
    QPointer<ScalarEntityAngle> angle;
    QPointer<ScalarEntityDistance> distance;
    QPointer<ScalarEntityTemperature> temperature;
    QPointer<ScalarEntityMeasurementSeries> measurementSeries;

    FeatureTypes typeOfFeature;

};

Q_DECLARE_METATYPE( FeatureWrapper* )

#endif // FEATUREWRAPPER_H
