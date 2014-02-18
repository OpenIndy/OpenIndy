#ifndef RESIDUAL_H
#define RESIDUAL_H

#include <QMap>
#include <QString>
#include "unitconverter.h"

class Residual
{
public:
    Residual();

    void addValue(QString key, double value, UnitConverter::dimensionType dimension);

    QMap<QString, double> getResidualMap();
    QMap<QString, UnitConverter::dimensionType> getResidualUnitMap();

    QList<QString> residualName;

    QMap<QString,double> residual;

    QMap<QString, UnitConverter::dimensionType> residualUnitType;

};

#endif // RESIDUAL_H
