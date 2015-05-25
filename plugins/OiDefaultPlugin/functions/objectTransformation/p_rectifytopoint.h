#ifndef P_RECTIFYTOPOINT_H
#define P_RECTIFYTOPOINT_H

#include <QObject>
#include <QPointer>

#include "objecttransformation.h"
#include "oivec.h"
#include "oimat.h"

/*!
 * \brief The RectifyToPoint class
 */
class RectifyToPoint : public ObjectTransformation
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

private:

    //##############
    //helper methods
    //##############

    bool setUpResult(Plane &plane);

};

#endif // P_RECTIFYTOPOINT_H
