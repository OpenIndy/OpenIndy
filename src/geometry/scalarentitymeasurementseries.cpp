#include "scalarentitymeasurementseries.h"

#include "featurewrapper.h"

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

    //reset measurement series definition if not solved and no nominal
    if(!this->isSolved && !this->isNominal){
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
    if(this->isSolved || this->isNominal){
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

/*!
 * \brief ScalarEntityMeasurementSeries::getDisplayMeasurementSeries
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString ScalarEntityMeasurementSeries::getDisplayMeasurementSeries(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->seriesValue, type), 'f', digits);
    }
    return QString("");
}
