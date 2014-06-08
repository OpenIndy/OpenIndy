#ifndef OIEXCHANGEASCII_H
#define OIEXCHANGEASCII_H

#include <QString>
#include <QStringList>
#include <QRegExp>
#include "oiexchangeinterface.h"

/*!
 * \brief The oiExchangeASCII class
 * implements the oiExchangeInterface to import feature by ascii files
 */
class oiExchangeASCII: public oiExchangeInterface
{

public:
    oiExchangeASCII();

    bool importOiData(oiExchangeObject& data);
    bool exportOiData(oiExchangeObject& data);

    QList<Configuration::ElementTypes> getSupportedElements();

    QStringList getElementDescription(Configuration::ElementTypes);

    QList<UnitConverter::unitType> getSupportedDistanceUnits();
    QList<UnitConverter::unitType> getSupportedAngleUnits();
    QList<UnitConverter::unitType> getSupportedTemperatureUnits();

private:
    // private functions for importiing different element types
    bool importPoint(oiExchangeObject& data);
    bool importPlane(oiExchangeObject& data);
    bool importLine(oiExchangeObject& data);
    bool importSphere(oiExchangeObject& data);
};

#endif // OIEXCHANGEASCII_H
