#include "residual.h"

Residual::Residual()
{
}

void Residual::addValue(QString key, double value, UnitConverter::dimensionType dimension){

    this->residual.insert(key,value);
    this->residualUnitType.insert(key, dimension);
    this->residualName.append(key);
}

QMap<QString,double> Residual::getResidualMap(){
    return this->residual;
}

QMap<QString, UnitConverter::dimensionType> Residual::getResidualUnitMap(){
    return this->residualUnitType;
}
