#include "direction.h"

/*!
 * \brief Direction::Direction
 * \param parent
 */
Direction::Direction(QObject *parent) : Element(parent){
    this->ijk = OiVec(3);
    this->ijkH = OiVec(3);
    this->ijkH.add(1.0);
}

/*!
 * \brief Direction::Direction
 * \param v
 * \param parent
 */
Direction::Direction(const OiVec &v, QObject *parent) : Element(parent){
    if(v.getSize() == 3){
        this->ijk = v;
        this->ijkH = v;
        this->ijkH.add(1.0);
    }else if(v.getSize() == 4){
        this->ijk.add(v.getAt(0) / v.getAt(3));
        this->ijk.add(v.getAt(1) / v.getAt(3));
        this->ijk.add(v.getAt(2) / v.getAt(3));
        this->ijkH = v;
    }else{
        this->ijk = OiVec(3);
        this->ijkH = OiVec(3);
        this->ijkH.add(1.0);
    }
}

/*!
 * \brief Direction::Direction
 * \param x
 * \param y
 * \param z
 * \param parent
 */
Direction::Direction(const double &x, const double &y, const double &z, const double &h, QObject *parent) : Element(parent){
    this->ijk.add(x / h);
    this->ijk.add(y / h);
    this->ijk.add(z / h);
    this->ijkH.add(x);
    this->ijkH.add(y);
    this->ijkH.add(z);
    this->ijkH.add(h);
}

/*!
 * \brief Direction::Direction
 * \param copy
 * \param parent
 */
Direction::Direction(const Direction &copy, QObject *parent) : Element(copy, parent){
    this->ijk = copy.ijk;
    this->ijkH = copy.ijkH;
}

/*!
 * \brief Direction::operator =
 * \param copy
 * \return
 */
Direction &Direction::operator=(const Direction &copy){

    this->ijk = copy.ijk;
    this->ijkH = copy.ijkH;

    return *this;

}

/*!
 * \brief Direction::~Direction
 */
Direction::~Direction(){

}

/*!
 * \brief Direction::setVector
 * \param x
 * \param y
 * \param z
 */
void Direction::setVector(const double &x, const double &y, const double &z, const double &h){
    this->ijk.setAt(0, x / h);
    this->ijk.setAt(1, y / h);
    this->ijk.setAt(2, z / h);
    this->ijkH.setAt(0, x);
    this->ijkH.setAt(1, y);
    this->ijkH.setAt(2, z);
    this->ijkH.setAt(3, h);
}

/*!
 * \brief Direction::setVector
 * \param v
 */
void Direction::setVector(const OiVec &v){
    if(v.getSize() == 3){
        this->ijk = v;
        this->ijkH.setAt(0, v.getAt(0));
        this->ijkH.setAt(1, v.getAt(1));
        this->ijkH.setAt(2, v.getAt(2));
        this->ijkH.setAt(3, 1.0);
    }else if(v.getSize() == 4){
        this->ijk.setAt(0, v.getAt(0) / v.getAt(3));
        this->ijk.setAt(1, v.getAt(1) / v.getAt(3));
        this->ijk.setAt(2, v.getAt(2) / v.getAt(3));
        this->ijkH = v;
    }
}

/*!
 * \brief Direction::getVector
 * \return
 */
const OiVec &Direction::getVector() const{
    return ijk;
}

/*!
 * \brief Direction::getVectorH
 * \return
 */
const OiVec &Direction::getVectorH() const{
    return ijkH;
}
