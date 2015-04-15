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
#include "slottedhole.h"
#include "torus.h"
#include "ellipse.h"
#include "pluginmetadata.h"

//############################################
//helper classes to define function parameters
//############################################

/*!
 * \brief The NeededElement class
 * Save the needed element types that are necessary to solve a function
 */
class NeededElement{
public:
    ElementTypes typeOfElement; //type of the needed input element
    QString description; //optional description for the needed element to show in the GUI
    bool infinite; //only one element or as many elements as one wants?
};

/*!
 * \brief The InputElement class
 * Save the input elements that are used to solve a function
 */
class InputElement{
public:
    InputElement() : isUsed(true){}
    InputElement(const int &id) : isUsed(true), id(id){}

    //! custom comparison operator to compare input elements by their id
    bool operator==(const InputElement &other){
        if(this->id == other.id){
            return true;
        }
        return false;
    }

    int id; //the id of the element
    ElementTypes typeOfElement; //the type of the element

    //single parameters of this element that shall not be used in function calculation
    //e.g. ignore the radius of an input sphere
    QList<UnknownParameters> ignoredDestinationParams;

    //single parameters of the feature to be calculated that this element shall not have effect on
    //e.g. use a point only for a circle's plane, not the radius
    QList<UnknownParameters> ignoredTargetParams;

    bool isUsed; //true if this element is used in function calculation

    //element pointers (only valid for the specified element type)
    QPointer<Station> station;
    QPointer<CoordinateSystem> coordSystem;
    QPointer<TrafoParam> trafoParam;

    QPointer<Circle> circle;
    QPointer<Cone> cone;
    QPointer<Cylinder> cylinder;
    QPointer<Ellipse> ellipse;
    QPointer<Ellipsoid> ellipsoid;
    QPointer<Hyperboloid> hyperboloid;
    QPointer<Line> line;
    QPointer<Nurbs> nurbs;
    QPointer<Paraboloid> paraboloid;
    QPointer<Plane> plane;
    QPointer<Point> point;
    QPointer<PointCloud> pointCloud;
    QPointer<ScalarEntityAngle> scalarEntityAngle;
    QPointer<ScalarEntityDistance> scalarEntityDistance;
    QPointer<ScalarEntityMeasurementSeries> scalarEntityMeasurementSeries;
    QPointer<ScalarEntityTemperature> scalarEntityTemperature;
    QPointer<SlottedHole> slottedHole;
    QPointer<Sphere> sphere;
    QPointer<Torus> torus;

    QPointer<Geometry> geometry;

    QPointer<Observation> observation;
    QPointer<Reading> polarReading;
    QPointer<Reading> directionReading;
    QPointer<Reading> distanceReading;
    QPointer<Reading> cartesianReading;
    QPointer<Reading> temperatureReading;
    QPointer<Reading> levelReading;
    QPointer<Reading> undefinedReading;
};

/*!
 * \brief The ScalarInputParams class
 * Save user specified non-element scalar function input parameters
 */
class ScalarInputParams{
public:
    ScalarInputParams() : isValid(false){}

    QMap<QString, double> doubleParameter;
    QMap<QString, int> intParameter;
    QMap<QString, QString> stringParameter;
    bool isValid;
};

/*!
 * \brief The FixedParameter class
 * Save a value for each parameter that shall be fixed
 */
class FixedParameter{
public:
    FixedParameter() : value(0.0){}
    FixedParameter(const UnknownParameters &param) : value(0.0), parameter(param){}

    //! custom comparison operator to compare fixed parameters
    bool operator==(const FixedParameter &other){
        if(this->parameter == other.parameter){
            return true;
        }
        return false;
    }

    UnknownParameters parameter;
    double value;
};

//#####################
//function class itself
//#####################

/*!
 * \brief The Function class
 */
class Function : public QObject
{
    Q_OBJECT

public:
    Function(QObject *parent = 0);

    virtual ~Function();

    //##############################
    //function initialization method
    //##############################

    virtual void init();

    //############
    //exec methods
    //############

    virtual bool exec(QPointer<Station> &station);
    virtual bool exec(QPointer<CoordinateSystem> &coordinateSystem);
    virtual bool exec(QPointer<TrafoParam> &trafoParam);

    virtual bool exec(QPointer<Circle> &circle);
    virtual bool exec(QPointer<Cone> &cone);
    virtual bool exec(QPointer<Cylinder> &cylinder);
    virtual bool exec(QPointer<Ellipse> &ellipse);
    virtual bool exec(QPointer<Ellipsoid> &ellipsoid);
    virtual bool exec(QPointer<Hyperboloid> &hyperboloid);
    virtual bool exec(QPointer<Line> &line);
    virtual bool exec(QPointer<Nurbs> &nurbs);
    virtual bool exec(QPointer<Paraboloid> &paraboloid);
    virtual bool exec(QPointer<Plane> &plane);
    virtual bool exec(QPointer<Point> &point);
    virtual bool exec(QPointer<PointCloud> &pointCloud);
    virtual bool exec(QPointer<ScalarEntityAngle> &angle);
    virtual bool exec(QPointer<ScalarEntityDistance> &distance);
    virtual bool exec(QPointer<ScalarEntityMeasurementSeries> &measurementSeries);
    virtual bool exec(QPointer<ScalarEntityTemperature> &temperature);
    virtual bool exec(QPointer<SlottedHole> &slottedHole);
    virtual bool exec(QPointer<Sphere> &sphere);
    virtual bool exec(QPointer<Torus> &torus);

    //#############################################################
    //methods to get or set further information to solve a function
    //#############################################################

    const QList<NeededElement> &getNeededElements() const;
    const QList<FeatureTypes> &getApplicableFor() const;
    const PluginMetaData &getMetaData() const;

    const QMap<QString, int> &getIntegerParameter() const;
    const QMap<QString, double> &getDoubleParameter() const;
    const QMultiMap<QString, QString> &getStringParameter() const;

    void setScalarInputParams(const ScalarInputParams &params);
    const ScalarInputParams &getScalarInputParams() const;

    const QList<FixedParameter> &getFixedParameters() const;
    void fixParameter(const FixedParameter &parameter);
    void unfixParameter(const UnknownParameters &parameter);
    void unfixAllParameters();

    //####################
    //get function results
    //####################

    const QStringList &getResultProtocol() const;

    const Statistic &getStatistic() const;

    //###############
    //general getters
    //###############

    const int &getId() const;

    bool getIsValid() const;

    //###################
    //get or set elements
    //###################

    const QMap<int, QList<InputElement> > &getInputElements() const;
    void addInputElement(const InputElement &element, const int &position);
    void removeInputElement(const int &id, const int &position);
    void replaceInputElement(const InputElement &element, const int &position);

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

signals:

    //################################################
    //signals to inform OpenIndy about function issues
    //################################################

    void sendMessage(const QString &msg);

protected:

    //#################################
    //mark elements as used or not used
    //#################################

    void setUseState(const int &position, const int &id, const bool &state);

    //###########################
    //input and output parameters
    //###########################

    //meta information about the function (to tell OpenIndy what this function does)
    QList<NeededElement> neededElements;
    QList<FeatureTypes> applicableFor;
    PluginMetaData metaData;

    //the needed scalar input parameters including default values
    QMap<QString, int> integerParameters;
    QMap<QString, double> doubleParameters;
    QMultiMap<QString, QString> stringParameters;

    //user defined scalar input parameters
    ScalarInputParams scalarInputParams;

    //parameters of a geometry that should have a fixed value
    QList<FixedParameter> fixedParameters;

    QStringList resultProtocol;

    //##################
    //general attributes
    //##################

    int id;

    Statistic statistic;

    QMap<int, QList<InputElement> > inputElements;

};

#define Function_iid "de.openIndy.plugin.function.v001"

#endif // FUNCTION_H
