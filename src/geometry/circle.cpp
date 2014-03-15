#include "circle.h"

Circle::Circle()
{
    this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

/*!
 * \brief Circle::Circle
 * \param copy
 */
Circle::Circle(const Circle &copy){
    this->id = copy.id;
    this->name = copy.name;
    this->isSolved = copy.isSolved;
}

void Circle::recalc(){

}

bool Circle::toOpenIndyXML(QXmlStreamWriter &stream){

    return false;
}

bool Circle::fromOpenIndyXML(QXmlStreamReader &xml){

    return false;
}
