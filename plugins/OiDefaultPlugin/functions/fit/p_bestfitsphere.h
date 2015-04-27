#ifndef P_BESTFITSPHERE_H
#define P_BESTFITSPHERE_H

#include <QObject>
#include <QPointer>
#include <QtMath>

#include "fitfunction.h"
#include "oivec.h"
#include "oimat.h"
#include "cfitting_sphere.h"

/*!
 * \brief The BestFitSphere class
 */
class BestFitSphere : public FitFunction
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

#endif // P_BESTFITSPHERE_H
