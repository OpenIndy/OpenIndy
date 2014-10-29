#ifndef FUNCTION_H
#define FUNCTION_H

#include <QMap>
#include <QStringList>
#include <QtXml>

#include "configuration.h"
#include "point.h"
#include "line.h"
#include "plane.h"
#include "sphere.h"
#include "circle.h"
#include "cone.h"
#include "cylinder.h"
#include "ellipsoid.h"
#include "hyperboloid.h"
#include "nurbs.h"
#include "paraboloid.h"
#include "pointcloud.h"
#include "station.h"
#include "coordinatesystem.h"
#include "trafoparam.h"
#include "observation.h"
#include "statistic.h"
#include "scalarentitydistance.h"
#include "scalarentityangle.h"
#include "scalarentitytemperature.h"
#include "scalarentitymeasurementseries.h"
#include "reading.h"
#include "functionconfiguration.h"
#include "oifunctionemitter.h"

/*
 * TODO funktion virtual QMap <QString, QStringList>* getStringParameter() = 0;
 */

/*!
 * \brief The InputParams struct
 * structure for saving the needed features
 */
struct InputParams{
    int index; //index of the input element to get the order
    Configuration::ElementTypes typeOfElement; //type of the needed input element
    QString description; //optional description for the needed element to show in the GUI
    bool infinite; //only one element or as many features as one wants?
};

/*!
 * \brief The InputFeature struct
 * structure for saving the feature order
 */
struct InputFeature{
    int id;
    Configuration::ElementTypes typeOfElement;
    bool isUsed;
};

/*!
 * \brief The Function class
 * Father class of all funtion Types like fit function. This class is an abstract class.
 */
class Function
{
public:
    virtual ~Function(){}

    virtual bool exec(Station&);
    virtual bool exec(CoordinateSystem&);
    virtual bool exec(TrafoParam&);
    virtual bool exec(Point&);
    virtual bool exec(Line&);
    virtual bool exec(Plane&);
    virtual bool exec(Sphere&);
    virtual bool exec(Circle&);
    virtual bool exec(Cone&);
    virtual bool exec(Cylinder&);
    virtual bool exec(Ellipsoid&);
    virtual bool exec(Hyperboloid&);
    virtual bool exec(Nurbs&);
    virtual bool exec(Paraboloid&);
    virtual bool exec(PointCloud&);
    virtual bool exec(ScalarEntityAngle&);
    virtual bool exec(ScalarEntityDistance&);
    virtual bool exec(ScalarEntityTemperature&);
    virtual bool exec(ScalarEntityMeasurementSeries&);

    virtual QList<InputParams> getNeededElements() = 0;
    virtual QList<Configuration::FeatureTypes> applicableFor() = 0;
    virtual PluginMetaData* getMetaData() = 0;

    virtual QMap<QString, int> getIntegerParameter();
    virtual QMap<QString, double> getDoubleParameter();
    virtual QMap<QString, QStringList> getStringParameter();

    virtual QStringList getResultProtocol();

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;

private:
    OiFunctionEmitter myEmitter;

protected:
    int id;
    FunctionConfiguration myConfiguration;

    QMap<int, QList<InputFeature> > featureOrder;

    //TODO Zeiger const machen

    //TODO zusaetzliche Listen fuer fehlende features

    QList<Station*> stations;
    QList<CoordinateSystem*> coordSystems;
    QList<TrafoParam*> trafoParams;
    QList<Point*> points;
    QList<Line*> lines;
    QList<Plane*> planes;
    QList<Sphere*> spheres;
    QList<Circle*> circles;
    QList<Cone*> cones;
    QList<Cylinder*> cylinders;
    QList<Ellipsoid*> ellipsoids;
    QList<Paraboloid*> paraboloids;
    QList<Hyperboloid*> hyperboloids;
    QList<Nurbs*> nurbs;
    QList<PointCloud*> pointClouds;
    QList<ScalarEntityDistance*> scalarEntityDistances;
    QList<ScalarEntityAngle*> scalarEntityAngles;
    QList<Observation*> observations;
    QList<Reading*> polarReadings;
    QList<Reading*> distanceReadings;
    QList<Reading*> directionReadings;
    QList<Reading*> cartesianReadings;
    QList<ScalarEntityTemperature*> scalarEntityTemperatures;

    Statistic myStatistic;

public:
    OiFunctionEmitter& getOiEmitter();

    void setFunctionConfiguration(FunctionConfiguration config);
    FunctionConfiguration getFunctionConfiguration();

    bool isValid();
    QMap<int, QList<InputFeature> > getFeatureOrder();
    Statistic& getStatistic();
    int getId();
    virtual void clear();
    virtual void clearResults();

    void addStation(Station *s, int position);
    void removeStation(int id);
    Station* getStation(int id);
    QList<Station*> getStations();

    void addCoordSystem(CoordinateSystem *c, int position);
    void removeCoordSystem(int id);
    CoordinateSystem* getCoordinateSystem(int id);
    QList<CoordinateSystem*> getCoordinateSystems();

    void addTrafoParam(TrafoParam *t, int position);
    void removeTrafoParam(int id);
    TrafoParam* getTrafoParam(int id);
    QList<TrafoParam*> getTrafoParams();

    void addPoint(Point *p, int position);
    void removePoint(int id);
    Point* getPoint(int id);
    QList<Point*> getPoints();

    void addLine(Line *l, int position);
    void removeLine(int id);
    Line* getLine(int id);
    QList<Line*> getLines();

    void addPlane(Plane *p, int position);
    void removePlane(int id);
    Plane* getPlane(int id);
    QList<Plane*> getPlanes();

    void addSphere(Sphere *s, int position);
    void removeSphere(int id);
    Sphere* getSphere(int id);
    QList<Sphere*> getSpheres();

    void addCircle(Circle *c, int position);
    void removeCircle(int id);
    Circle* getCircle(int id);
    QList<Circle*> getCircles();

    void addCylinder(Cylinder *c, int position);
    void removeCylinder(int id);
    Cylinder* getCylinder(int id);
    QList<Cylinder*> getCylinders();

    void addCone(Cone *c, int position);
    void removeCone(int id);
    Cone* getCone(int id);
    QList<Cone*> getCones();

    void addEllipsoid(Ellipsoid *e, int position);
    void removeEllipsoid(int id);
    Ellipsoid* getEllipsoid(int id);
    QList<Ellipsoid*> getEllipsoids();

    void addParaboloid(Paraboloid *p, int position);
    void removeParaboloid(int id);
    Paraboloid* getParaboloid(int id);
    QList<Paraboloid*> getParaboloids();

    void addHyperboloid(Hyperboloid *h, int position);
    void removeHyperboloid(int id);
    Hyperboloid* getHyperboloid(int id);
    QList<Hyperboloid*> getHyperboloids();

    void addNurb(Nurbs *n, int position);
    void removeNurb(int id);
    Nurbs* getNurb(int id);
    QList<Nurbs*> getNurbs();

    void addPointCloud(PointCloud *p, int position);
    void removePointCloud(int id);
    PointCloud* getPointCloud(int id);
    QList<PointCloud*> getPointClouds();

    void addScalarEntityDistance(ScalarEntityDistance *sed, int position);
    void removeScalarEntityDistance(int id);
    ScalarEntityDistance* getScalarEntityDistance(int id);
    QList<ScalarEntityDistance*> getScalarEntityDistances();

    void addScalarEntityAngle(ScalarEntityAngle *sea, int position);
    void removeScalarEntityAngle(int id);
    ScalarEntityAngle* getScalarEntityAngle(int id);
    QList<ScalarEntityAngle*> getScalarEntityAngles();

    void addObservation(Observation *o, int position);
    void removeObservation(int id);
    Observation* getObservation(int id);
    QList<Observation*> getObservations();

    void addReadingPolar(Reading *r, int position);
    void removeReadingPolar(int id);
    Reading* getReadingPolar(int id);
    QList<Reading*> getPolarReadings();

    void addReadingDistance(Reading *r, int position);
    void removeReadingDistance(int id);
    Reading* getReadingDistance(int id);
    QList<Reading*> getDistanceReadings();

    void addReadingDirection(Reading *r, int position);
    void removeReadingDirection(int id);
    Reading* getReadingDirection(int id);
    QList<Reading*> getDirectionReadings();

    void addReadingCartesian(Reading *r, int position);
    void removeReadingCartesian(int id);
    Reading* getReadingCartesian(int id);
    QList<Reading*> getCartesianReadings();

    void addScalarEntityTemperature(ScalarEntityTemperature *SET, int position);
    void removeScalarEntityTemperature(int id);
    ScalarEntityTemperature* getScalarEntityTemperature(int id);
    QList<ScalarEntityTemperature*> getScalarEntityTemperatures();

    void removeFeature(int id);

protected:
    void setUseState(int id, bool state);
    void writeToConsole(QString message);

};

#endif // FUNCTION_H
