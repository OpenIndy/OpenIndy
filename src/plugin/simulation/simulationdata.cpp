#include "simulationdata.h"

SimulationData::SimulationData()
{
}

void SimulationData::addXYZ(double x, double y, double z)
{
    this->uncertaintyX.values.append(x);
    this->uncertaintyY.values.append(y);
    this->uncertaintyZ.values.append(z);
}

void SimulationData::addIJK(double i, double j, double k)
{
    this->uncertaintyI.values.append(i);
    this->uncertaintyJ.values.append(j);
    this->uncertaintyK.values.append(k);
}

void SimulationData::addScalar(double s)
{
    this->uncertaintyScalar.values.append(s);

}

void SimulationData::addRadius(double r)
{
    this->uncertaintyRadius.values.append(r);
}
