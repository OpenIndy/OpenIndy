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
     * Add the second vector to the first
     * \param v1
     * \param v2
     * \return
     */
    virtual OiVec addIn(OiVec v1, OiVec v2) = 0;

    /*!
     * \brief addIn
     * Add the second matrix to the first
     * \param m1
     * \param m2
     * \return
     */
    virtual OiMat addIn(OiMat m1, OiMat m2) = 0;

    /*!
     * \brief substract
     * Substract the second vector from the first
     * \param v1
     * \param v2
     * \return
     */
    virtual OiVec substract(OiVec v1, OiVec v2) = 0;

    /*!
     * \brief substract
     * Substract the second matrix from the first
     * \param m1
     * \param m2
     * \return
     */
    virtual OiMat substract(OiMat m1, OiMat m2) = 0;

    /*!
     * \brief multiply
     * Multiply the second matrix by the first
     * \param m1
     * \param m2
     * \return
     */
    virtual OiMat multiply(OiMat m1, OiMat m2) = 0;

    /*!
     * \brief multiply
     * Multiply the vector by the matrix
     * \param m
     * \param v
     * \return
     */
    virtual OiVec multiply(OiMat m, OiVec v) = 0;

    /*!
     * \brief multiply
     * Multiply the matrix by the scalar
     * \param s
     * \param m
     * \return
     */
    virtual OiMat multiply(double s, OiMat m) = 0;

    /*!
     * \brief multiply
     * Multiply the vector by the scalar
     * \param s
     * \param v
     * \return
     */
    virtual OiVec multiply(double s, OiVec v) = 0;

    /*!
     * \brief invert
     * Invert the matrix
     * \param m
     * \return
     */
    virtual OiMat invert(OiMat m) = 0;

    /*!
     * \brief transpose
     * Transpose the matrix
     * \param m
     * \return
     */
    virtual OiMat transpose(OiMat m) = 0;

    /*!
     * \brief svd
     * Compute singular value decomposition
     * \param u
     * \param d
     * \param v
     * \param x
     */
    virtual void svd(OiMat &u, OiVec &d, OiMat &v, OiMat x) = 0;

    /*!
     * \brief cross
     * Calculate cross product of 2 vectors
     * \param a
     * \param b
     * \return
     */
    virtual OiVec cross(OiVec a, OiVec b) = 0;

    /*!
     * \brief dot
     * Calculate scalar product of 2 vectors
     * \param a
     * \param b
     * \return
     */
    virtual double dot(OiVec a, OiVec b) = 0;

    /*!
     * \brief det
     * Calculate the determinant of the matrix
     * \param a
     * \return
     */
    virtual double det(OiMat a) = 0;
};

#endif // LINEARALGEBRA_H
