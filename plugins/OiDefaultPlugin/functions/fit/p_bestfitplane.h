#ifndef P_BESTFITPLANE_H
#define P_BESTFITPLANE_H

#include <QObject>
#include <QPointer>
#include <QtMath>

#include "fitfunction.h"
#include "oivec.h"
#include "oimat.h"

/*!
 * \brief The BestFitPlane class
 */
class BestFitPlane : public FitFunction
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

    bool exec(Plane &plane);

private:

    //##############
    //helper methods
    //##############

    bool setUpResult(Plane &plane);

};

#endif // P_BESTFITPLANE_H
