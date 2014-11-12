#include "ellipsoid.h"

#include "function.h"

Ellipsoid::Ellipsoid(bool isNominal, QObject *parent) : Geometry(isNominal, parent), xyz(4),a(0.0),b(0.0),c(0.0)
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
Ellipsoid::Ellipsoid(const Ellipsoid &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->isSolved = copy.isSolved;
}

/*!
 * \brief Ellipsoid::getXYZ returns the xyz vector
 * \return
 */
OiVec Ellipsoid::getXYZ() const
{
    return this->xyz;
}

void Ellipsoid::recalc(){

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
        this->a = 0.0;
        this->b = 0.0;
        this->c = 0.0;
        this->setIsSolved(false);

    }

}

/*!
 * \brief Ellipsoid::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Ellipsoid::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement ellipsoid = Geometry::toOpenIndyXML(xmlDoc);

    if(ellipsoid.isNull()){
        return ellipsoid;
    }

    ellipsoid.setAttribute("type", Configuration::sEllipsoid);

    return ellipsoid;

}

/*!
 * \brief Ellipsoid::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Ellipsoid::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){



    }

    return result;

}

bool Ellipsoid::saveSimulationData()
{
    return false;
}
