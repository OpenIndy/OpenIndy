#include "hyperboloid.h"

Hyperboloid::Hyperboloid()
{
    this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

/*!
 * \brief Hyperboloid::Hyperboloid
 * \param copy
 */
Hyperboloid::Hyperboloid(const Hyperboloid &copy){
    this->id = copy.id;
    this->name = copy.name;
    this->isSolved = copy.isSolved;
}

void Hyperboloid::recalc(){

}
