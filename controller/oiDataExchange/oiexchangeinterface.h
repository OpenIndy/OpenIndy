#ifndef OIEXCHANGEINTERFACE_H
#define OIEXCHANGEINTERFACE_H

#include "oiexchangeobject.h"


class oiExchangeInterface
{

public:
    virtual ~oiExchangeInterface(){}

    virtual bool importOiData(oiExchangeObject &projectData) = 0;
    virtual bool exportOiData(oiExchangeObject &projectData) = 0;

    virtual QList<Configuration::ElementTypes> getSupportedElements() = 0;

};


#endif // OIEXCHANGEINTERFACE_H
