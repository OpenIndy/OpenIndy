#include "cone.h"

/*

#include "function.h"

MeasurementConfig Cone::defaultMeasurementConfig;

Cone::Cone(bool isNominal, QObject *parent) : Geometry(isNominal, parent), xyz(4), ijk(4), alpha(0.0)
{
    //this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

Cone::Cone(const Cone &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->isSolved = copy.isSolved;
}

OiVec Cone::getXYZ() const
{
    return this->xyz;
}

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

QDomElement Cone::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement cone = Geometry::toOpenIndyXML(xmlDoc);

    if(cone.isNull()){
        return cone;
    }

    cone.setAttribute("type", getGeometryTypeName(eConeGeometry));

    return cone;

}

bool Cone::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){



    }

    return result;

}

bool Cone::saveSimulationData()
{
    return false;
}
*/

/*!
 * \brief Cone::Cone
 * \param isNominal
 * \param parent
 */
Cone::Cone(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

}

/*!
 * \brief Cone::Cone
 * \param isNominal
 * \param apex
 * \param axis
 * \param aperture
 * \param parent
 */
Cone::Cone(const bool &isNominal, const Position &apex, const Direction &axis, const double &aperture, QObject *parent) : Geometry(isNominal, parent){
    this->setCone(apex, axis, aperture);
}

/*!
 * \brief Cone::Cone
 * \param copy
 * \param parent
 */
Cone::Cone(const Cone &copy, QObject *parent) : Geometry(copy, parent){

    this->apex = copy.apex;
    this->axis = copy.axis;
    this->aperture = copy.aperture;

}

/*!
 * \brief Cone::operator =
 * \param copy
 * \return
 */
Cone &Cone::operator=(const Cone &copy){

    this->apex = copy.apex;
    this->axis = copy.axis;
    this->aperture = copy.aperture;

    return *this;

}

/*!
 * \brief Cone::~Cone
 */
Cone::~Cone(){

}

/*!
 * \brief Cone::hasDirection
 * \return
 */
bool Cone::hasDirection() const{
    return true;
}

/*!
 * \brief Cone::hasPosition
 * \return
 */
bool Cone::hasPosition() const{
    return true;
}

/*!
 * \brief Cone::getDirection
 * Returns the vector pointing from the apex inside the cone
 * \return
 */
const Direction &Cone::getDirection() const{
    return this->axis;
}

/*!
 * \brief Cone::getPosition
 * Returns the apex of the cone
 * \return
 */
const Position &Cone::getPosition() const{
    return this->apex;
}

/*!
 * \brief Cone::getAperture
 * Returns the aperture of the cone
 * \return
 */
const double &Cone::getAperture() const{
    return this->aperture;
}

/*!
 * \brief Cone::setCone
 * \param apex
 * \param axis
 * \param aperture
 */
void Cone::setCone(const Position &apex, const Direction &axis, const double &aperture){

    //set the given parameters
    this->apex = apex;
    this->axis = axis;
    this->aperture = aperture;

}

/*!
 * \brief Cone::recalc
 */
void Cone::recalc(){

    Geometry::recalc();

    //reset cone definition if not solved
    if(!this->isSolved){
        this->apex.setVector(0.0, 0.0, 0.0);
        this->axis.setVector(0.0, 0.0, 0.0);
        this->aperture = 0.0;
    }

}

/*!
 * \brief Cone::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Cone::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement cone = Geometry::toOpenIndyXML(xmlDoc);

    if(cone.isNull()){
        return cone;
    }

    cone.setAttribute("type", getGeometryTypeName(eConeGeometry));

    return cone;

}

/*!
 * \brief Cone::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Cone::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){



    }

    return result;

}
