#include "cylinder.h"

#include "function.h"

Cylinder::Cylinder(bool isNominal, QObject *parent) : Geometry(isNominal, parent), xyz(4),ijk(4),radius(0.0)
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
Cylinder::Cylinder(const Cylinder &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->isSolved = copy.isSolved;
}

/*!
 * \brief Cylinder::getXYZ returns xyz vector
 * \return
 */
OiVec Cylinder::getXYZ() const
{
    return this->xyz;
}

/*!
 * \brief Cylinder::getIJK returns ijk vector
 * \return
 */
OiVec Cylinder::getIJK() const
{
    return this->ijk;
}

void Cylinder::recalc(){

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
        this->radius = 0.0;
        this->setIsSolved(false);

    }

}

/*!
 * \brief toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Cylinder::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement cylinder = Geometry::toOpenIndyXML(xmlDoc);

    if(cylinder.isNull()){
        return cylinder;
    }

    cylinder.setAttribute("type", Configuration::sCylinder);

    return cylinder;

}

ElementDependencies Cylinder::fromOpenIndyXML(QXmlStreamReader &xml){

    ElementDependencies dependencies;

    return dependencies;
}

bool Cylinder::saveSimulationData()
{
    return false;
}
