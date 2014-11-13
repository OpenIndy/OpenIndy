#include "cylinder.h"

#include "function.h"

MeasurementConfig Cylinder::defaultMeasurementConfig;

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

/*!
 * \brief Cylinder::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Cylinder::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set cylinder attributes
        QDomElement radius = xmlElem.firstChildElement("radius");
        QDomElement directionVector = xmlElem.firstChildElement("spatialDirection");
        QDomElement axisPoint = xmlElem.firstChildElement("coordinates");

        if(radius.isNull() || directionVector.isNull() || axisPoint.isNull() || !radius.hasAttribute("value")
                || !directionVector.hasAttribute("i") || !directionVector.hasAttribute("j") || !directionVector.hasAttribute("k")
                || !axisPoint.hasAttribute("x") || !axisPoint.hasAttribute("y") || !axisPoint.hasAttribute("z")){
            return false;
        }

        this->radius = radius.attribute("value").toDouble();
        this->ijk.setAt(0, directionVector.attribute("i").toDouble());
        this->ijk.setAt(1, directionVector.attribute("j").toDouble());
        this->ijk.setAt(2, directionVector.attribute("k").toDouble());
        this->ijk.setAt(3, 1.0);
        this->xyz.setAt(0, axisPoint.attribute("x").toDouble());
        this->xyz.setAt(1, axisPoint.attribute("y").toDouble());
        this->xyz.setAt(2, axisPoint.attribute("z").toDouble());
        this->xyz.setAt(3, 1.0);

    }

    return result;

}

bool Cylinder::saveSimulationData()
{
    return false;
}
