#ifndef SYSTEMTRANSFORMATION_H
#define SYSTEMTRANSFORMATION_H

#include "function.h"
#include <QString>

/*!
 * \brief The SystemTransformation class
 * Interface for implementing coordinate system transformation function plugins.
 */

class SystemTransformation : public Function
{

public:
    virtual ~SystemTransformation(){}

    virtual QList<InputParams> getNeededElements() = 0;

    /*!
     * \brief applicableFor
     * Only applicable for Transformation Parameter Feature
     * \return
     */
    QList<Configuration::FeatureTypes> applicableFor(){
        QList<Configuration::FeatureTypes> result;
        result.append(Configuration::eTrafoParamFeature);
        return result;
    }

    virtual PluginMetaData* getMetaData() = 0;

    QList<Station> stations_startSystem;
    QList<CoordinateSystem> coordSystems_startSystem;
    QList<TrafoParam> trafoParams_startSystem;
    QList<Point> points_startSystem;
    QList<Line> lines_startSystem;
    QList<Plane> planes_startSystem;
    QList<Sphere> spheres_startSystem;
    QList<ScalarEntityDistance> scalarEntityDistances_startSystem;
    QList<ScalarEntityAngle> scalarEntityAngles_startSystem;
    QList<Observation> observations_startSystem;
    QList<Reading> polarReadings_startSystem;
    QList<Reading> distanceReadings_startSystem;
    QList<Reading> directionReadings_startSystem;
    QList<Reading> cartesianReadings_startSystem;

    QList<Station> stations_targetSystem;
    QList<CoordinateSystem> coordSystems_targetSystem;
    QList<TrafoParam> trafoParams_targetSystem;
    QList<Point> points_targetSystem;
    QList<Line> lines_targetSystem;
    QList<Plane> planes_targetSystem;
    QList<Sphere> spheres_targetSystem;
    QList<ScalarEntityDistance> scalarEntityDistances_targetSystem;
    QList<ScalarEntityAngle> scalarEntityAngles_targetSystem;
    QList<Observation> observations_targetSystem;
    QList<Reading> polarReadings_targetSystem;
    QList<Reading> distanceReadings_targetSystem;
    QList<Reading> directionReadings_targetSystem;
    QList<Reading> cartesianReadings_targetSystem;

};

#define SystemTransformation_iidd "de.openIndy.Plugin.Function.SystemTransformation.v001"

#endif // SYSTEMTRANSFORMATION_H
