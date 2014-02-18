#ifndef OIEXCHANGEASCII_H
#define OIEXCHANGEASCII_H

#include "oiexchangeinterface.h"

class oiExchangeASCII: public oiExchangeInterface
{
public:
    oiExchangeASCII();

    bool importOiData(oiExchangeObject& data);
    bool exportOiData(oiExchangeObject& data);

    QList<Configuration::ElementTypes> getSupportedElements();

private:
    bool importPoint(oiExchangeObject& data);
    bool importPlane(oiExchangeObject& data);
    bool importLine(oiExchangeObject& data);
    bool importSphere(oiExchangeObject& data);
};

#endif // OIEXCHANGEASCII_H
