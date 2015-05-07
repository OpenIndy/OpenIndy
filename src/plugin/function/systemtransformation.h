#ifndef SYSTEMTRANSFORMATION_H
#define SYSTEMTRANSFORMATION_H

#include "function.h"

class FeatureUpdater;

/*!
 * \brief The SystemTransformation class
 * Functions that solves transformation parameters between two coordinate systems (station-station, part-part, alignments)
 */
class SystemTransformation : public Function
{
    friend class FeatureUpdater;
    Q_OBJECT

public:
    SystemTransformation(QObject *parent = 0) : Function(parent){

        //only applicable for trafo params
        this->applicableFor.clear();
        this->applicableFor.append(eTrafoParamFeature);

    }

    virtual ~SystemTransformation(){}

protected:

    //####################################
    //methods that cannot be reimplemented
    //####################################

    bool exec(Station &station){ return Function::exec(station); }
    bool exec(CoordinateSystem &coordinateSystem){ return Function::exec(coordinateSystem); }

    bool exec(Circle &circle){ return Function::exec(circle); }
    bool exec(Cone &cone){ return Function::exec(cone); }
    bool exec(Cylinder &cylinder){ return Function::exec(cylinder); }
    bool exec(Ellipse &ellipse){ return Function::exec(ellipse); }
    bool exec(Ellipsoid &ellipsoid){ return Function::exec(ellipsoid); }
    bool exec(Hyperboloid &hyperboloid){ return Function::exec(hyperboloid); }
    bool exec(Line &line){ return Function::exec(line); }
    bool exec(Nurbs &nurbs){ return Function::exec(nurbs); }
    bool exec(Paraboloid &paraboloid){ return Function::exec(paraboloid); }
    bool exec(Plane &plane){ return Function::exec(plane); }
    bool exec(Point &point){ return Function::exec(point); }
    bool exec(PointCloud &pointCloud){ return Function::exec(pointCloud); }
    bool exec(ScalarEntityAngle &angle){ return Function::exec(angle); }
    bool exec(ScalarEntityDistance &distance){ return Function::exec(distance); }
    bool exec(ScalarEntityMeasurementSeries &measurementSeries){ return Function::exec(measurementSeries); }
    bool exec(ScalarEntityTemperature &temperature){ return Function::exec(temperature); }
    bool exec(SlottedHole &slottedHole){ return Function::exec(slottedHole); }
    bool exec(Sphere &sphere){ return Function::exec(sphere); }
    bool exec(Torus &torus){ return Function::exec(torus); }

    //special attributes for system transformations (normal transformations)
    QList<Point> inputPointsStartSystem; //input elements solved in start system
    QList<Point> inputPointsDestinationSystem; //input elements solved in destination system

    //special attributes for system transformations (alignments)
    QMap<int, QList<InputElement> > inputElementsStartSystem; //input elements solved in start system
    QMap<int, QList<InputElement> > inputElementsDestinationSystem; //input elements solved in destination system

};

#define SystemTransformation_iidd "de.openIndy.plugin.function.systemTransformation.v001"

#endif // SYSTEMTRANSFORMATION_H
