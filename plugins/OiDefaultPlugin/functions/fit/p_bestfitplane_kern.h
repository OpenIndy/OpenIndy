#ifndef P_BESTFITPLANE_KERN_H
#define P_BESTFITPLANE_KERN_H

#include <QObject>
#include <QPointer>
#include <QtMath>

#include "fitfunction.h"
#include "oivec.h"
#include "oimat.h"
#include "cfitting_plane.h"

/*!
 * \brief The BestFitPlane_kern class
 */
class BestFitPlane_kern : public FitFunction
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

#endif // P_BESTFITPLANE_KERN_H
