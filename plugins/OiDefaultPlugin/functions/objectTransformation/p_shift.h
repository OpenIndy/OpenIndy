#ifndef P_SHIFTPLANE_H
#define P_SHIFTPLANE_H

#include <QList>
#include <QString>

#include "objecttransformation.h"
#include "oivec.h"
#include "oimat.h"

class Shift : public ObjectTransformation
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

#endif // P_SHIFTPLANE_H
