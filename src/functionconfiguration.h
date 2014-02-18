#ifndef FUNCTIONCONFIGURATION_H
#define FUNCTIONCONFIGURATION_H

#include <QMap>
#include <QString>

class FunctionConfiguration
{
public:
    FunctionConfiguration();

    QMap<QString, double> doubleParameter;
    QMap<QString, int> intParameter;
    QMap<QString, QString> stringParameter;
    bool functionConfigSet;
};

#endif // FUNCTIONCONFIGURATION_H
