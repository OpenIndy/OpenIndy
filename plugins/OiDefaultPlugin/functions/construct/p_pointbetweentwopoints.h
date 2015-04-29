#ifndef P_POINTBETWEENTWOPOINTS_H
#define P_POINTBETWEENTWOPOINTS_H

#include <QObject>
#include <QPointer>

#include "constructfunction.h"
#include "oivec.h"
#include "oimat.h"

/*!
 * \brief The PointBetweenTwoPoints class
 */
class PointBetweenTwoPoints : public ConstructFunction
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

#endif // P_POINTBETWEENTWOPOINTS_H
