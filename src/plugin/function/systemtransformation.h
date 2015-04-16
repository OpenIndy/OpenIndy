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

protected:

    //####################################
    //methods that cannot be reimplemented
    //####################################

    bool exec(Station &station){ Function::exec(station); }
    bool exec(CoordinateSystem &coordinateSystem){ Function::exec(coordinateSystem); }

    bool exec(Circle &circle){ Function::exec(circle); }
    bool exec(Cone &cone){ Function::exec(cone); }
    bool exec(Cylinder &cylinder){ Function::exec(cylinder); }
    bool exec(Ellipse &ellipse){ Function::exec(ellipse); }
    bool exec(Ellipsoid &ellipsoid){ Function::exec(ellipsoid); }
    bool exec(Hyperboloid &hyperboloid){ Function::exec(hyperboloid); }
    bool exec(Line &line){ Function::exec(line); }
    bool exec(Nurbs &nurbs){ Function::exec(nurbs); }
    bool exec(Paraboloid &paraboloid){ Function::exec(paraboloid); }
    bool exec(Plane &plane){ Function::exec(plane); }
    bool exec(Point &point){ Function::exec(point); }
    bool exec(PointCloud &pointCloud){ Function::exec(pointCloud); }
    bool exec(ScalarEntityAngle &angle){ Function::exec(angle); }
    bool exec(ScalarEntityDistance &distance){ Function::exec(distance); }
    bool exec(ScalarEntityMeasurementSeries &measurementSeries){ Function::exec(measurementSeries); }
    bool exec(ScalarEntityTemperature &temperature){ Function::exec(temperature); }
    bool exec(SlottedHole &slottedHole){ Function::exec(slottedHole); }
    bool exec(Sphere &sphere){ Function::exec(sphere); }
    bool exec(Torus &torus){ Function::exec(torus); }
};

#define SystemTransformation_iidd "de.openIndy.plugin.function.systemTransformation.v001"

#endif // SYSTEMTRANSFORMATION_H
