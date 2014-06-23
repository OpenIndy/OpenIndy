#include "cone.h"

#include "function.h"

Cone::Cone(bool isNominal, QObject *parent) : Geometry(isNominal, parent), xyz(4), ijk(4), alpha(0.0)
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
Cone::Cone(const Cone &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->isSolved = copy.isSolved;
}

/*!
 * \brief Cone::getXYZ returns xyz as double values in vector
 * \return
 */
OiVec Cone::getXYZ() const
{
    return this->xyz;
}

/*!
 * \brief Cone::getIJK returns ijk vector
 * \return
 */
OiVec Cone::getIJK() const
{
    return this->ijk;
}

void Cone::recalc(){

    if(this->functionList.size() > 0){

        bool solved = true;
        foreach(Function *f, this->functionList){

            //execute the function if it exists and if the last function was executed successfully
            if(f != NULL && solved == true){
                solved = f->exec(*this);
            }

        }
        this->setIsSolved(solved);

    }else if(this->isNominal == false){

        this->xyz = OiVec(4);
        this->ijk = OiVec(4);
        this->alpha = 0.0;
        this->setIsSolved(false);

    }

}

bool Cone::toOpenIndyXML(QXmlStreamWriter &stream){

    return false;
}

ElementDependencies Cone::fromOpenIndyXML(QXmlStreamReader &xml){

    ElementDependencies dependencies;

    return dependencies;
}
