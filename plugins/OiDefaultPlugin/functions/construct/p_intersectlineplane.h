#ifndef P_INTERSECTLINEPLANE_H
#define P_INTERSECTLINEPLANE_H

#include <QObject>
#include <QPointer>

#include "constructfunction.h"
#include "oivec.h"
#include "oimat.h"

/*!
 * \brief The IntersectLinePlane class
 */
class IntersectLinePlane : public ConstructFunction
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

private:

    //##############
    //helper methods
    //##############

    bool setUpResult(Point &point);
    bool isIntersection(const QPointer<Line> &line, const QPointer<Plane> &plane);

};

#endif // P_INTERSECTLINEPLANE_H
