#ifndef P_BESTFTICIRCLE2_H
#define P_BESTFTICIRCLE2_H

#include <QObject>
#include <QPointer>
#include <QtMath>

#include "fitfunction.h"
#include "oivec.h"
#include "oimat.h"

using namespace std;

/*!
 * \brief The BestFitLine class
 */
class BestFitCircle2 : public FitFunction
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

    bool exec(Circle &circle);

private:

    //##############
    //helper methods
    //##############

    bool setUpResult(Circle &circle);

};

#endif // P_BESTFTICIRCLE2_H
