#include "oimat.h"

#include "oivec.h"
#include "linearalgebra.h"

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
unsigned int OiMat::getRowCount() const{
    return this->values.size();
}

/*!
 * \brief OiMat::getColCount
 * \return
 */
unsigned int OiMat::getColCount() const{
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
 * \brief OiMat::setRow
 * Replace the row at index by the given row
 * \param index
 * \param row
 */
void OiMat::setRow(const int index, const OiVec &row){
    if(this->getRowCount() > index){ //if there is a row at index

        if(this->getColCount() == row.getSize()){ //if the given row has the right number of elements

            //fill vector with the given values...
            vector<double> rowVec;
            for(unsigned int i = 0; i < row.getSize(); i++){
                rowVec.push_back(row.getAt(i));
            }

            //...and replace the row of the matrix at index
            this->values.at(index) = rowVec;

        }else{
            throw logic_error("Cannot replace a row of a matrix by a given row with incompatible size");
        }

    }else{
        throw logic_error("Cannot replace a row of a matrix by a given row because of an invalid index");
    }
}

/*!
 * \brief OiMat::setCol
 * Replace the column at index by the given column
 * \param index
 * \param col
 */
void OiMat::setCol(const int index, const OiVec &col){
    if(this->getColCount() > index){ //if there is a column at index

        if(this->getRowCount() == col.getSize()){ //if the given column has the right number of elements

            for(unsigned int i = 0; i < col.getSize(); i++){
                this->values.at(i).at(index) = col.getAt(i);
            }

        }else{
            throw logic_error("Cannot replace a column of a matrix by a given column with incompatible size");
        }

    }else{
        throw logic_error("Cannot replace a column of a matrix by a given column because of an invalid index");
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
OiMat OiMat::operator+(const OiMat &m) const{
    if(this->getRowCount() == m.getRowCount() && this->getColCount() == m.getColCount()
            && this->getRowCount() > 0 && this->getColCount() > 0 ){
        OiMat result(m.getRowCount(), m.getColCount());
        OiMat::myLinearAlgebra->addIn(result, *this, m);
        return result;
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
OiMat OiMat::operator-(const OiMat &m) const{
    if( this->getRowCount() == m.getRowCount() && this->getColCount() == m.getColCount()
            && this->getRowCount() > 0 && this->getColCount() > 0 ){
        OiMat result(m.getRowCount(), m.getColCount());
        OiMat::myLinearAlgebra->substract(result, *this, m);
        return result;
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
        OiMat::myLinearAlgebra->addIn(*this, *this, m);
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
        OiMat::myLinearAlgebra->substract(*this, *this, m);
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
OiMat OiMat::operator*(const OiMat &m) const{
    if( this->getColCount() == m.getRowCount() && this->getColCount() > 0 ){
        OiMat result(this->getRowCount(), m.getColCount());
        OiMat::myLinearAlgebra->multiply(result, *this, m);
        return result;
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
OiVec OiMat::operator*(const OiVec &v) const{
    if( this->getColCount() == v.getSize() && v.getSize() > 0 ){
        OiVec result(this->getRowCount());
        OiVec::myLinearAlgebra->multiply(result, *this, v);
        return result;
    }else{
        throw logic_error("Cannot multiply a vector by a matrix with incompatible size");
        return OiVec();
    }
}

OiMat OiMat::operator*(const double value) const{
    OiMat result(this->getRowCount(), this->getColCount());
    OiMat::myLinearAlgebra->multiply(result, value, *this);
    return result;
}

/*!
 * \brief OiVec::mult
 * Multiply the vector by a scalar
 * \param value
 * \param v
 * \return
 */
OiMat OiMat::mult(const double value, const OiMat &m){
    OiMat result(m.getRowCount(), m.getColCount());
    OiMat::myLinearAlgebra->multiply(result, value, m);
    return result;
}

/*!
 * \brief OiMat::t
 * Calculate the transposed matrix
 * \return
 */
OiMat OiMat::t() const{
    if( this->getRowCount() > 0 && this->getColCount() > 0 ){
        OiMat result(this->getColCount(), this->getRowCount());
        OiMat::myLinearAlgebra->transpose(result, *this);
        return result;
    }else{
        return OiMat();
    }
}

/*!
 * \brief OiMat::inv
 * Calculate the inverse matrix
 * \return
 */
OiMat OiMat::inv() const{
    if( this->getRowCount() == this->getColCount() && this->getRowCount() > 0 ){
        OiMat result(this->getRowCount(), this->getRowCount());
        OiMat::myLinearAlgebra->invert(result, *this);
        return result;
    }else{
        throw logic_error("Cannot calculate inverse of non-square matrix");
        return OiMat();
    }
}

/*!
 * \brief OiMat::det
 * Calculate the determinant of the matrix
 * \return
 */
double OiMat::det() const{
    if( this->getRowCount() == this->getColCount() && this->getRowCount() > 0 ){
        double result = 0.0;
        OiMat::myLinearAlgebra->det(result, *this);
        return result;
    }else{
        throw logic_error("Cannot calculate determinant of non-square matrix");
        return 0.0;
    }
}

/*!
 * \brief OiMat::svd
 * Calculate singular value decomposition
 * \param u
 * \param d
 * \param v
 */
void OiMat::svd(OiMat &u, OiVec &d, OiMat &v) const{
    OiMat::myLinearAlgebra->svd(u, d, v, *this);
}

/*!
 * \brief OiMat::solve
 * Solves a linear equation of the form A*x = b (x = inv(A)*b)
 * \param x
 * \param A
 * \param b
 * \return
 */
bool OiMat::solve(OiVec &x, const OiMat &A, const OiVec &b){
    if(x.getSize() == A.getRowCount() && x.getSize() == A.getColCount()
            && x.getSize() == b.getSize() && x.getSize() > 0){
        return OiMat::myLinearAlgebra->solve(x, A, b);
    }else{
        throw logic_error("No valid linear equation system");
        return false;
    }
}

/*!
 * \brief OiMat::solve
 * Solves a linear equation of the form A*X = B (X = inv(A)*B)
 * \param X
 * \param A
 * \param B
 * \return
 */
bool OiMat::solve(OiMat &X, const OiMat &A, const OiMat &B){
    if(X.getRowCount() == X.getColCount() && X.getRowCount() == A.getRowCount()
            && X.getRowCount() == A.getColCount() && X.getRowCount() == B.getRowCount()
            && X.getRowCount() == B.getColCount()){
        return OiMat::myLinearAlgebra->solve(X, A, B);
    }else{
        throw logic_error("No valid linear equation system");
        return false;
    }
}

/*!
 * \brief OiMat::getRotationMatrix
 * Get the rotation matrix corresponding to a rotation around an arbitrary rotation axis by the given amount
 * \param angle
 * \param axis
 * \return
 */
OiMat OiMat::getRotationMatrix(double angle, OiVec axis){
    if(axis.getSize() == 3){
        OiMat result(3, 3);

        axis = axis.normalize();

        double w = qCos(angle / 2.0);
        OiVec x = qSin(angle / 2.0) * axis;

        result.setAt(0, 0, 1.0 - 2.0 * (x.getAt(1)*x.getAt(1) + x.getAt(2)*x.getAt(2)));
        result.setAt(0, 1, 2.0 * (x.getAt(0)*x.getAt(1) - w * x.getAt(2)));
        result.setAt(0, 2, 2.0 * (x.getAt(0)*x.getAt(2) + w * x.getAt(1)));
        result.setAt(1, 0, 2.0 * (x.getAt(0)*x.getAt(1) + w * x.getAt(2)));
        result.setAt(1, 1, 1.0 - 2.0 * (x.getAt(0)*x.getAt(0) + x.getAt(2)*x.getAt(2)));
        result.setAt(1, 2, 2.0 * (x.getAt(1)*x.getAt(2) - w * x.getAt(0)));
        result.setAt(2, 0, 2.0 * (x.getAt(0)*x.getAt(2) - w * x.getAt(1)));
        result.setAt(2, 1, 2.0 * (x.getAt(1)*x.getAt(2) + w * x.getAt(0)));
        result.setAt(2, 2, 1.0 - 2.0 * (x.getAt(0)*x.getAt(0) + x.getAt(1)*x.getAt(1)));

        return result;
    }else{
        throw logic_error("To set up the rotation matrix the given axis has to be of size 3");
        return OiMat();
    }
}

/*!
 * \brief OiMat::getRotationMatrix
 * Get the rotation matrix corresponding to a rotation around the X, Y or Z axis by the given amount
 * \param angle
 * \param axis
 * \return
 */
OiMat OiMat::getRotationMatrix(double angle, Rotation::RotationAxis axis){
    OiVec myAxis(3);
    switch(axis){
    case Rotation::X_AXIS:
        myAxis.setAt(0, 1.0);
        break;
    case Rotation::Y_AXIS:
        myAxis.setAt(1, 1.0);
        break;
    case Rotation::Z_AXIS:
        myAxis.setAt(2, 1.0);
        break;
    }

    return OiMat::getRotationMatrix(angle, myAxis);
}

/*!
 * \brief OiMat::getRotationMatrix
 * Get the rotation matrix corresponding to the given chain of rotations around X, Y or Z axes
 * \param rotationChain
 * \return
 */
OiMat OiMat::getRotationMatrix(RotationChain rotationChain){
    return OiMat();
}
