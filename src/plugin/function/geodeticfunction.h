#ifndef PI_GEODETICFUNCTION_H
#define PI_GEODETICFUNCTION_H

#include "function.h"

/*!
 * \brief The GeodeticFunction class
 * for future use only
 */
class GeodeticFunction : public Function
{
    Q_OBJECT

public:
    GeodeticFunction(QObject *parent = 0) : Function(parent){}

    virtual ~GeodeticFunction(){}

    //####################################
    //methods that cannot be reimplemented
    //####################################

    bool exec(QPointer<Station> &station){ Function::exec(station); }
    bool exec(QPointer<CoordinateSystem> &coordinateSystem){ Function::exec(coordinateSystem); }
    bool exec(QPointer<TrafoParam> &trafoParam){ Function::exec(trafoParam); }
};

#define GeodeticFunction_iidd "de.openIndy.Plugin.Function.GeodeticFunction.v001"

#endif // PI_GEODETICFUNCTION_H
