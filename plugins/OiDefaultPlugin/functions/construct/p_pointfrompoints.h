#ifndef P_POINTFROMPOINTS_H
#define P_POINTFROMPOINTS_H

#include <QObject>
#include <QPointer>
#include <QtMath>

#include "constructfunction.h"
#include "oivec.h"
#include "oimat.h"

using namespace std;

/*!
 * \brief The PointFromPoints class
 */
class PointFromPoints : public ConstructFunction
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

    bool exec(Point &point);

private:

    //##############
    //helper methods
    //##############

    bool setUpResult(Point &point);

};

#endif // P_POINTFROMPOINTS_H
