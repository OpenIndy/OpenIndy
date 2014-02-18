#include "nurbs.h"

Nurbs::Nurbs()
{
    this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

/*!
 * \brief Nurbs::Nurbs
 * \param copy
 */
Nurbs::Nurbs(const Nurbs &copy){
    this->id = copy.id;
    this->name = copy.name;
    this->isSolved = copy.isSolved;
}

void Nurbs::recalc(){

}
