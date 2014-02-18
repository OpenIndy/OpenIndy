#include "oimat.h"

#include "oivec.h"
#include "linearalgebra.h"

/*#ifdef OI_MAIN_PROGRAM
    #include "types.h"
    LinearAlgebra* OiMat::myLinearAlgebra = new LAAlgorithm();
#else
    LinearAlgebra* OiMat::myLinearAlgebra = NULL;
#endif*/

LinearAlgebra* OiMat::myLinearAlgebra = NULL;

OiMat::OiMat()
{
    this->values.clear();
}

OiMat::OiMat(const int rows, const int cols){
    this->values.clear();
    for(int i = 0; i < rows; i++){
        vector<double> row;
        for(int j = 0; j < cols; j++){
            row.push_back(0.0);
        }
        this->values.push_back(row);
    }
}

/*!
 * \brief OiMat::getAt
 * Get the value at the specified position
 * \param row
 * \param col
 * \return
 */
double OiMat::getAt(const int row, const int col) const{
    if(this->values.size() > row){
        if(this->values.at(row).size() > col){
            return this->values.at(row).at(col);
        }
    }
    throw runtime_error("Size of matrix less than requested position");
}

/*!
 * \brief OiMat::getRowCount
 * \return
 */
int OiMat::getRowCount() const{
    return this->values.size();
}

/*!
 * \brief OiMat::getColCount
 * \return
 */
int OiMat::getColCount() const{
    if(this->values.size() > 0){
        return this->values.at(0).size();
    }else{
        return 0;
    }
}

/*!
 * \brief OiMat::getRow
 * Get row vector at the specified position
 * \param row
 * \return
 */
OiVec OiMat::getRow(const int row) const{
    OiVec result;
    if(this->values.size() > row){
        for(int i = 0; i < this->values.at(row).size(); i++){
            result.add(this->values.at(row).at(i));
        }
    }else{
        throw runtime_error("Size of matrix less than requested position");
    }
    return result;
}

/*!
 * \brief OiMat::getCol
 * Get column vector at the specified position
 * \param col
 * \return
 */
OiVec OiMat::getCol(const int col) const{
    OiVec result;
    if(this->values.size() > 0 && this->values.at(0).size() > col){
        for(int i = 0; i < this->values.size(); i++){
            result.add(this->values.at(i).at(col));
        }
    }else{
        throw runtime_error("Size of matrix less than requested position");
    }
    return result;
}

/*!
 * \brief OiMat::set
 * Set this matrix to m
 * \param m
 */
void OiMat::replace(const OiMat &m){
    this->values.clear();
    for(int i = 0; i < m.getRowCount(); i++){
        vector<double> row;
        for(int j = 0; j < m.getColCount(); j++){
            row.push_back(m.getAt(i, j));
        }
        this->values.push_back(row);
    }
}

/*!
 * \brief OiMat::setAt
 * Set the matrix element at the specified position
 * \param row
 * \param col
 * \param value
 */
void OiMat::setAt(const int row, const int col,const double value){
    if(this->values.size() > row){
        if(this->values.at(row).size() > col){
            this->values.at(row).at(col) = value;
            return;
        }
    }
    throw runtime_error("Size of matrix less than requested position");
}

/*!
 * \brief OiMat::diag
 * Set this matrix to diagonal matrix with diagVec holding the diagonal elements
 * \param diagVec
 */
void OiMat::diag(vector<double> diagVec){
    if( this->values.size() > 0 && this->values.size() == diagVec.size() && this->values.at(0).size() == diagVec.size() ){
        for(int i = 0; i < diagVec.size(); i++){
            for(int j = 0; j < diagVec.size(); j++){
                if(i == j){
                    this->setAt(i, j, diagVec.at(i));
                }else{
                    this->setAt(i, j, 0.0);
                }
            }
        }
    }else{
        throw logic_error("Size of diagVec incompatible to this matrix");
    }
}

/*!
 * \brief OiMat::operator =
 * Assign a matrix to another
 * \param m
 * \return
 */
OiMat& OiMat::operator =(const OiMat &m){
    if(this->getRowCount() == m.getRowCount() && this->getColCount() == m.getColCount()){ //sizes of matrices need to be equal...
        for(int i = 0; i < m.getRowCount(); i++){
            for(int j = 0; j < m.getColCount(); j++){
                this->setAt(i, j, m.getAt(i, j));
            }
        }
        return *this;
    }else if(this->getRowCount() == 0 && this->getColCount() == 0){ //...or this matrix has size 0 (is undefined)
        this->replace(m);
        return *this;
    }else{
        throw logic_error("A matrix cannot be assigned to another matrix with a different size");
        return *this;
    }
}

/*!
 * \brief OiMat::operator +
 * Add m to this matrix
 * \param v
 * \return
 */
OiMat OiMat::operator+(const OiMat &m){
    if(this->getRowCount() == m.getRowCount() && this->getColCount() == m.getColCount()
            && this->getRowCount() > 0 && this->getColCount() > 0 ){
        return OiMat::myLinearAlgebra->addIn(*this, m);
    }else{
        throw logic_error("Cannot add two matrices of different size");
        return OiMat();
    }
}

/*!
 * \brief OiMat::operator -
 * Substract m from this matrix
 * \param v
 * \return
 */
OiMat OiMat::operator-(const OiMat &m){
    if( this->getRowCount() == m.getRowCount() && this->getColCount() == m.getColCount()
            && this->getRowCount() > 0 && this->getColCount() > 0 ){
        return OiMat::myLinearAlgebra->substract(*this, m);
    }else{
        throw logic_error("Cannot substract two matrices of different size");
        return OiMat();
    }
}

/*!
 * \brief OiMat::operator +=
 * Add m to this matrix
 * \param m
 * \return
 */
OiMat& OiMat::operator+=(const OiMat &m){
    if(this->getRowCount() == m.getRowCount() && this->getColCount() == m.getColCount()
            && this->getRowCount() > 0 && this->getColCount() > 0 ){
        OiMat result = OiMat::myLinearAlgebra->addIn(*this, m);
        *this = result;
        return *this;
    }else{
        throw logic_error("Cannot add two matrices of different size");
        return *this;
    }
}

/*!
 * \brief OiMat::operator +=
 * Substract m from this matrix
 * \param m
 * \return
 */
OiMat& OiMat::operator-=(const OiMat &m){
    if( this->getRowCount() == m.getRowCount() && this->getColCount() == m.getColCount()
            && this->getRowCount() > 0 && this->getColCount() > 0 ){
        OiMat result = OiMat::myLinearAlgebra->substract(*this, m);
        *this = result;
        return *this;
    }else{
        throw logic_error("Cannot substract two matrices of different size");
        return *this;
    }
}

/*!
 * \brief OiMat::operator *
 * Multiply m by this matrix
 * \param v
 * \return
 */
OiMat OiMat::operator*(const OiMat &m){
    if( this->getColCount() == m.getRowCount() && this->getColCount() > 0 ){
        return OiMat::myLinearAlgebra->multiply(*this, m);
    }else{
        throw logic_error("Cannot multiply two matrices of incompatible size");
        return OiMat();
    }
}

/*!
 * \brief OiMat::operator *
 * Multiply v by this matrix
 * \param v
 * \return
 */
OiVec OiMat::operator*(const OiVec &v){
    if( this->getColCount() == v.getSize() && v.getSize() > 0 ){
        return OiMat::myLinearAlgebra->multiply(*this, v);
    }else{
        throw logic_error("Cannot multiply a vector by a matrix with incompatible size");
        return OiVec();
    }
}

OiMat OiMat::operator*(const double value){
    return OiMat::myLinearAlgebra->multiply(value, *this);;
}

/*!
 * \brief OiVec::mult
 * Multiply the vector by a scalar
 * \param value
 * \param v
 * \return
 */
OiMat OiMat::mult(const double value, const OiMat &m){
    return OiMat::myLinearAlgebra->multiply(value, m);
}

/*!
 * \brief OiMat::t
 * Calculate the transposed matrix
 * \return
 */
OiMat OiMat::t(){
    if( this->getRowCount() > 0 && this->getColCount() > 0 ){
        return OiMat::myLinearAlgebra->transpose(*this);
    }
}

/*!
 * \brief OiMat::inv
 * Calculate the inverse matrix
 * \return
 */
OiMat OiMat::inv(){
    if( this->getRowCount() == this->getColCount() && this->getRowCount() > 0 ){
        return OiMat::myLinearAlgebra->invert(*this);
    }else{
        throw logic_error("Cannot calculate inverse of non-square matrix");
        return OiMat();
    }
}

/*!
 * \brief OiMat::svd
 * Calculate singular value decomposition
 * \param u
 * \param d
 * \param v
 */
void OiMat::svd(OiMat &u, OiVec &d, OiMat &v){
    OiMat::myLinearAlgebra->svd(u, d, v, *this);
}
