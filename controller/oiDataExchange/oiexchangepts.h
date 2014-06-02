#ifndef OIEXCHANGEPTS_H
#define OIEXCHANGEPTS_H

#include <QString>
#include <QStringList>
#include <QRegExp>
#include "oiexchangeinterface.h"

class oiExchangePTS: public oiExchangeInterface
{
public:
    oiExchangePTS();

    bool importOiData(oiExchangeObject& data);
    bool exportOiData(oiExchangeObject& data);

    QList<Configuration::ElementTypes> getSupportedElements();

    QStringList getElementDescription(Configuration::ElementTypes);

    QList<UnitConverter::unitType> getSupportedDistanceUnits();
    QList<UnitConverter::unitType> getSupportedAngleUnits();
    QList<UnitConverter::unitType> getSupportedTemperatureUnits();

private:
    // private functions for importiing different element types
    bool importPointCloud(oiExchangeObject& data);
};

#endif // OIEXCHANGEPTS_H
