#include "laarmadillo.h"

LAArmadillo::LAArmadillo()
{
}

LAArmadillo::~LAArmadillo(){

}

/*!
 * \brief LAArmadillo::addIn
 * Add v2 to v1
 * \param result
 * \param v1
 * \param v2
 */
void LAArmadillo::addIn(OiVec &result, const OiVec &v1, const OiVec &v2){
    int vecSize = v1.getSize();
    arma::vec v1a(vecSize), v2a(vecSize);

    this->oiVec2Arma(v1a, v1);
    this->oiVec2Arma(v2a, v2);

    this->arma2OiVec(result, (v1a + v2a));
}

/*!
 * \brief LAArmadillo::addIn
 * Add m2 to m1
 * \param result
 * \param m1
 * \param m2
 */
void LAArmadillo::addIn(OiMat &result, const OiMat &m1, const OiMat &m2){
    int matRows = m1.getRowCount();
    int matCols = m1.getColCount();
    arma::mat m1a(matRows, matCols), m2a(matRows, matCols);

    this->oiMat2Arma(m1a, m1);
    this->oiMat2Arma(m2a, m2);

    this->arma2OiMat(result, (m1a + m2a));
}

/*!
 * \brief LAArmadillo::substract
 * Substract v2 from v1
 * \param result
 * \param v1
 * \param v2
 */
void LAArmadillo::substract(OiVec &result, const OiVec &v1, const OiVec &v2){
    int vecSize = v1.getSize();
    arma::vec v1a(vecSize), v2a(vecSize);

    this->oiVec2Arma(v1a, v1);
    this->oiVec2Arma(v2a, v2);

    this->arma2OiVec(result, (v1a - v2a));
}

/*!
 * \brief LAArmadillo::substract
 * Substract m2 from m1
 * \param result
 * \param m1
 * \param m2
 */
void LAArmadillo::substract(OiMat &result, const OiMat &m1, const OiMat &m2){
    int matRows = m1.getRowCount();
    int matCols = m1.getColCount();
    arma::mat m1a(matRows, matCols), m2a(matRows, matCols);

    this->oiMat2Arma(m1a, m1);
    this->oiMat2Arma(m2a, m2);

    this->arma2OiMat(result, (m1a - m2a));
}

/*!
 * \brief LAArmadillo::multiply
 * Multiply m2 by m1
 * \param result
 * \param m1
 * \param m2
 */
void LAArmadillo::multiply(OiMat &result, const OiMat &m1, const OiMat &m2){
    int matRowCol = m1.getColCount();
    arma::mat m1a(m1.getRowCount(), matRowCol), m2a(matRowCol, m2.getColCount());

    this->oiMat2Arma(m1a, m1);
    this->oiMat2Arma(m2a, m2);

    this->arma2OiMat(result, (m1a * m2a));
}

/*!
 * \brief LAArmadillo::multiply
 * Multiply v by m
 * \param result
 * \param m
 * \param v
 */
void LAArmadillo::multiply(OiVec &result, const OiMat &m, const OiVec &v){
    int vecSize = v.getSize();
    arma::vec va(vecSize);
    arma::mat ma(m.getRowCount(), vecSize);

    this->oiVec2Arma(va, v);
    this->oiMat2Arma(ma, m);

    this->arma2OiVec(result, (ma * va));
}

/*!
 * \brief LAArmadillo::multiply
 * Multiply m by s
 * \param result
 * \param s
 * \param m
 */
void LAArmadillo::multiply(OiMat &result, const double &s, const OiMat &m){
    arma::mat ma(m.getRowCount(), m.getColCount());

    this->oiMat2Arma(ma, m);

    this->arma2OiMat(result, (s * ma));
}

/*!
 * \brief LAArmadillo::multiply
 * Multiply v by s
 * \param result
 * \param s
 * \param v
 */
void LAArmadillo::multiply(OiVec &result, const double &s, const OiVec &v){
    arma::vec va(v.getSize());

    this->oiVec2Arma(va, v);

    this->arma2OiVec(result, (s * va));
}

/*!
 * \brief LAArmadillo::invert
 * Invert m
 * \param result
 * \param m
 * \return
 */
bool LAArmadillo::invert(OiMat &result, const OiMat &m){
    int matSize = m.getRowCount();
    arma::mat ma(matSize, matSize);

    this->oiMat2Arma(ma, m);

    try{
        this->arma2OiMat(result, arma::inv(ma)); //try to calc the standard inverse
    }catch(const runtime_error &e1){
        try{
            this->arma2OiMat(result, arma::pinv(ma));
        }catch(const runtime_error &e2){
            return false;
        }
    }

    return true;
}

/*!
 * \brief LAArmadillo::transpose
 * Transpose m
 * \param result
 * \param m
 */
void LAArmadillo::transpose(OiMat &result, const OiMat &m){
    arma::mat ma(m.getRowCount(), m.getColCount());

    this->oiMat2Arma(ma, m);

    this->arma2OiMat(result, ma.t());
}

/*!
 * \brief LAArmadillo::svd
 * Compute singular value decomposition of x
 * \param u
 * \param d
 * \param v
 * \param x
 * \return
 */
bool LAArmadillo::svd(OiMat &u, OiVec &d, OiMat &v, const OiMat &x){
    int matSize = x.getRowCount();
    arma::mat ua(matSize, matSize), va(matSize, matSize), xa(matSize, matSize);
    arma::vec da(matSize);

    this->oiMat2Arma(xa, x);

    arma::svd(ua, da, va, xa);

    this->arma2OiMat(u, ua);
    this->arma2OiMat(v, va);
    this->arma2OiVec(d, da);

    return true;
}

/*!
 * \brief LAArmadillo::cross
 * Calculate cross product of a and b
 * \param result
 * \param a
 * \param b
 */
void LAArmadillo::cross(OiVec &result, const OiVec &a, const OiVec &b){
    int vecSize = b.getSize();
    arma::vec aa(vecSize), ba(vecSize);

    this->oiVec2Arma(aa, a);
    this->oiVec2Arma(ba, b);

    this->arma2OiVec(result, arma::cross(aa, ba));
}

/*!
 * \brief LAArmadillo::dot
 * Calculate scalar product of a and b
 * \param result
 * \param a
 * \param b
 */
void LAArmadillo::dot(double &result, const OiVec &a, const OiVec &b){
    int vecSize = b.getSize();
    arma::vec aa(vecSize), ba(vecSize);

    this->oiVec2Arma(aa, a);
    this->oiVec2Arma(ba, b);

    result = arma::dot(aa, ba);
}

/*!
 * \brief LAArmadillo::det
 * Calculate the determinant of a
 * \param result
 * \param a
 */
void LAArmadillo::det(double &result, const OiMat &a){
    int matSize = a.getRowCount();
    arma::mat aa(matSize, matSize);

    this->oiMat2Arma(aa, a);

    result = arma::det(aa);
}

/*!
 * \brief LAArmadillo::solve
 * Solves a linear equation of the form A*x = b (x = inv(A)*b)
 * \param x
 * \param A
 * \param b
 * \return
 */
bool LAArmadillo::solve(OiVec &x, const OiMat &A, const OiVec &b){
    int xSize = x.getSize();
    arma::mat Aa(xSize, xSize);
    arma::vec ba(xSize);

    this->oiMat2Arma(Aa, A);
    this->oiVec2Arma(ba, b);

    try{
        arma::vec xa = arma::solve(Aa, ba);
        this->arma2OiVec(x, xa);
        return true;
    }catch(const runtime_error &e){
        return false;
    }
}

/*!
 * \brief LAArmadillo::solve
 * Solves a linear equation of the form A*X = B (X = inv(A)*B)
 * \param X
 * \param A
 * \param B
 * \return
 */
bool LAArmadillo::solve(OiMat &X, const OiMat &A, const OiMat &B){
    int XSize = X.getRowCount();
    arma::mat Aa(XSize, XSize);
    arma::mat Ba(XSize, XSize);

    this->oiMat2Arma(Aa, A);
    this->oiMat2Arma(Ba, B);

    try{
        arma::mat Xa = arma::solve(Aa, Ba);
        this->arma2OiMat(X, Xa);
        return true;
    }catch(const runtime_error &e){
        return false;
    }
}

/*!
 * \brief LAArmadillo::oiVec2Arma
 * \param result
 * \param v
 */
void LAArmadillo::oiVec2Arma(arma::vec &result, const OiVec &v){
    for(int i = 0; i < v.getSize(); i++){
        result[i] = v.getAt(i);
    }
}

/*!
 * \brief LAArmadillo::oiMat2Arma
 * \param result
 * \param m
 */
void LAArmadillo::oiMat2Arma(arma::mat &result, const OiMat &m){
    for(int row = 0; row < m.getRowCount(); row++){
        for(int col = 0; col < m.getColCount(); col++){
            result.at(row, col) = m.getAt(row, col);
        }
    }
}

/*!
 * \brief LAArmadillo::arma2OiVec
 * \param result
 * \param v
 */
void LAArmadillo::arma2OiVec(OiVec &result, const arma::vec &v){
    for(int i = 0; i < v.n_rows; i++){
        result.setAt(i, v[i]);
    }
}

/*!
 * \brief LAArmadillo::arma2OiMat
 * \param result
 * \param m
 */
void LAArmadillo::arma2OiMat(OiMat &result, const arma::mat &m){
    for(int i = 0; i < m.n_rows; i++){
        for(int j = 0; j < m.n_cols; j++){
            result.setAt(i, j, m.at(i, j));
        }
    }
}
