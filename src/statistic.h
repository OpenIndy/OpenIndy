#ifndef STATISTIC_H
#define STATISTIC_H

#include <QList>

#include "oivec.h"
#include "oimat.h"
#include "types.h"

/*!
 * \brief The Residual class
 */
class Residual{
public:
    int elementId; //id of the element this residual belongs to
    QMap<QString, double> corrections; //the correction values (key: name)
    DimensionType dimension; //dimension of the correction values
};

/*!
 * \brief The Statistic class
 */
class Statistic
{

public:
    Statistic();
    Statistic(const Statistic &copy);

    Statistic& operator=(const Statistic &other);

    //###################################
    //get valid state and reset statistic
    //###################################

    const bool &getIsValid() const;
    void setIsValid(const bool &isValid);

    void reset();

    //#################################
    //get or set statistical parameters
    //#################################

    const double &getS0APriori() const;
    void setS0APriori(const double &s0);

    const double &getS0APosteriori() const;
    void setS0APosteriori(const double &s0);

    const double &getStdev() const;
    void setStdev(const double &stdev);

    const OiMat &getP() const;
    void setP(const OiMat &p);

    const OiMat &getQxx() const;
    void setQxx(const OiMat &qxx);

    const OiVec &getV() const;
    void setV(const OiVec &v);

    const QList<Residual> &getDisplayResiduals() const;
    void addDisplayResidual(const Residual &residual);

private:
    bool isValid;

    //######################
    //statistical parameters
    //######################

    double s0_apriori;
    double s0_aposteriori;

    double stdev; //standard deviation of vertical distances from geometry surface

    OiMat p;
    OiMat qxx;

    OiVec v; //vertical distances from geometry surface
    QList<Residual> displayResiduals;

};

#endif // STATISTIC_H
