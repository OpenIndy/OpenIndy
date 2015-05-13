#include "position.h"

/*!
 * \brief Position::Position
 * \param parent
 */
Position::Position(QObject *parent) : Element(parent){
    this->xyz = OiVec(3);
    this->xyzH = OiVec(3);
    this->xyzH.add(1.0);
}

/*!
 * \brief Position::Position
 * \param v
 * \param parent
 */
Position::Position(const OiVec &v, QObject *parent) : Element(parent){
    if(v.getSize() == 3){
        this->xyz = v;
        this->xyzH = v;
        this->xyzH.add(1.0);
    }else if(v.getSize() == 4){
        this->xyz.add(v.getAt(0) / v.getAt(3));
        this->xyz.add(v.getAt(1) / v.getAt(3));
        this->xyz.add(v.getAt(2) / v.getAt(3));
        this->xyzH = v;
    }else{
        this->xyz = OiVec(3);
        this->xyzH = OiVec(3);
        this->xyzH.add(1.0);
    }
}

/*!
 * \brief Position::Position
 * \param x
 * \param y
 * \param z
 * \param parent
 */
Position::Position(const double &x, const double &y, const double &z, const double &h, QObject *parent) : Element(parent){
    this->xyz.add(x / h);
    this->xyz.add(y / h);
    this->xyz.add(z / h);
    this->xyzH.add(x);
    this->xyzH.add(y);
    this->xyzH.add(z);
    this->xyzH.add(h);
}

/*!
 * \brief Position::Position
 * \param copy
 * \param parent
 */
Position::Position(const Position &copy, QObject *parent) : Element(copy, parent){
    this->xyz = copy.xyz;
    this->xyzH = copy.xyzH;
}

/*!
 * \brief Position::operator =
 * \param copy
 * \return
 */
Position &Position::operator=(const Position &copy){

    this->xyz = copy.xyz;
    this->xyzH = copy.xyzH;

    return *this;

}

/*!
 * \brief Position::~Position
 */
Position::~Position(){

}

/*!
 * \brief Position::setVector
 * \param v
 */
void Position::setVector(const OiVec &v){
    if(v.getSize() == 3){
        this->xyz = v;
        this->xyzH.setAt(0, v.getAt(0));
        this->xyzH.setAt(1, v.getAt(1));
        this->xyzH.setAt(2, v.getAt(2));
        this->xyzH.setAt(3, 1.0);
    }else if(v.getSize() == 4){
        this->xyz.setAt(0, v.getAt(0) / v.getAt(3));
        this->xyz.setAt(1, v.getAt(1) / v.getAt(3));
        this->xyz.setAt(2, v.getAt(2) / v.getAt(3));
        this->xyzH = v;
    }
}

/*!
 * \brief Position::setVector
 * \param x
 * \param y
 * \param z
 * \param h
 */
void Position::setVector(const double &x, const double &y, const double &z, const double &h){
    this->xyz.setAt(0, x / h);
    this->xyz.setAt(1, y / h);
    this->xyz.setAt(2, z / h);
    this->xyzH.setAt(0, x);
    this->xyzH.setAt(1, y);
    this->xyzH.setAt(2, z);
    this->xyzH.setAt(3, h);
}

/*!
 * \brief Position::getVector
 * \return
 */
const OiVec &Position::getVector() const{
    return this->xyz;
}

/*!
 * \brief Position::getVectorH
 * \return
 */
const OiVec &Position::getVectorH() const{
    return this->xyzH;
}
