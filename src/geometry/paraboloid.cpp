#include "paraboloid.h"

Paraboloid::Paraboloid()
{
    this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

/*!
 * \brief Paraboloid::Paraboloid
 * \param copy
 */
Paraboloid::Paraboloid(const Paraboloid &copy){
    this->id = copy.id;
    this->name = copy.name;
    this->isSolved = copy.isSolved;
}

void Paraboloid::recalc(){

}
