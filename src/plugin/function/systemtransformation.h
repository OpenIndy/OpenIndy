#ifndef SYSTEMTRANSFORMATION_H
#define SYSTEMTRANSFORMATION_H

#include "function.h"

/*!
 * \brief The SystemTransformation class
 * Functions that solves transformation parameters between two coordinate systems (station-station, part-part, alignments)
 */
class SystemTransformation : public Function
{
    Q_OBJECT

public:
    SystemTransformation(QObject *parent = 0) : Function(parent){

        //only applicable for trafo params
        this->applicableFor.clear();
        this->applicableFor.append(eTrafoParamFeature);

    }

    virtual ~SystemTransformation(){}

    //####################################
    //methods that cannot be reimplemented
    //####################################

    bool exec(QPointer<Station> &station){ Function::exec(station); }
    bool exec(QPointer<CoordinateSystem> &coordinateSystem){ Function::exec(coordinateSystem); }

    bool exec(QPointer<Circle> &circle){ Function::exec(circle); }
    bool exec(QPointer<Cone> &cone){ Function::exec(cone); }
    bool exec(QPointer<Cylinder> &cylinder){ Function::exec(cylinder); }
    bool exec(QPointer<Ellipse> &ellipse){ Function::exec(ellipse); }
    bool exec(QPointer<Ellipsoid> &ellipsoid){ Function::exec(ellipsoid); }
    bool exec(QPointer<Hyperboloid> &hyperboloid){ Function::exec(hyperboloid); }
    bool exec(QPointer<Line> &line){ Function::exec(line); }
    bool exec(QPointer<Nurbs> &nurbs){ Function::exec(nurbs); }
    bool exec(QPointer<Paraboloid> &paraboloid){ Function::exec(paraboloid); }
    bool exec(QPointer<Plane> &plane){ Function::exec(plane); }
    bool exec(QPointer<Point> &point){ Function::exec(point); }
    bool exec(QPointer<PointCloud> &pointCloud){ Function::exec(pointCloud); }
    bool exec(QPointer<ScalarEntityAngle> &angle){ Function::exec(angle); }
    bool exec(QPointer<ScalarEntityDistance> &distance){ Function::exec(distance); }
    bool exec(QPointer<ScalarEntityMeasurementSeries> &measurementSeries){ Function::exec(measurementSeries); }
    bool exec(QPointer<ScalarEntityTemperature> &temperature){ Function::exec(temperature); }
    bool exec(QPointer<SlottedHole> &slottedHole){ Function::exec(slottedHole); }
    bool exec(QPointer<Sphere> &sphere){ Function::exec(sphere); }
    bool exec(QPointer<Torus> &torus){ Function::exec(torus); }
};

#define SystemTransformation_iidd "de.openIndy.plugin.function.systemTransformation.v001"

#endif // SYSTEMTRANSFORMATION_H
