#ifndef P_BESTFTILINE_H
#define P_BESTFTILINE_H

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
class BestFitLine : public FitFunction
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

};

#endif // P_BESTFTILINE_H
