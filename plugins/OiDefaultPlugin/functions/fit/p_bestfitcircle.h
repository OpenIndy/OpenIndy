#ifndef P_BESTFITCIRCLE_H
#define P_BESTFITCIRCLE_H

#include "fitfunction.h"
#include "oivec.h"
#include "oimat.h"
#include "cfitting_plane.h"
#include "cfitting_sphere.h"

/*!
 * \brief The BestFitCircle class
 */
class BestFitCircle : public FitFunction
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

    bool exec(Circle &circle);

private:

    OiVec getApproximatedPlane(int nop, double x[], double y[], double z[], double cx, double cy, double cz);
};

#endif // P_BESTFITCIRCLE_H
