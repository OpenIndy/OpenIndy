#ifndef P_TRANSLATEBYVALUE_H
#define P_TRANSLATEBYVALUE_H

#include <QObject>
#include <QPointer>

#include "objecttransformation.h"
#include "oivec.h"
#include "oimat.h"

class TranslateByValue : public ObjectTransformation
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
    bool exec(Line &line);
    bool exec(Plane &plane);
    bool exec(Sphere &sphere);

private:

    //##############
    //helper methods
    //##############

    bool setUpResult(Point &point);
    bool setUpResult(Line &line);
    bool setUpResult(Plane &plane);
    bool setUpResult(Sphere &sphere);

};

#endif // P_TRANSLATEBYVALUE_H
