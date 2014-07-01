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
    this->simulationData = stat.simulationData;
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
    this->simulationData = stat.simulationData;
    return *this;
}
