#ifndef OIMAT_H
#define OIMAT_H

#include <vector>
#include <stdexcept>

#include "global.h"

class LinearAlgebra;
class OiVec;

using namespace std;

/*!
 * \brief The Rotation struct
 * Struct that represents a rotation around one of the three coordinate axes
 */
struct OI_LIB_EXPORT Rotation{

    enum RotationAxis{
        X_AXIS,
        Y_AXIS,
        Z_AXIS
    };

    Rotation(double angle, RotationAxis axis){
        this->angle = angle;
        this->axis = axis;
    }

    RotationAxis axis;
    double angle;

};

/*!
 * \brief The RotationChain class
 */
class OI_LIB_EXPORT RotationChain{

public:
    void clear(){ this->myRotations.clear(); }
    void appendRotation(Rotation r){ this->myRotations.push_back(r); }
    int getRotationCount(){ return this->myRotations.size(); }
    Rotation getRotationAt(int index){ return this->myRotations.at(index); }

private:
    vector<Rotation> myRotations;

};

/*!
 * \brief The OiMat class
 */
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
    void setRow(const int index, const OiVec &row);
    void setCol(const int index, const OiVec &col);

    OiMat& operator=(const OiMat &m);
    OiMat operator+(const OiMat &m) const;
    OiMat operator-(const OiMat &m) const;
    OiMat& operator+=(const OiMat &m);
    OiMat& operator-=(const OiMat &m);
    OiMat operator*(const OiMat &m) const;
    OiMat operator*(const double value) const;
    OiVec operator*(const OiVec &v) const;
    friend OiMat operator*(const double value, const OiMat &m){
        return OiMat::mult(value, m);
    }
    friend OiMat operator/(const OiMat &m, const double value){
        return OiMat::mult((1.0 / value), m);
    }

    OiMat t() const;
    OiMat inv() const;

    double det() const;

    void svd(OiMat &u, OiVec &d, OiMat &v) const;

    static OiMat getRotationMatrix(double angle, OiVec axis);
    static OiMat getRotationMatrix(double angle, Rotation::RotationAxis axis);
    static OiMat getRotationMatrix(RotationChain rotationChain);

private:
    static OiMat mult(const double value, const OiMat &m);

};

#endif // OIMAT_H
