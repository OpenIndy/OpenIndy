#ifndef GEODETICFUNCTION_H
#define GEODETICFUNCTION_H

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

#define GeodeticFunction_iidd "de.openIndy.plugin.function.geodeticFunction.v001"

#endif // GEODETICFUNCTION_H
