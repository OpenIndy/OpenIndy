#include "oivec.h"

#include "oimat.h"
#include "linearalgebra.h"

LinearAlgebra* OiVec::myLinearAlgebra = NULL;

OiVec::OiVec()
{
    this->values.clear();
}

OiVec::OiVec(const int size){
    this->values.clear();
    for(int i = 0; i < size; i++){
        this->values.push_back(0.0);
    }
}

OiVec::~OiVec(){

}

/*!
 * \brief OiVec::getAt
 * Get The value at the specified position
 * \param pos
 * \return
 */
double OiVec::getAt(int pos) const{
    if(this->values.size() > pos){
        return this->values.at(pos);
    }else{
        throw runtime_error("Size of vector less than requested position");
        return -1.0;
    }
}

/*!
 * \brief OiVec::getSize
 * Returns the size of the vector
 * \return
 */
int OiVec::getSize() const{
    return this->values.size();
}

/*!
 * \brief OiVec::set
 * Replace the vector by another
 * \param v
 */
void OiVec::set(vector<double> v){
    this->values = v;
}

/*!
 * \brief OiVec::setAt
 * Set the vector element at the specified position
 * \param pos
 * \param value
 */
void OiVec::setAt(int pos, double value){
    if(this->values.size() > pos){
        this->values.at(pos) = value;
    }else{
        throw runtime_error("Size of vector less than requested position");
    }
}

/*!
 * \brief OiVec::add
 * Add an element at the end of the vector
 * \param value
 */
void OiVec::add(double value){
    this->values.push_back(value);
}

/*!
 * \brief OiVec::replace
 */
void OiVec::replace(const OiVec &v){
    this->values.clear();
    for(int i = 0; i < v.getSize(); i++){
        this->add(v.getAt(i));
    }
}

/*!
 * \brief OiVec::operator =
 * Assign a vector to another
 * \param v
 * \return
 */
OiVec& OiVec::operator =(const OiVec &v){
    if(this->getSize() == v.getSize()){ //sizes of vectors need to be equal...
        for(int i = 0; i < this->getSize(); i++){
            this->setAt(i, v.getAt(i));
        }
        return *this;
    }else if(this->getSize() == 0){ //...or this vector has size 0 (is undefined)
        for(int i = 0; i < v.getSize(); i++){
            this->add(v.getAt(i));
        }
        return *this;
    }else{
        throw logic_error("A vector cannot be assigned to another vector with a different size");
        return *this;
    }
}

/*!
 * \brief OiVec::operator +
 * Add v to this vector
 * \param v
 * \return
 */
OiVec OiVec::operator+(const OiVec &v) const{
    if(this->getSize() == v.getSize()){
        return OiVec::myLinearAlgebra->addIn(*this, v);
    }else{
        throw logic_error("Cannot add two vectors of different size");
        return OiVec();
    }
}

/*!
 * \brief OiVec::operator -
 * Substract v from this vector
 * \param v
 * \return
 */
OiVec OiVec::operator-(const OiVec &v) const{
    if(this->getSize() == v.getSize()){
        return OiVec::myLinearAlgebra->substract(*this, v);
    }else{
        throw logic_error("Cannot substract two vectors of different size");
        return OiVec();
    }
}

/*!
 * \brief OiVec::operator +=
 * \param v
 * \return
 */
OiVec& OiVec::operator +=(const OiVec &v){
    if(this->getSize() == v.getSize()){
        OiVec result = OiVec::myLinearAlgebra->addIn(*this, v);
        *this = result;
        return *this;
    }else{
        throw logic_error("Cannot add two vectors of different size");
        return *this;
    }
}

/*!
 * \brief OiVec::operator -=
 * \param v
 * \return
 */
OiVec& OiVec::operator -=(const OiVec &v){
    if(this->getSize() == v.getSize()){
        OiVec result = OiVec::myLinearAlgebra->substract(*this, v);
        *this = result;
        return *this;
    }else{
        throw logic_error("Cannot substract two vectors of different size");
        return *this;
    }
}

/*!
 * \brief OiVec::normalize
 * \return
 */
OiVec OiVec::normalize() const{
    OiVec n0(this->getSize());
    double l = 0.0;
    for(int i = 0; i < this->getSize(); i++){
        l += this->getAt(i) * this->getAt(i);
    }
    if(l > 0.0){
        l = qSqrt(l);
        for(int i = 0; i < this->getSize(); i++){
            n0.setAt(i, this->getAt(i) / l);
        }
    }
    return n0;
}

/*!
 * \brief OiVec::t
 * Calculate the transposed of this vector which is a matrix
 * \return
 */
OiMat OiVec::t() const{
    if(this->getSize() > 0){
        OiMat result(1, this->getSize());
        for(int i = 0; i < this->getSize(); i++){
            result.setAt(0, i, this->getAt(i));
        }
        return result;
    }
    return OiMat();

}

/*!
 * \brief OiVec::cross
 * Calculate cross product (only if a and b are 3 dimensional vectors)
 * \param a
 * \param b
 * \return
 */
OiVec OiVec::cross(OiVec a, OiVec b){
    if(a.getSize() == b.getSize() && a.getSize() == 3){
        return OiVec::myLinearAlgebra->cross(a, b);
    }else{
        throw logic_error("Cannot calculate cross product of non - 3 dimesnional vectors");
        return OiVec();
    }
}

/*!
 * \brief OiVec::dot
 * Calculate scalar product (only if a and b are of same size)
 * \param a
 * \param b
 * \return
 */
double OiVec::dot(OiVec a, OiVec b){
    if(a.getSize() == b.getSize() && a.getSize() > 0){
        return OiVec::myLinearAlgebra->dot(a, b);
    }else{
        throw logic_error("Cannot calculate scalar product of vectors with incompatible size");
        return -1.0;
    }
}

/*!
 * \brief OiVec::mult
 * Multiply the vector by a scalar
 * \param value
 * \param v
 * \return
 */
OiVec OiVec::mult(const double value, const OiVec &v){
    return OiVec::myLinearAlgebra->multiply(value, v);
}
