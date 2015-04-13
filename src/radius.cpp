#include "radius.h"

/*!
 * \brief Radius::Radius
 * \param parent
 */
Radius::Radius(QObject *parent) : Element(parent){
    this->radius = 0.0;
}

/*!
 * \brief Radius::Radius
 * \param r
 * \param parent
 */
Radius::Radius(const double &r, QObject *parent) : Element(parent){
    this->radius = r;
}

/*!
 * \brief Radius::Radius
 * \param copy
 * \param parent
 */
Radius::Radius(const Radius &copy, QObject *parent) : Element(copy, parent){
    this->radius = copy.radius;
}

/*!
 * \brief Radius::operator =
 * \param copy
 * \return
 */
Radius &Radius::operator=(const Radius &copy){
    this->radius = copy.radius;
    return *this;
}

/*!
 * \brief Radius::~Radius
 */
Radius::~Radius(){

}

/*!
 * \brief Radius::setRadius
 * \param r
 */
void Radius::setRadius(const double &r){
    this->radius = r;
}

/*!
 * \brief Radius::getRadius
 * \return
 */
const double &Radius::getRadius() const{
    return this->radius;
}
