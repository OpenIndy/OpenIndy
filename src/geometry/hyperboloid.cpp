#include "hyperboloid.h"

#include "featurewrapper.h"
/*
#include "function.h"

MeasurementConfig Hyperboloid::defaultMeasurementConfig;

Hyperboloid::Hyperboloid(bool isNominal, QObject *parent) : Geometry(isNominal, parent)
{
    //this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

Hyperboloid::Hyperboloid(const Hyperboloid &copy) : Geometry(copy.isNominal) {
    this->id = copy.id;
    this->name = copy.name;
    this->isSolved = copy.isSolved;
}

void Hyperboloid::recalc(){

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

        this->setIsSolved(false);

    }

}

QDomElement Hyperboloid::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement hyperboloid = Geometry::toOpenIndyXML(xmlDoc);

    if(hyperboloid.isNull()){
        return hyperboloid;
    }

    hyperboloid.setAttribute("type", getGeometryTypeName(eHyperboloidGeometry));

    return hyperboloid;

}

bool Hyperboloid::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){



    }

    return result;

}

bool Hyperboloid::saveSimulationData()
{
    return false;
}
*/


/*!
 * \brief Hyperboloid::Hyperboloid
 * \param isNominal
 * \param parent
 */
Hyperboloid::Hyperboloid(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setHyperboloid(this);
    }

}

/*!
 * \brief Hyperboloid::Hyperboloid
 * \param isNominal
 * \param center
 * \param axis
 * \param a
 * \param c
 * \param parent
 */
Hyperboloid::Hyperboloid(const bool &isNominal, const Position &center, const Direction &axis, const double &a, const double &c, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setHyperboloid(this);
    }

    this->setHyperboloid(center, axis, a, c);

}

/*!
 * \brief Hyperboloid::Hyperboloid
 * \param copy
 * \param parent
 */
Hyperboloid::Hyperboloid(const Hyperboloid &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setHyperboloid(this);
    }

    this->center = copy.center;
    this->axis = copy.axis;
    this->a = copy.a;
    this->c = copy.c;

}

/*!
 * \brief Hyperboloid::operator =
 * \param copy
 * \return
 */
Hyperboloid &Hyperboloid::operator=(const Hyperboloid &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setHyperboloid(this);
    }

    this->center = copy.center;
    this->axis = copy.axis;
    this->a = copy.a;
    this->c = copy.c;

    return *this;

}

/*!
 * \brief Hyperboloid::~Hyperboloid
 */
Hyperboloid::~Hyperboloid(){

}

/*!
 * \brief Hyperboloid::hasDirection
 * \return
 */
bool Hyperboloid::hasDirection() const{
    return true;
}

/*!
 * \brief Hyperboloid::hasPosition
 * \return
 */
bool Hyperboloid::hasPosition() const{
    return true;
}

/*!
 * \brief Hyperboloid::getDirection
 * Returns the rotation axis of the hyperboloid
 * \return
 */
const Direction &Hyperboloid::getDirection() const{
    return this->axis;
}

/*!
 * \brief Hyperboloid::getPosition
 * Returns the center of the hyperboloid
 * \return
 */
const Position &Hyperboloid::getPosition() const{
    return this->center;
}

/*!
 * \brief Hyperboloid::getA
 * \return
 */
const double &Hyperboloid::getA() const{
    return this->a;
}

/*!
 * \brief Hyperboloid::getC
 * \return
 */
const double &Hyperboloid::getC() const{
    return this->c;
}

/*!
 * \brief Hyperboloid::setHyperboloid
 * \param center
 * \param axis
 * \param a
 * \param c
 */
void Hyperboloid::setHyperboloid(const Position &center, const Direction &axis, const double &a, const double &c){

    //set the given parameters
    this->center = center;
    this->axis = axis;
    this->a = a;
    this->c = c;

}

/*!
 * \brief Hyperboloid::recalc
 */
void Hyperboloid::recalc(){

    Geometry::recalc();

    //reset hyperboloid definition if not solved
    if(!this->isSolved){
        this->center.setVector(0.0, 0.0, 0.0);
        this->axis.setVector(0.0, 0.0, 0.0);
        this->a = 0.0;
        this->c = 0.0;
    }

}

/*!
 * \brief Hyperboloid::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Hyperboloid::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement hyperboloid = Geometry::toOpenIndyXML(xmlDoc);

    if(hyperboloid.isNull()){
        return hyperboloid;
    }

    hyperboloid.setAttribute("type", getGeometryTypeName(eHyperboloidGeometry));

    return hyperboloid;

}

/*!
 * \brief Hyperboloid::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Hyperboloid::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){



    }

    return result;

}
