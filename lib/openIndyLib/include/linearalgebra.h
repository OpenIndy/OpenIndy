#ifndef LINEARALGEBRA_H
#define LINEARALGEBRA_H

#include "oivec.h"
#include "oimat.h"

#include "global.h"

class OI_LIB_EXPORT LinearAlgebra
{
public:
    virtual ~LinearAlgebra(){}

    /*!
     * \brief addIn
     * Add v2 to v1
     * \param result
     * \param v1
     * \param v2
     */
    virtual void addIn(OiVec &result, const OiVec &v1, const OiVec &v2) = 0;

    /*!
     * \brief addIn
     * Add m2 to m1
     * \param result
     * \param m1
     * \param m2
     */
    virtual void addIn(OiMat &result, const OiMat &m1, const OiMat &m2) = 0;

    /*!
     * \brief substract
     * Substract v2 from v1
     * \param result
     * \param v1
     * \param v2
     */
    virtual void substract(OiVec &result, const OiVec &v1, const OiVec &v2) = 0;

    /*!
     * \brief substract
     * Substract m2 from m1
     * \param result
     * \param m1
     * \param m2
     */
    virtual void substract(OiMat &result, const OiMat &m1, const OiMat &m2) = 0;

    /*!
     * \brief multiply
     * Multiply m2 by m1
     * \param result
     * \param m1
     * \param m2
     */
    virtual void multiply(OiMat &result, const OiMat &m1, const OiMat &m2) = 0;

    /*!
     * \brief multiply
     * Multiply v by m
     * \param result
     * \param m
     * \param v
     */
    virtual void multiply(OiVec &result, const OiMat &m, const OiVec &v) = 0;

    /*!
     * \brief multiply
     * Multiply m by s
     * \param result
     * \param s
     * \param m
     */
    virtual void multiply(OiMat &result, const double &s, const OiMat &m) = 0;

    /*!
     * \brief multiply
     * Multiply v by s
     * \param result
     * \param s
     * \param v
     */
    virtual void multiply(OiVec &result, const double &s, const OiVec &v) = 0;

    /*!
     * \brief invert
     * Invert m
     * \param result
     * \param m
     * \return
     */
    virtual bool invert(OiMat &result, const OiMat &m) = 0;

    /*!
     * \brief transpose
     * Transpose m
     * \param result
     * \param m
     */
    virtual void transpose(OiMat &result, const OiMat &m) = 0;

    /*!
     * \brief svd
     * Compute singular value decomposition of x
     * \param u
     * \param d
     * \param v
     * \param x
     * \return
     */
    virtual bool svd(OiMat &u, OiVec &d, OiMat &v, const OiMat &x) = 0;

    /*!
     * \brief cross
     * Calculate cross product of a and b
     * \param result
     * \param a
     * \param b
     */
    virtual void cross(OiVec &result, const OiVec &a, const OiVec &b) = 0;

    /*!
     * \brief dot
     * Calculate scalar product of a and b
     * \param result
     * \param a
     * \param b
     */
    virtual void dot(double &result, const OiVec &a, const OiVec &b) = 0;

    /*!
     * \brief det
     * Calculate the determinant of a
     * \param result
     * \param a
     */
    virtual void det(double &result, const OiMat &a) = 0;

    /*!
     * \brief solve
     * Solves a linear equation of the form A*x = b (x = inv(A)*b)
     * \param x
     * \param A
     * \param b
     * \return
     */
    virtual bool solve(OiVec &x, const OiMat &A, const OiVec &b) = 0;

    /*!
     * \brief solve
     * Solves a linear equation of the form A*X = B (X = inv(A)*B)
     * \param X
     * \param A
     * \param B
     * \return
     */
    virtual bool solve(OiMat &X, const OiMat &A, const OiMat &B) = 0;

};

#endif // LINEARALGEBRA_H
