#ifndef CONSTRUCTFUNCTION_H
#define CONSTRUCTFUNCTION_H

#include "function.h"

/*!
 * \brief The ConstructFunction class
 * Function that solves geometries by constructing them from other elements
 */
class ConstructFunction : public Function
{
    Q_OBJECT

public:
    ConstructFunction(QObject *parent = 0) : Function(parent){}

    virtual ~ConstructFunction(){}

protected:

    //####################################
    //methods that cannot be reimplemented
    //####################################

    bool exec(Station &station){ return Function::exec(station); }
    bool exec(CoordinateSystem &coordinateSystem){ return Function::exec(coordinateSystem); }
    bool exec(TrafoParam &trafoParam){ return Function::exec(trafoParam); }

};

#define ConstructFunction_iidd "de.openIndy.plugin.function.constructFunction.v001"


#endif // CONSTRUCTFUNCTION_H
