#include "nurbs.h"

#include "featurewrapper.h"
/*
#include "function.h"

MeasurementConfig Nurbs::defaultMeasurementConfig;

Nurbs::Nurbs(bool isNominal, QObject *parent) : Geometry(isNominal, parent)
{
    //this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

Nurbs::Nurbs(const Nurbs &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->isSolved = copy.isSolved;
}

void Nurbs::recalc(){

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

QDomElement Nurbs::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement nurbs = Geometry::toOpenIndyXML(xmlDoc);

    if(nurbs.isNull()){
        return nurbs;
    }

    nurbs.setAttribute("type", getGeometryTypeName(eNurbsGeometry));

    return nurbs;

}

bool Nurbs::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){



    }

    return result;

}

bool Nurbs::saveSimulationData()
{
    return false;
}
*/


/*!
 * \brief Nurbs::Nurbs
 * \param isNominal
 * \param parent
 */
Nurbs::Nurbs(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setNurbs(this);
    }

}

/*!
 * \brief Nurbs::Nurbs
 * \param copy
 * \param parent
 */
Nurbs::Nurbs(const Nurbs &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setNurbs(this);
    }

}

/*!
 * \brief Nurbs::operator =
 * \param copy
 * \return
 */
Nurbs &Nurbs::operator=(const Nurbs &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setNurbs(this);
    }

    return *this;

}

/*!
 * \brief Nurbs::~Nurbs
 */
Nurbs::~Nurbs(){

}

/*!
 * \brief Nurbs::recalc
 */
void Nurbs::recalc(){

    Geometry::recalc();

}

/*!
 * \brief Nurbs::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Nurbs::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement nurbs = Geometry::toOpenIndyXML(xmlDoc);

    if(nurbs.isNull()){
        return nurbs;
    }

    nurbs.setAttribute("type", getGeometryTypeName(eNurbsGeometry));

    return nurbs;

}

/*!
 * \brief Nurbs::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Nurbs::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){



    }

    return result;

}
