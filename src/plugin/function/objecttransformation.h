#ifndef OBJECTTRANSFORMATION_H
#define OBJECTTRANSFORMATION_H

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

protected:

    //####################################
    //methods that cannot be reimplemented
    //####################################

    bool exec(Station &station){ return Function::exec(station); }
    bool exec(CoordinateSystem &coordinateSystem){ return Function::exec(coordinateSystem); }
};

#define ObjectTransformation_iidd "de.openIndy.plugin.function.objectTransformation.v001"

#endif // OBJECTTRANSFORMATION_H
