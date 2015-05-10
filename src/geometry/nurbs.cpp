#include "nurbs.h"

#include "featurewrapper.h"

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
