#ifndef P_CHANGERADIUS_H
#define P_CHANGERADIUS_H

#include <QObject>
#include <QPointer>

#include "objecttransformation.h"
#include "oivec.h"
#include "oimat.h"

/*!
 * \brief The ChangeRadius class
 */
class ChangeRadius : public ObjectTransformation
{  
    Q_OBJECT
protected:

    //##############################
    //function initialization method
    //##############################

    void init();

    //############
    //exec methods
    //############

    bool exec(Sphere &sphere);

private:

    //##############
    //helper methods
    //##############

    bool setUpResult(Sphere &sphere);

};

#endif // P_CHANGERADIUS_H
