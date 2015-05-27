#ifndef P_RECTIFYTOVECTOR_H
#define P_RECTIFYTOVECTOR_H

#include <QObject>
#include <QPointer>
#include <QtMath>

#include "objecttransformation.h"
#include "oivec.h"
#include "oimat.h"

/*!
 * \brief The RectifyToVector class
 */
class RectifyToVector : public ObjectTransformation
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
    bool exec(Circle &circle);

private:

    //##############
    //helper methods
    //##############

    bool setUpResult(Plane &plane);
    bool setUpResult(Circle &circle);

};

#endif // P_RECTIFYTOVECTOR_H
