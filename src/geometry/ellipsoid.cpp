#include "ellipsoid.h"

#include "function.h"

Ellipsoid::Ellipsoid() : xyz(4),a(0.0),b(0.0),c(0.0)
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

/*!
 * \brief Ellipsoid::getXYZ returns the xyz vector
 * \return
 */
OiVec *Ellipsoid::getXYZ()
{
    OiVec *xyz = &this->xyz;
    return xyz;
}

void Ellipsoid::recalc(){
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
        this->a = 0.0;
        this->b = 0.0;
        this->c = 0.0;
    }
}

bool Ellipsoid::toOpenIndyXML(QXmlStreamWriter &stream){

    return false;
}

ElementDependencies Ellipsoid::fromOpenIndyXML(QXmlStreamReader &xml){

    ElementDependencies dependencies;

    return dependencies;
}
