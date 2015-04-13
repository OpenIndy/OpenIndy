#ifndef FUNCTION_H
#define FUNCTION_H

#include <QMap>
#include <QMultiMap>
#include <QStringList>
#include <QtXml>
#include <QObject>
#include <QPointer>

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
#include "oifunctionemitter.h"
#include "pluginmetadata.h"

// \brief save the needed elements
class NeededElement{
public:
    ElementTypes typeOfElement; //type of the needed input element
    QString description; //optional description for the needed element to show in the GUI
    bool infinite; //only one element or as many features as one wants?
};

// \brief save the feature order
class InputElement{
public:
    int id;
    ElementTypes typeOfElement;
    bool isUsed;

    //custom comparison operator to define an input element by its id
    bool operator==(const InputElement &other){
        if(this->id == other.id){
            return true;
        }
        return false;
    }
};

//! \brief  save user specified non-feature function input parameters
class ScalarInputParams{
public:
    QMap<QString, double> doubleParameter;
    QMap<QString, int> intParameter;
    QMap<QString, QString> stringParameter;
    bool isValid;
};

/*!
 * \brief The Function class
 */
class Function : public QObject
{
    Q_OBJECT

public:
    virtual ~Function(){}

    //############
    //exec methods
    //############

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

    //##########################################################
    //methods to specify further information to solve a function
    //##########################################################

    virtual QList<NeededElement> getNeededElements() const = 0;
    virtual QList<FeatureTypes> applicableFor() const = 0;
    virtual PluginMetaData getMetaData() const = 0;

    virtual QMap<QString, int> getIntegerParameter() const;
    virtual QMap<QString, double> getDoubleParameter() const;
    virtual QMultiMap<QString, QString> getStringParameter() const;

    void setScalarInputParams(const ScalarInputParams &params);
    const ScalarInputParams &getScalarInputParams();

    //####################
    //get function results
    //####################

    virtual QStringList getResultProtocol() const;

    Statistic& getStatistic();

    //###############
    //general getters
    //###############

    OiFunctionEmitter& getOiEmitter();

    bool isValid();

    const QMultiMap<int, InputElement> &getFeatureOrder() const;

    const int &getId() const;

    //#############
    //clear results
    //#############

    virtual void clear();
    virtual void clearResults();

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

    //#######################
    //get and update elements
    //#######################

    void addStation(const QPointer<Station> &station, const int &position);
    void removeStation(const int &id);
    QPointer<Station> getStation(const int &id) const;
    const QMap<int, QPointer<Station> > &getStations() const;

    void addCoordSystem(const QPointer<CoordinateSystem> &coordSys, const int &position);
    void removeCoordSystem(const int &id);
    QPointer<CoordinateSystem> getCoordinateSystem(const int &id) const;
    const QMap<int, QPointer<CoordinateSystem> > &getCoordinateSystems() const;

    void addTrafoParam(const QPointer<TrafoParam> &trafoParam, const int &position);
    void removeTrafoParam(const int &id);
    QPointer<TrafoParam> getTrafoParam(const int &id) const;
    const QMap<int, QPointer<TrafoParam> > &getTrafoParams() const;

    void addPoint(const QPointer<Point> &point, const int &position);
    void removePoint(const int &id);
    QPointer<Point> getPoint(const int &id) const;
    const QMap<int, QPointer<Point> > &getPoints() const;

    void addLine(const QPointer<Line> &line, const int &position);
    void removeLine(const int &id);
    QPointer<Line> getLine(const int &id) const;
    const QMap<int, QPointer<Line> > &getLines() const;

    void addPlane(const QPointer<Plane> &plane, const int &position);
    void removePlane(const int &id);
    QPointer<Plane> getPlane(const int &id) const;
    const QMap<int, QPointer<Plane> > &getPlanes() const;

    void addSphere(const QPointer<Sphere> &sphere, const int &position);
    void removeSphere(const int &id);
    QPointer<Sphere> getSphere(const int &id) const;
    const QMap<int, QPointer<Sphere> > &getSpheres() const;

    void addCircle(const QPointer<Circle> &circle, const int &position);
    void removeCircle(const int &id);
    QPointer<Circle> getCircle(const int &id) const;
    const QMap<int, QPointer<Circle> > &getCircles() const;

    void addCylinder(const QPointer<Cylinder> &cylinder, const int &position);
    void removeCylinder(const int &id);
    QPointer<Cylinder> getCylinder(const int &id) const;
    const QMap<int, QPointer<Cylinder> > &getCylinders() const;

    void addCone(const QPointer<Cone> &cone, const int &position);
    void removeCone(const int &id);
    QPointer<Cone> getCone(const int &id) const;
    const QMap<int, QPointer<Cone> > &getCones() const;

    void addEllipsoid(const QPointer<Ellipsoid> &ellipsoid, const int &position);
    void removeEllipsoid(const int &id);
    QPointer<Ellipsoid> getEllipsoid(const int &id) const;
    const QMap<int, QPointer<Ellipsoid> > &getEllipsoids() const;

    void addParaboloid(const QPointer<Paraboloid> &paraboloid, const int &position);
    void removeParaboloid(const int &id);
    QPointer<Paraboloid> getParaboloid(const int &id) const;
    const QMap<int, QPointer<Paraboloid> > &getParaboloids() const;

    void addHyperboloid(const QPointer<Hyperboloid> &hyperboloid, const int &position);
    void removeHyperboloid(const int &id);
    QPointer<Hyperboloid> getHyperboloid(const int &id) const;
    const QMap<int, QPointer<Hyperboloid> > &getHyperboloids() const;

    void addNurb(const QPointer<Nurbs> &nurbs, const int &position);
    void removeNurb(const int &id);
    QPointer<Nurbs> getNurb(const int &id) const;
    const QMap<int, QPointer<Nurbs> > &getNurbs() const;

    void addPointCloud(const QPointer<PointCloud> &pointCloud, const int &position);
    void removePointCloud(const int &id);
    QPointer<PointCloud> getPointCloud(const int &id) const;
    const QMap<int, QPointer<PointCloud> > &getPointClouds() const;

    void addScalarEntityDistance(const QPointer<ScalarEntityDistance> &distance, const int &position);
    void removeScalarEntityDistance(const int &id);
    QPointer<ScalarEntityDistance> getScalarEntityDistance(const int &id) const;
    const QMap<int, QPointer<ScalarEntityDistance> > &getScalarEntityDistances() const;

    void addScalarEntityAngle(const QPointer<ScalarEntityAngle> &angle, const int &position);
    void removeScalarEntityAngle(const int &id);
    QPointer<ScalarEntityAngle> getScalarEntityAngle(const int &id) const;
    const QMap<int, QPointer<ScalarEntityAngle> > &getScalarEntityAngles() const;

    void addObservation(const QPointer<Observation> &observation, const int &position);
    void removeObservation(const int &id);
    QPointer<Observation> getObservation(const int &id) const;
    const QMap<int, QPointer<Observation> > &getObservations() const;

    void addReadingPolar(const QPointer<Reading> &reading, const int &position);
    void removeReadingPolar(const int &id);
    QPointer<Reading> getReadingPolar(const int &id) const;
    const QMap<int, QPointer<Reading> > &getPolarReadings() const;

    void addReadingDistance(const QPointer<Reading> &reading, const int &position);
    void removeReadingDistance(const int &id);
    QPointer<Reading> getReadingDistance(const int &id) const;
    const QMap<int, QPointer<Reading> > &getDistanceReadings() const;

    void addReadingDirection(const QPointer<Reading> &reading, const int &position);
    void removeReadingDirection(const int &id);
    QPointer<Reading> getReadingDirection(const int &id) const;
    const QMap<int, QPointer<Reading> > &getDirectionReadings() const;

    void addReadingCartesian(const QPointer<Reading> &reading, const int &position);
    void removeReadingCartesian(const int &id);
    QPointer<Reading> getReadingCartesian(const int &id) const;
    const QMap<int, QPointer<Reading> > &getCartesianReadings() const;

    void addScalarEntityTemperature(const QPointer<ScalarEntityTemperature> &temperature, const int &position);
    void removeScalarEntityTemperature(const int &id);
    QPointer<ScalarEntityTemperature> getScalarEntityTemperature(const int &id) const;
    const QMap<int, QPointer<ScalarEntityTemperature> > &getScalarEntityTemperatures() const;

    //TODO add getter for vector and xyz elements

    void addReading(const QPointer<Reading> &reading, const int &position);

    void addFeature(const QPointer<FeatureWrapper> &feature, const int &position);
    void removeFeature(const int &id);

protected:
    void setUseState(const int &id, bool state);
    void writeToConsole(QString message);

    //#######################################################
    //lists with elements that are used to solve the function
    //#######################################################

    QMap<int, QPointer<Station> > stations;
    QMap<int, QPointer<CoordinateSystem> > coordSystems;
    QMap<int, QPointer<TrafoParam> > trafoParams;
    QMap<int, QPointer<Point> > points;
    QMap<int, QPointer<Line> > lines;
    QMap<int, QPointer<Plane> > planes;
    QMap<int, QPointer<Sphere> > spheres;
    QMap<int, QPointer<Circle> > circles;
    QMap<int, QPointer<Cone> > cones;
    QMap<int, QPointer<Cylinder> > cylinders;
    QMap<int, QPointer<Ellipsoid> > ellipsoids;
    QMap<int, QPointer<Paraboloid> > paraboloids;
    QMap<int, QPointer<Hyperboloid> > hyperboloids;
    QMap<int, QPointer<Nurbs> > nurbs;
    QMap<int, QPointer<PointCloud> > pointClouds;
    QMap<int, QPointer<ScalarEntityDistance> > scalarEntityDistances;
    QMap<int, QPointer<ScalarEntityAngle> > scalarEntityAngles;
    QMap<int, QPointer<Observation> > observations;
    QMap<int, QPointer<Reading> > polarReadings;
    QMap<int, QPointer<Reading> > distanceReadings;
    QMap<int, QPointer<Reading> > directionReadings;
    QMap<int, QPointer<Reading> > cartesianReadings;
    QMap<int, QPointer<ScalarEntityTemperature> > scalarEntityTemperatures;

    //##################
    //general attributes
    //##################

    Statistic myStatistic;

    int id;
    ScalarInputParams scalarInputParams;

    QMultiMap<int, InputElement> featureOrder;

private:
    OiFunctionEmitter myEmitter;

};

#endif // FUNCTION_H
