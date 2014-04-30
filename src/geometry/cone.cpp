#include "cone.h"

#include "function.h"

Cone::Cone() : xyz(4), ijk(4),alpha(0.0)
{
    this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

/*!
 * \brief Cone::Cone
 * \param copy
 */
Cone::Cone(const Cone &copy){
    this->id = copy.id;
    this->name = copy.name;
    this->isSolved = copy.isSolved;
}

void Cone::recalc(){
    /*
     * isDefined -> becomes true as soon as the first function of a feature has been executed, which defines the feature
     * isSolved -> is true as long as there isn't any function which cannot be successfully executed
     */
    bool isDefined = false;
    foreach(Function *f, this->functionList){
        if(!isDefined){
            this->isSolved = f->exec(*this);
            isDefined = true;
        }else if(this->isSolved){
            this->isSolved = f->exec(*this);
        }
    }
    //if no function is set this feature cannot be solved and its coordinates are reset
    if(this->functionList.size() == 0 && this->isNominal == false){
        this->isSolved = false;
        this->xyz = OiVec(4);
        this->ijk = OiVec(4);
        this->alpha = 0.0;
    }
}

bool Cone::toOpenIndyXML(QXmlStreamWriter &stream){

    return false;
}

ElementDependencies Cone::fromOpenIndyXML(QXmlStreamReader &xml){

    ElementDependencies dependencies;

    return dependencies;
}
