#ifndef STATISTIC_H
#define STATISTIC_H

#include <QList>
#include "oivec.h"
#include "oimat.h"
#include "residual.h"

struct SimulationData{
    QList<double> x;
    QList<double> y;
    QList<double> z;
    QList<double> i;
    QList<double> j;
    QList<double> k;
    QList<double> radius;
    QList<double> scalar;
};

/*!
 * \brief The Statistic class
 * Class that calculates and contains all the statistics of some elements.
 */

class Statistic
{
public:
    Statistic();
    Statistic(const Statistic&);

    Statistic& operator=(const Statistic&);

    double s0_apriori;
    double s0_aposteriori;
    double stdev;
    OiMat p;
    OiMat qxx;
    OiVec v;

    QList<Residual> displayResiduals;

    bool isValid;

    void addSimulationXYZ(double x, double y, double z);
    void addSimulationIJK(double i, double j, double k);
    void addSimulationScalar(double s);
    void addSimulationRadius(double r);

    QList<double> getSimulationX();
    QList<double> getSimulationY();
    QList<double> getSimulationZ();
    QList<double> getSimulationI();
    QList<double> getSimulationJ();
    QList<double> getSimulationK();
    QList<double> getSimulationScalar();
    QList<double> getSimulationRadius();

    SimulationData getSimulationData() const;
    void setSimulationData(SimulationData d);

private:
    SimulationData simulationData;

};

#endif // STATISTIC_H
