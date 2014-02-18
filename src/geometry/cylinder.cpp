#include "cylinder.h"

Cylinder::Cylinder()
{
    this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

/*!
 * \brief Cylinder::Cylinder
 * \param copy
 */
Cylinder::Cylinder(const Cylinder &copy){
    this->id = copy.id;
    this->name = copy.name;
    this->isSolved = copy.isSolved;
}

void Cylinder::recalc(){

}
