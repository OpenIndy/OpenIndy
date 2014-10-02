#ifndef LAARMADILLO_H
#define LAARMADILLO_H

#include <QDebug>

#include "linearalgebra.h"
#include "oivec.h"
#include "oimat.h"
#include "armadillo"

#include "global.h"

class OI_LIB_EXPORT LAArmadillo : public LinearAlgebra
{
public:
    LAArmadillo();
    ~LAArmadillo();

    void addIn(OiVec &result, const OiVec &v1, const OiVec &v2);
    void addIn(OiMat &result, const OiMat &m1, const OiMat &m2);
    void substract(OiVec &result, const OiVec &v1, const OiVec &v2);
    void substract(OiMat &result, const OiMat &m1, const OiMat &m2);
    void multiply(OiMat &result, const OiMat &m1, const OiMat &m2);
    void multiply(OiVec &result, const OiMat &m, const OiVec &v);
    void multiply(OiMat &result, const double &s, const OiMat &m);
    void multiply(OiVec &result, const double &s, const OiVec &v);
    bool invert(OiMat &result, const OiMat &m);
    void transpose(OiMat &result, const OiMat &m);
    bool svd(OiMat &u, OiVec &d, OiMat &v, const OiMat &x);
    void cross(OiVec &result, const OiVec &a, const OiVec &b);
    void dot(double &result, const OiVec &a, const OiVec &b);
    void det(double &result, const OiMat &a);
    bool solve(OiVec &x, const OiMat &A, const OiVec &b);
    bool solve(OiMat &X, const OiMat &A, const OiMat &B);

private:
    void oiVec2Arma(arma::vec &result, const OiVec &v);
    void oiMat2Arma(arma::mat &result, const OiMat &m);
    void arma2OiVec(OiVec &result, const arma::vec &v);
    void arma2OiMat(OiMat &result, const arma::mat &m);

};

#endif // LAARMADILLO_H
