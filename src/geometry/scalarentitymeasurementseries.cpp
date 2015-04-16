#include "scalarentitymeasurementseries.h"

#include "featurewrapper.h"
/*
#include "function.h"

MeasurementConfig ScalarEntityMeasurementSeries::defaultMeasurementConfig;

ScalarEntityMeasurementSeries::ScalarEntityMeasurementSeries(bool isNominal, QObject *parent) : Geometry(isNominal, parent)
{
    //this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = false;
    this->seriesValue = 0.0;
}

ScalarEntityMeasurementSeries::ScalarEntityMeasurementSeries(const ScalarEntityMeasurementSeries &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->seriesValue = copy.getSeriesValue();
    this->isSolved = copy.isSolved;
}

void ScalarEntityMeasurementSeries::recalc(){


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

QDomElement ScalarEntityMeasurementSeries::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement entityMeasurementSeries = Geometry::toOpenIndyXML(xmlDoc);

    if(entityMeasurementSeries.isNull()){
        return entityMeasurementSeries;
    }

    entityMeasurementSeries.setAttribute("type", getGeometryTypeName(eScalarEntityMeasurementSeriesGeometry));

    //add series value
    QDomElement seriesValue = xmlDoc.createElement("seriesValue");
    if(this->getIsSolved() || this->getIsNominal()){
        seriesValue.setAttribute("value", this->seriesValue);
    }else{
        seriesValue.setAttribute("value", 0.0);
    }
    entityMeasurementSeries.appendChild(seriesValue);

    return entityMeasurementSeries;

}

bool ScalarEntityMeasurementSeries::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set circle attributes
        QDomElement seriesValue = xmlElem.firstChildElement("seriesValue");

        if(seriesValue.isNull() || !seriesValue.hasAttribute("value")){
            return false;
        }

        this->seriesValue = seriesValue.attribute("value").toDouble();

    }

    return result;

}

bool ScalarEntityMeasurementSeries::saveSimulationData()
{
    this->mySimulationData.addScalar(this->seriesValue);
    return true;
}

double ScalarEntityMeasurementSeries::getSeriesValue() const{
    return this->seriesValue;
}

void ScalarEntityMeasurementSeries::setSeriesValue(double value){
    this->seriesValue = value;
}

double ScalarEntityMeasurementSeries::getScalar() const
{
    return this->seriesValue;
}

QString ScalarEntityMeasurementSeries::getDisplayIsCommon() const{
    return QString(isCommon?"true":"false");
}

QString ScalarEntityMeasurementSeries::getDisplayIsNominal() const{
    return QString(isNominal?"true":"false");
}
QString ScalarEntityMeasurementSeries::getDisplayObs() const{
    return QString::number(this->myObservations.size());
}

QString ScalarEntityMeasurementSeries::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString ScalarEntityMeasurementSeries::getDisplayMConfig() const{
    return this->activeMeasurementConfig.getDisplayName();
}

QString ScalarEntityMeasurementSeries::getDisplayStdDev() const{

    if(this->myStatistic.isValid){
        //return QString::number(this->myStatistic.stdev*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
    }
    return "-/-";

}

QString ScalarEntityMeasurementSeries::getDisplayScalarMeasurementSeriesValue() const{
    return QString::number(this->seriesValue,'f',6);
}
*/

/*!
 * \brief ScalarEntityMeasurementSeries::ScalarEntityMeasurementSeries
 * \param isNominal
 * \param parent
 */
ScalarEntityMeasurementSeries::ScalarEntityMeasurementSeries(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setScalarEntityMeasurementSeries(this);
    }

}

/*!
 * \brief ScalarEntityMeasurementSeries::ScalarEntityMeasurementSeries
 * \param isNominal
 * \param seriesValue
 * \param parent
 */
ScalarEntityMeasurementSeries::ScalarEntityMeasurementSeries(const bool &isNominal, const double &seriesValue, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setScalarEntityMeasurementSeries(this);
    }

    this->setSeriesValue(seriesValue);

}

/*!
 * \brief ScalarEntityMeasurementSeries::ScalarEntityMeasurementSeries
 * \param copy
 * \param parent
 */
ScalarEntityMeasurementSeries::ScalarEntityMeasurementSeries(const ScalarEntityMeasurementSeries &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setScalarEntityMeasurementSeries(this);
    }

    this->seriesValue = copy.seriesValue;

}

/*!
 * \brief ScalarEntityMeasurementSeries::operator =
 * \param copy
 * \return
 */
ScalarEntityMeasurementSeries &ScalarEntityMeasurementSeries::operator=(const ScalarEntityMeasurementSeries &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setScalarEntityMeasurementSeries(this);
    }

    this->seriesValue = copy.seriesValue;

    return *this;

}

/*!
 * \brief ScalarEntityMeasurementSeries::~ScalarEntityMeasurementSeries
 */
ScalarEntityMeasurementSeries::~ScalarEntityMeasurementSeries(){

}

/*!
 * \brief ScalarEntityMeasurementSeries::getSeriesValue
 * \return
 */
const double &ScalarEntityMeasurementSeries::getSeriesValue() const{
    return this->seriesValue;
}

/*!
 * \brief ScalarEntityMeasurementSeries::setSeriesValue
 * \param seriesValue
 */
void ScalarEntityMeasurementSeries::setSeriesValue(const double &seriesValue){
    this->seriesValue = seriesValue;
}

/*!
 * \brief ScalarEntityMeasurementSeries::recalc
 */
void ScalarEntityMeasurementSeries::recalc(){

    Geometry::recalc();

    //reset measurement series definition if not solved
    if(!this->isSolved){
        this->seriesValue = 0.0;
    }

}

/*!
 * \brief ScalarEntityMeasurementSeries::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement ScalarEntityMeasurementSeries::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement entityMeasurementSeries = Geometry::toOpenIndyXML(xmlDoc);

    if(entityMeasurementSeries.isNull()){
        return entityMeasurementSeries;
    }

    entityMeasurementSeries.setAttribute("type", getGeometryTypeName(eScalarEntityMeasurementSeriesGeometry));

    //add series value
    QDomElement seriesValue = xmlDoc.createElement("seriesValue");
    if(this->getIsSolved() || this->getIsNominal()){
        seriesValue.setAttribute("value", this->seriesValue);
    }else{
        seriesValue.setAttribute("value", 0.0);
    }
    entityMeasurementSeries.appendChild(seriesValue);

    return entityMeasurementSeries;

}

/*!
 * \brief ScalarEntityMeasurementSeries::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool ScalarEntityMeasurementSeries::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set measurement series attributes
        QDomElement seriesValue = xmlElem.firstChildElement("seriesValue");

        if(seriesValue.isNull() || !seriesValue.hasAttribute("value")){
            return false;
        }

        this->seriesValue = seriesValue.attribute("value").toDouble();

    }

    return result;

}
