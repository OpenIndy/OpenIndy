#include "ellipsoid.h"

Ellipsoid::Ellipsoid()
{
    this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

/*!
 * \brief Ellipsoid::Ellipsoid
 * \param copy
 */
Ellipsoid::Ellipsoid(const Ellipsoid &copy){
    this->id = copy.id;
    this->name = copy.name;
    this->isSolved = copy.isSolved;
}

void Ellipsoid::recalc(){

}

bool Ellipsoid::toOpenIndyXML(QXmlStreamWriter &stream){

    return false;
}

bool Ellipsoid::fromOpenIndyXML(QXmlStreamReader &xml){

    return false;
}
