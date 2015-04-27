#ifndef P_INTERSECTPLANEPLANE_H
#define P_INTERSECTPLANEPLANE_H

#include <QObject>
#include <QPointer>

#include "constructfunction.h"
#include "oivec.h"
#include "oimat.h"

/*!
 * \brief The IntersectPlanePlane class
 */
class IntersectPlanePlane : public ConstructFunction
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

    bool exec(Line &line);

private:

    //##############
    //helper methods
    //##############

    bool setUpResult(Line &line);
    bool isIntersection(const QPointer<Plane> &plane1, const QPointer<Plane> &plane2);
    void calcPointOnLine(OiVec &p_l, const OiVec &n1, const OiVec &n2, const OiVec &n3, const OiVec &d);

};

#endif // P_INTERSECTPLANEPLANE_H
