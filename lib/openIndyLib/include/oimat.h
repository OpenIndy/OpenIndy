#ifndef OIMAT_H
#define OIMAT_H

#include <vector>
#include <stdexcept>

#include "global.h"

class LinearAlgebra;
class OiVec;

using namespace std;

class OI_LIB_EXPORT OiMat
{
public:
    OiMat();
    OiMat(const int rows, const int cols);

    static LinearAlgebra *myLinearAlgebra;

private:
    vector< vector<double> > values;


public:

    double getAt(const int row, const int col) const;
    int getRowCount() const;
    int getColCount() const;
    OiVec getRow(const int row) const;
    OiVec getCol(const int col) const;

    void replace(const OiMat &m);
    void setAt(const int row, const int col, const double value);
    void diag(vector<double> diagVec);

    OiMat& operator=(const OiMat &m);
    OiMat operator+(const OiMat &m);
    OiMat operator-(const OiMat &m);
    OiMat& operator+=(const OiMat &m);
    OiMat& operator-=(const OiMat &m);
    OiMat operator*(const OiMat &m);
    OiMat operator*(const double value);
    OiVec operator*(const OiVec &v);
    friend OiMat operator*(const OiMat &m, const double value){
        return OiMat::mult(value, m);
    }
    friend OiMat operator*(const double value, const OiMat &m){
        return OiMat::mult(value, m);
    }
    friend OiMat operator/(const OiMat &m, const double value){
        return OiMat::mult((1.0 / value), m);
    }

    OiMat t();
    OiMat inv();

    void svd(OiMat &u, OiVec &d, OiMat &v);

private:
    static OiMat mult(const double value, const OiMat &m);

};

#endif // OIMAT_H
