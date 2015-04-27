#ifndef P_REGISTER_H
#define P_REGISTER_H

#include <QObject>
#include <QPointer>

#include "objecttransformation.h"
#include "oivec.h"
#include "oimat.h"

/*!
 * \brief The Register class
 */
class Register : public ObjectTransformation
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

    bool exec(Point &point);
    bool exec(Sphere &sphere);

private:

    //##############
    //helper methods
    //##############

    bool setUpResult(Point &point);
    bool setUpResult(Sphere &sphere);

};

#endif // P_REGISTER_H
