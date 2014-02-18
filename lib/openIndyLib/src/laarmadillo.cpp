#include "laarmadillo.h"

LAArmadillo::LAArmadillo()
{
}

LAArmadillo::~LAArmadillo(){

}

/*!
 * \brief LAArmadillo::addIn
 * Add the second vector to the first
 * \param v1
 * \param v2
 * \return
 */
OiVec LAArmadillo::addIn(OiVec v1, OiVec v2){
    arma::vec v1a = this->oiVec2Arma(v1);
    arma::vec v2a = this->oiVec2Arma(v2);
    return this->arma2OiVec( (v1a + v2a) );
}

/*!
 * \brief LAArmadillo::addIn
 * Add the second matrix to the first
 * \param m1
 * \param m2
 * \return
 */
OiMat LAArmadillo::addIn(OiMat m1, OiMat m2){
    arma::mat m1a = this->oiMat2Arma(m1);
    arma::mat m2a = this->oiMat2Arma(m2);
    return this->arma2OiMat( (m1a + m2a) );
}

/*!
 * \brief LAArmadillo::substract
 * Substract the second vector from the first
 * \param v1
 * \param v2
 * \return
 */
OiVec LAArmadillo::substract(OiVec v1, OiVec v2){
    arma::vec v1a = this->oiVec2Arma(v1);
    arma::vec v2a = this->oiVec2Arma(v2);
    return this->arma2OiVec( (v1a - v2a) );
}

/*!
 * \brief LAArmadillo::substract
 * Substract the second matrix from the first
 * \param m1
 * \param m2
 * \return
 */
OiMat LAArmadillo::substract(OiMat m1, OiMat m2){
    arma::mat m1a = this->oiMat2Arma(m1);
    arma::mat m2a = this->oiMat2Arma(m2);
    return this->arma2OiMat( (m1a - m2a) );
}

/*!
 * \brief LAArmadillo::multiply
 * Multiply the second matrix by the first
 * \param m1
 * \param m2
 * \return
 */
OiMat LAArmadillo::multiply(OiMat m1, OiMat m2){
    arma::mat m1a = this->oiMat2Arma(m1);
    arma::mat m2a = this->oiMat2Arma(m2);
    return this->arma2OiMat( (m1a * m2a) );
}

/*!
 * \brief LAArmadillo::multiply
 * Multiply the vector by the matrix
 * \param m
 * \param v
 * \return
 */
OiVec LAArmadillo::multiply(OiMat m, OiVec v){
    arma::mat ma = this->oiMat2Arma(m);
    arma::vec va = this->oiVec2Arma(v);
    return this->arma2OiVec( (ma * va) );
}

/*!
 * \brief LAArmadillo::multiply
 * Multiply the matrix by the scalar
 * \param s
 * \param m
 * \return
 */
OiMat LAArmadillo::multiply(double s, OiMat m){
    arma::mat ma = this->oiMat2Arma(m);
    return this->arma2OiMat( (s * ma) );
}

/*!
 * \brief LAArmadillo::multiply
 * Multiply the vector by the scalar
 * \param s
 * \param v
 * \return
 */
OiVec LAArmadillo::multiply(double s, OiVec v){
    arma::vec va = this->oiVec2Arma(v);
    return this->arma2OiVec( (s * va) );
}

/*!
 * \brief LAArmadillo::invert
 * Invert the matrix
 * \param m
 * \return
 */
OiMat LAArmadillo::invert(OiMat m){
    arma::mat ma = this->oiMat2Arma(m);
    arma::mat result;
    try{
        result = arma::inv(ma); //try to calc the standard inverse
    }catch(runtime_error e){
        try{
            //Console::addLine("Matrix is singular, so trying to calc Moore-Penrose pseudo-inverse");
            result = arma::pinv(ma);
        }catch(runtime_error e){
            //Console::addLine("Cannot calculate inverse matrix");
            return OiMat();
        }
    }
    return this->arma2OiMat( result );
}

/*!
 * \brief LAArmadillo::transpose
 * Transpose the matrix
 * \param m
 * \return
 */
OiMat LAArmadillo::transpose(OiMat m){
    arma::mat ma = this->oiMat2Arma(m);
    return this->arma2OiMat( ma.t() );
}

/*!
 * \brief LAArmadillo::svd
 * Compute singular value decomposition
 * \param u
 * \param d
 * \param v
 * \param x
 */
void LAArmadillo::svd(OiMat &u, OiVec &d, OiMat &v, OiMat x){
    qDebug() << "in svd";
    arma::mat ua;
    arma::vec da;
    arma::mat va;
    qDebug() << "vor trafo";
    arma::mat xa = this->oiMat2Arma(x);
    qDebug() << "nach trafo";
    arma::svd(ua, da, va, xa);
    qDebug() << "nach svd";
    u = arma2OiMat(ua);
    d = arma2OiVec(da);
    v = arma2OiMat(va);
}

/*!
 * \brief LAArmadillo::cross
 * Calculate cross product of 2 vectors
 * \param a
 * \param b
 * \return
 */
OiVec LAArmadillo::cross(OiVec a, OiVec b){
    arma::vec aa = this->oiVec2Arma(a);
    arma::vec ba = this->oiVec2Arma(b);
    arma::vec ca = arma::cross(aa, ba);
    return this->arma2OiVec(ca);
}

/*!
 * \brief LAArmadillo::dot
 * Calculate scalar product of 2 vectors
 * \param a
 * \param b
 * \return
 */
double LAArmadillo::dot(OiVec a, OiVec b){
    arma::vec aa = this->oiVec2Arma(a);
    arma::vec ba = this->oiVec2Arma(b);
    double c = arma::dot(aa, ba);
    return c;
}

/*!
 * \brief LAArmadillo::oiVec2Arma
 * \return
 */
arma::vec LAArmadillo::oiVec2Arma(OiVec v){
    arma::vec result(v.getSize());
    for(int i = 0; i < v.getSize(); i++){
        result(i) = v.getAt(i);
    }
    return result;
}

/*!
 * \brief LAArmadillo::oiMat2Arma
 * \return
 */
arma::mat LAArmadillo::oiMat2Arma(OiMat m){
    arma::mat result(m.getRowCount(), m.getColCount());
    for(int i = 0; i < m.getRowCount(); i++){
        for(int j = 0; j < m.getColCount(); j++){
            result(i, j) = m.getAt(i, j);
        }
    }
    return result;
}

/*!
 * \brief LAArmadillo::arma2OiVec
 * \return
 */
OiVec LAArmadillo::arma2OiVec(arma::vec v){
    OiVec result(v.n_rows);
    for(int i = 0; i < v.n_rows; i++){
        result.setAt(i, v(i));
    }
    return result;
}

/*!
 * \brief LAArmadillo::arma2OiMat
 * \return
 */
OiMat LAArmadillo::arma2OiMat(arma::mat m){
    OiMat result(m.n_rows, m.n_cols);
    for(int i = 0; i < m.n_rows; i++){
        for(int j = 0; j < m.n_cols; j++){
            result.setAt(i, j, m(i, j));
        }
    }
    return result;
}
