#ifndef OIVEC_H
#define OIVEC_H

#include <QDebug>
#include <QtCore/QtMath>
#include <vector>
#include <stdexcept>

#include "global.h"

class LinearAlgebra;
class OiMat;

using namespace std;

class OI_LIB_EXPORT OiVec
{
public:
    OiVec();
    OiVec(const int size);
    ~OiVec();

    static LinearAlgebra *myLinearAlgebra;

private:
    vector<double> values;

public:
    double getAt(int pos) const;
    int getSize() const;

    void set(vector<double> v);
    void setAt(int pos,double value);

    void add(double value);

    void replace(const OiVec&);

    OiVec& operator=(const OiVec&);
    OiVec operator+(const OiVec&);
    OiVec operator-(const OiVec&);
    OiVec& operator+=(const OiVec&);
    OiVec& operator-=(const OiVec&);
    friend OiVec operator*(const OiVec &v, const double value){
        return OiVec::mult(value, v);
    }
    friend OiVec operator*(const double value, const OiVec &v){
        return OiVec::mult(value, v);
    }
    friend OiVec operator/(const OiVec &v, const double value){
        return OiVec::mult((1.0 / value), v);
    }

    OiVec normalize();
    OiMat t();

    static OiVec cross(OiVec a, OiVec b);
    static double dot(OiVec a, OiVec b);

private:
    static OiVec mult(const double value, const OiVec &v);

};

#endif // OIVEC_H
