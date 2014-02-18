#ifndef STATISTIC_H
#define STATISTIC_H

#include <QList>
#include "oivec.h"
#include "oimat.h"
#include "residual.h"

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

};

#endif // STATISTIC_H
