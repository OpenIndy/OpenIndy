#ifndef P_BESTFITPLANE2_H
#define P_BESTFITPLANE2_H

#include <QObject>
#include <QPointer>
#include <QtMath>

#include "fitfunction.h"
#include "oivec.h"
#include "oimat.h"
#include "cfitting_plane.h"

/*!
 * \brief The BestFitPlane2 class
 */
class BestFitPlane2 : public FitFunction
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

    bool exec(Plane &plane);

};

#endif // P_KERNFITPLANE_H
