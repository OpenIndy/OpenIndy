#include "element.h"

Element::Element(QObject *parent) : QObject(parent), id(-1){

}

Element::Element(const Element &copy, QObject *parent) : QObject(parent){
    this->id = copy.id;
}

Element &Element::operator=(const Element &copy){
    this->id = copy.id;
    return *this;
}

Element::~Element(){

}

/*!
 * \brief Element::getId
 * \return
 */
int Element::getId() const{
    return this->id;
}

/*!
 * \brief Element::setId
 * \param id
 */
bool Element::setId(int id){
    if(id == -1){
        this->id = id;
        emit this->elementIdChanged();
        return true;
    }
    return false;
}



