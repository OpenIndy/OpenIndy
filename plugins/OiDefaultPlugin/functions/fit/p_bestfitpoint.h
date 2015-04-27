#ifndef P_BESTFITPOINT_H
#define P_BESTFITPOINT_H

#include <QObject>
#include <QPointer>
#include <QtMath>

#include "fitfunction.h"
#include "oivec.h"
#include "oimat.h"

using namespace std;

/*!
 * \brief The BestFitPoint class
 */
class BestFitPoint : public FitFunction
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

};

#endif // P_BESTFITPOINT_H
