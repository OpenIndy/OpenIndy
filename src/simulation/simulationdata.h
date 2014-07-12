#ifndef SIMULATIONDATA_H
#define SIMULATIONDATA_H

#include <QString>
#include <QList>
#include <QMap>

struct UncertaintyData{

    QList<double> values;

    double maxValue;
    double minValue;

    double uncertainty;
    QString distribution;

    double (*densityFunction)(double x);
    double (*distributionFunction)(double x);

    QMap<QString, QString> info;

};

class SimulationData
{
public:
    SimulationData();

    UncertaintyData uncertaintyX;
    UncertaintyData uncertaintyY;
    UncertaintyData uncertaintyZ;
    UncertaintyData uncertaintyI;
    UncertaintyData uncertaintyJ;
    UncertaintyData uncertaintyK;
    UncertaintyData uncertaintyRadius;
    UncertaintyData uncertaintyScalar;

    void addXYZ(double x, double y, double z);
    void addIJK(double i, double j, double k);
    void addScalar(double s);
    void addRadius(double r);


};

#endif // SIMULATIONDATA_H
