#include "scalarentitytemperature.h"

#include "featurewrapper.h"

/*
#include "function.h"

MeasurementConfig ScalarEntityTemperature::defaultMeasurementConfig;

ScalarEntityTemperature::ScalarEntityTemperature(bool isNominal, QObject *parent) : Geometry(isNominal, parent)
{
    //this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = false;
    this->setTemperature(20.0);
}

ScalarEntityTemperature::ScalarEntityTemperature(const ScalarEntityTemperature &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->temperature = copy.getTemperature();
    this->isSolved = copy.isSolved;
}

void ScalarEntityTemperature::recalc(){

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

QDomElement ScalarEntityTemperature::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement entityTemperature = Geometry::toOpenIndyXML(xmlDoc);

    if(entityTemperature.isNull()){
        return entityTemperature;
    }

    entityTemperature.setAttribute("type", getGeometryTypeName(eScalarEntityTemperatureGeometry));

    //add temperature
    QDomElement temperature = xmlDoc.createElement("temperature");
    if(this->getIsSolved() || this->getIsNominal()){
        temperature.setAttribute("value", this->temperature);
    }else{
        temperature.setAttribute("value", 0.0);
    }
    entityTemperature.appendChild(temperature);

    return entityTemperature;

}

bool ScalarEntityTemperature::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set circle attributes
        QDomElement temperature = xmlElem.firstChildElement("temperature");

        if(temperature.isNull() || !temperature.hasAttribute("value")){
            return false;
        }

        this->temperature = temperature.attribute("value").toDouble();

    }

    return result;

}

bool ScalarEntityTemperature::saveSimulationData()
{
    this->mySimulationData.addScalar(this->temperature);
    return true;
}

void ScalarEntityTemperature::setTemperature(double temperature){
    this->temperature = temperature;
}

double ScalarEntityTemperature::getScalar() const
{
    return this->temperature;
}

double ScalarEntityTemperature::getTemperature() const{
    return this->temperature;
}

QString ScalarEntityTemperature::getDisplayIsCommon() const{
    return QString(isCommon?"true":"false");
}

QString ScalarEntityTemperature::getDisplayIsNominal() const{
    return QString(isNominal?"true":"false");
}
QString ScalarEntityTemperature::getDisplayObs() const{
    return QString::number(this->myObservations.size());
}

QString ScalarEntityTemperature::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString ScalarEntityTemperature::getDisplayMConfig() const{
    return this->activeMeasurementConfig.getDisplayName();
}

QString ScalarEntityTemperature::getDisplayStdDev() const{

    if(this->myStatistic.isValid){
        //return QString::number(this->myStatistic.stdev*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
    }
    return "-/-";

}

QString ScalarEntityTemperature::getDisplayScalarTemperatureValue() const{
    return "";
    //return QString::number(OiUnitConverter::getTemperature(this->temperature),'f',OiUnitConverter::temperatureDigits);
}
*/


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

    //reset temperature definition if not solved
    if(!this->isSolved){
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
    if(this->getIsSolved() || this->getIsNominal()){
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

        //set circle attributes
        QDomElement temperature = xmlElem.firstChildElement("temperature");

        if(temperature.isNull() || !temperature.hasAttribute("value")){
            return false;
        }

        this->temperature = temperature.attribute("value").toDouble();

    }

    return result;

}
