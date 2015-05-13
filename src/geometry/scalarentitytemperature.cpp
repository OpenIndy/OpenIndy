#include "scalarentitytemperature.h"

#include "featurewrapper.h"

/*!
 * \brief ScalarEntityTemperature::ScalarEntityTemperature
 * \param isNominal
 * \param parent
 */
ScalarEntityTemperature::ScalarEntityTemperature(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setScalarEntityTemperature(this);
    }

}

/*!
 * \brief ScalarEntityTemperature::ScalarEntityTemperature
 * \param isNominal
 * \param temperature
 * \param parent
 */
ScalarEntityTemperature::ScalarEntityTemperature(const bool &isNominal, const double &temperature, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setScalarEntityTemperature(this);
    }

    this->setTemperature(temperature);

}

/*!
 * \brief ScalarEntityTemperature::ScalarEntityTemperature
 * \param copy
 * \param parent
 */
ScalarEntityTemperature::ScalarEntityTemperature(const ScalarEntityTemperature &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setScalarEntityTemperature(this);
    }

    this->temperature = copy.temperature;

}

/*!
 * \brief ScalarEntityTemperature::operator =
 * \param copy
 * \return
 */
ScalarEntityTemperature &ScalarEntityTemperature::operator=(const ScalarEntityTemperature &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setScalarEntityTemperature(this);
    }

    this->temperature = copy.temperature;

    return *this;

}

/*!
 * \brief ScalarEntityTemperature::~ScalarEntityTemperature
 */
ScalarEntityTemperature::~ScalarEntityTemperature(){

}

/*!
 * \brief ScalarEntityTemperature::getTemperature
 * \return
 */
const double &ScalarEntityTemperature::getTemperature() const{
    return this->temperature;
}

/*!
 * \brief ScalarEntityTemperature::setTemperature
 * \param temperature
 */
void ScalarEntityTemperature::setTemperature(const double &temperature){
    this->temperature = temperature;
}

/*!
 * \brief ScalarEntityTemperature::recalc
 */
void ScalarEntityTemperature::recalc(){

    Geometry::recalc();

    //reset temperature definition if not solved and no nominal
    if(!this->isSolved && !this->isNominal){
        this->temperature = 0.0;
    }

}

/*!
 * \brief ScalarEntityTemperature::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement ScalarEntityTemperature::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement entityTemperature = Geometry::toOpenIndyXML(xmlDoc);

    if(entityTemperature.isNull()){
        return entityTemperature;
    }

    entityTemperature.setAttribute("type", getGeometryTypeName(eScalarEntityTemperatureGeometry));

    //add temperature
    QDomElement temperature = xmlDoc.createElement("temperature");
    if(this->isSolved || this->isNominal){
        temperature.setAttribute("value", this->temperature);
    }else{
        temperature.setAttribute("value", 0.0);
    }
    entityTemperature.appendChild(temperature);

    return entityTemperature;

}

/*!
 * \brief ScalarEntityTemperature::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool ScalarEntityTemperature::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set temperature attributes
        QDomElement temperature = xmlElem.firstChildElement("temperature");

        if(temperature.isNull() || !temperature.hasAttribute("value")){
            return false;
        }

        this->temperature = temperature.attribute("value").toDouble();

    }

    return result;

}

/*!
 * \brief ScalarEntityTemperature::getDisplayTemperature
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString ScalarEntityTemperature::getDisplayTemperature(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->temperature, type), 'f', digits);
    }
    return QString("");
}
