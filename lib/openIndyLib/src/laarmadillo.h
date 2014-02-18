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

    OiVec addIn(OiVec v1, OiVec v2);
    OiMat addIn(OiMat m1, OiMat m2);
    OiVec substract(OiVec v1, OiVec v2);
    OiMat substract(OiMat m1, OiMat m2);
    OiMat multiply(OiMat m1, OiMat m2);
    OiVec multiply(OiMat m, OiVec v);
    OiMat multiply(double s, OiMat m);
    OiVec multiply(double s, OiVec v);
    OiMat invert(OiMat m);
    OiMat transpose(OiMat m);
    void svd(OiMat &u, OiVec &d, OiMat &v, OiMat x);
    OiVec cross(OiVec a, OiVec b);
    double dot(OiVec a, OiVec b);

private:
    arma::vec oiVec2Arma(OiVec);
    arma::mat oiMat2Arma(OiMat);
    OiVec arma2OiVec(arma::vec);
    OiMat arma2OiMat(arma::mat);

};

#endif // LAARMADILLO_H
