#include "scalarentityangle.h"

#include "featurewrapper.h"

/*!
 * \brief ScalarEntityAngle::ScalarEntityAngle
 * \param isNominal
 * \param parent
 */
ScalarEntityAngle::ScalarEntityAngle(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setScalarEntityAngle(this);
    }

}

/*!
 * \brief ScalarEntityAngle::ScalarEntityAngle
 * \param isNominal
 * \param angle
 * \param parent
 */
ScalarEntityAngle::ScalarEntityAngle(const bool &isNominal, const double &angle, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setScalarEntityAngle(this);
    }

    this->setAngle(angle);

}

/*!
 * \brief ScalarEntityAngle::ScalarEntityAngle
 * \param copy
 * \param parent
 */
ScalarEntityAngle::ScalarEntityAngle(const ScalarEntityAngle &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setScalarEntityAngle(this);
    }

    this->angle = copy.angle;

}

/*!
 * \brief ScalarEntityAngle::operator =
 * \param copy
 * \return
 */
ScalarEntityAngle &ScalarEntityAngle::operator=(const ScalarEntityAngle &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setScalarEntityAngle(this);
    }

    this->angle = copy.angle;

    return *this;

}

/*!
 * \brief ScalarEntityAngle::~ScalarEntityAngle
 */
ScalarEntityAngle::~ScalarEntityAngle(){

}

/*!
 * \brief ScalarEntityAngle::getAngle
 * \return
 */
const double &ScalarEntityAngle::getAngle() const{
    return this->angle;
}

/*!
 * \brief ScalarEntityAngle::setAngle
 * \param angle
 */
void ScalarEntityAngle::setAngle(const double &angle){
    this->angle = angle;
}

/*!
 * \brief ScalarEntityAngle::recalc
 */
void ScalarEntityAngle::recalc(){

    Geometry::recalc();

    //reset angle definition if not solved and no nominal
    if(!this->isSolved && !this->isNominal){
        this->angle = 0.0;
    }

}

/*!
 * \brief ScalarEntityAngle::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement ScalarEntityAngle::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement entityAngle = Geometry::toOpenIndyXML(xmlDoc);

    if(entityAngle.isNull()){
        return entityAngle;
    }

    entityAngle.setAttribute("type", getGeometryTypeName(eScalarEntityAngleGeometry));

    //add angle
    QDomElement angle = xmlDoc.createElement("angle");
    if(this->isSolved || this->isNominal){
        angle.setAttribute("value", this->angle);
    }else{
        angle.setAttribute("value", 0.0);
    }
    entityAngle.appendChild(angle);

    return entityAngle;

}

/*!
 * \brief ScalarEntityAngle::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool ScalarEntityAngle::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set angle attributes
        QDomElement angle = xmlElem.firstChildElement("angle");

        if(angle.isNull() || !angle.hasAttribute("value")){
            return false;
        }

        this->angle = angle.attribute("value").toDouble();

    }

    return result;

}

/*!
 * \brief ScalarEntityAngle::getDisplayAngle
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString ScalarEntityAngle::getDisplayAngle(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->angle, type), 'f', digits);
    }
    return QString("");
}
