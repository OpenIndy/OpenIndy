#ifndef PI_OBJECTTRANSFORMATION_H
#define PI_OBJECTTRANSFORMATION_H

#include "function.h"

/*!
 * \brief The ObjectTransformation class
 * Function that changes a previously solved feature by using other elements
 */
class ObjectTransformation : public Function
{
    Q_OBJECT

public:
    ObjectTransformation(QObject *parent = 0) : Function(parent){}

    virtual ~ObjectTransformation(){}

    //####################################
    //methods that cannot be reimplemented
    //####################################

    bool exec(QPointer<Station> &station){ Function::exec(station); }
    bool exec(QPointer<CoordinateSystem> &coordinateSystem){ Function::exec(coordinateSystem); }
};

#define ObjectTransformation_iidd "de.openIndy.Plugin.Function.ObjectTransformation.v001"

#endif // PI_OBJECTTRANSFORMATION_H
