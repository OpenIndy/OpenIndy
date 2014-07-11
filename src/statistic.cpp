#include "statistic.h"

Statistic::Statistic()
{
    this->isValid = false;
    s0_apriori = 0.0;
    s0_aposteriori = 0.0;
    stdev = 0.0;
}

Statistic::Statistic(const Statistic &stat){
    this->s0_apriori = stat.s0_apriori;
    this->s0_aposteriori = stat.s0_aposteriori;
    this->stdev = stat.stdev;
    this->displayResiduals = stat.displayResiduals;
    this->isValid = stat.isValid;
    this->p.replace(stat.p);
    this->qxx.replace(stat.qxx);
    this->v.replace(stat.v);
    this->setSimulationData(stat.getSimulationData());
}

Statistic& Statistic::operator =(const Statistic &stat){
    this->s0_apriori = stat.s0_apriori;
    this->s0_aposteriori = stat.s0_aposteriori;
    this->stdev = stat.stdev;
    this->displayResiduals = stat.displayResiduals;
    this->isValid = stat.isValid;
    this->p.replace(stat.p);
    this->qxx.replace(stat.qxx);
    this->v.replace(stat.v);
    this->setSimulationData(stat.getSimulationData());
    return *this;
}

void Statistic::addSimulationXYZ(double x, double y, double z)
{
    simulationData.x.append(x);
    simulationData.y.append(y);
    simulationData.z.append(z);
}

void Statistic::addSimulationIJK(double i, double j, double k)
{
    simulationData.i.append(i);
    simulationData.j.append(j);
    simulationData.k.append(k);
}

void Statistic::addSimulationScalar(double s)
{
    simulationData.scalar.append(s);
}

void Statistic::addSimulationRadius(double r)
{
    simulationData.radius.append(r);
}

QList<double> Statistic::getSimulationX()
{
    return simulationData.x;
}

QList<double> Statistic::getSimulationY()
{
    return simulationData.y;
}

QList<double> Statistic::getSimulationZ()
{
    return simulationData.z;
}

QList<double> Statistic::getSimulationI()
{
    return simulationData.i;
}

QList<double> Statistic::getSimulationJ()
{
    return simulationData.j;
}

QList<double> Statistic::getSimulationK()
{
    return simulationData.k;
}

QList<double> Statistic::getSimulationScalar()
{
    return simulationData.scalar;
}

QList<double> Statistic::getSimulationRadius()
{
    return simulationData.radius;
}

SimulationData Statistic::getSimulationData() const
{
    return simulationData;
}

void Statistic::setSimulationData(SimulationData d)
{
    this->simulationData = d;
}
