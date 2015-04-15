#ifndef PI_FITFUNCTION_H
#define PI_FITFUNCTION_H

#include "function.h"

/*!
 * \brief The FitFunction class
 * Function that solves geometries by fitting them using observations
 */
class FitFunction : public Function
{
    Q_OBJECT

public:
    FitFunction(QObject *parent = 0) : Function(parent){

        //needed elements are only observations
        NeededElement element;
        element.typeOfElement = eObservationElement;
        element.description = QString("Select as many observations as you want to be used to fit the target geometry.");
        element.infinite = true;
        this->neededElements.clear();
        this->neededElements.append(element);

    }

    virtual ~FitFunction(){}

    //####################################
    //methods that cannot be reimplemented
    //####################################

    bool exec(QPointer<Station> &station){ Function::exec(station); }
    bool exec(QPointer<CoordinateSystem> &coordinateSystem){ Function::exec(coordinateSystem); }
    bool exec(QPointer<TrafoParam> &trafoParam){ Function::exec(trafoParam); }
};

#define FitFunction_iidd "de.openIndy.Plugin.Function.FitFunction.v001"

#endif // PI_FITFUNCTION_H
