#ifndef P_SPHEREFROMPOINTS_H
#define P_SPHEREFROMPOINTS_H

#include <QObject>
#include <QPointer>
#include <QtMath>

#include "constructfunction.h"
#include "oivec.h"
#include "oimat.h"
#include "cfitting_sphere.h"

/*!
 * \brief The SphereFromPoints class
 */
class SphereFromPoints : public ConstructFunction
{ 
    Q_OBJECT

public:

    //##############################
    //function initialization method
    //##############################

    void init();

protected:

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

#endif // P_SPHEREFROMPOINTS_H
