#include "reading.h"

#include "observation.h"
/*
#include "sensor.h"
#include "observation.h"

Reading::Reading() : Element()
{
    this->measuredAt = QDateTime::currentDateTime();
    this->face = eFrontSide;
    this->observation = NULL;
    this->instrument = NULL;
}

Reading::~Reading(){

}

const ReadingTypes &Reading::getTypeOfReading() const{
    return this->typeOfReading;
}

const ReadingPolar &Reading::getPolarReading() const{
    return this->rPolar;
}

const ReadingCartesian &Reading::getCartesianReading() const{
    return this->rCartesian;
}

const ReadingDirection &Reading::getDirectionReading() const{
    return this->rDirection;
}

const ReadingDistance &Reading::getDistanceReading() const{
    return this->rDistance;
}

const ReadingTemperature &Reading::getTemperatureReading() const{
    return this->rTemperature;
}

const ReadingLevel &Reading::getLevelReading() const{
    return this->rLevel;
}

const ReadingUndefined &Reading::getUndefinedReading() const{
    return this->rUndefined;
}

const MeasurementConfig &Reading::getMeasurementConfig(){
    return this->mConfig;
}

void Reading::setMeasurementConfig(const MeasurementConfig &mConfig){
    this->mConfig = mConfig;
}

const QPointer<Sensor> &Reading::getSensor() const{
    return this->instrument;
}

void Reading::setSensor(const QPointer<Sensor> &sensor){
    this->instrument = sensor;
}

const QPointer<Observation> &Reading::getObservation() const{
    return this->observation;
}

void Reading::setObservation(const QPointer<Observation> &observation){
    this->observation = observation;
}

bool Reading::toCartesian(){

    if(rPolar.isValid){
        this->rCartesian.xyz = OiVec(4);
        this->rCartesian.xyz.setAt( 0, this->rPolar.distance * qSin(this->rPolar.zenith) * qCos(this->rPolar.azimuth) );
        this->rCartesian.xyz.setAt( 1, this->rPolar.distance * qSin(this->rPolar.zenith) * qSin(this->rPolar.azimuth) );
        this->rCartesian.xyz.setAt( 2, this->rPolar.distance * qCos(this->rPolar.zenith) );
        this->rCartesian.xyz.setAt( 3, 1.0 );
    }

    return this->rPolar.isValid;

}

OiVec Reading::toCartesian(const double &azimuth, const double &zenith, const double &distance){

    OiVec g(4);

    g.setAt(0, distance * qSin(zenith) * qCos(azimuth) );
    g.setAt(1, distance * qSin(zenith) * qSin(azimuth) );
    g.setAt(2, distance * qCos(zenith) );
    g.setAt(3, 1.0 );

    return g;

}

bool Reading::toPolar(){

    if(this->rCartesian.isValid && this->rCartesian.xyz.getSize() == 4){

        OiVec polarElems = Reading::toPolar(this->rCartesian.xyz.getAt(0), this->rCartesian.xyz.getAt(1), this->rCartesian.xyz.getAt(2));

        this->rPolar.azimuth = polarElems.getAt(0);
        this->rPolar.zenith = polarElems.getAt(1);
        this->rPolar.distance = polarElems.getAt(2);

        this->rDirection.azimuth = polarElems.getAt(0);
        this->rDirection.zenith = polarElems.getAt(1);

        this->rDistance.distance = polarElems.getAt(2);

    }

    return (this->rCartesian.isValid && this->rCartesian.xyz.getSize() == 4);

}

OiVec Reading::toPolar(const double &x, const double &y, const double &z){

    OiVec g(4);

    double azimuth = qAtan2(y,x);
    double s = qSqrt(x*x+y*y+z*z);
    double zenith = acos(z/s);

    g.setAt( 0, azimuth);
    g.setAt( 1, zenith);
    g.setAt( 2, s);
    g.setAt( 3, 1.0 );

    return g;

}

void Reading::makeBackup(){
    this->backupPolar = this->rPolar;
    this->backupCartesian = this->rCartesian;
    this->backupDirection = this->rDirection;
    this->backupDistance = this->rDistance;
    this->backupTemperature = this->rTemperature;
    this->backupUndefined = this->rUndefined;
    this->backupLevel = this->rLevel;
}

void Reading::restoreBackup(){
    this->rPolar = this->backupPolar;
    this->rCartesian = this->backupCartesian;
    this->rDirection = this->backupDirection;
    this->rDistance = this->backupDistance;
    this->rTemperature = this->backupTemperature;
    this->rUndefined = this->backupUndefined;
    this->rLevel = this->backupLevel;
}

OiVec Reading::errorPropagationPolarToCartesian(){

    OiVec sigmaCartXyz;

    OiMat F(3,3);
    F.setAt(0, 0, qSin(this->rPolar.zenith) * qCos(this->rPolar.azimuth));
    F.setAt(0, 1, this->rPolar.distance * qSin(this->rPolar.zenith) * -qSin(this->rPolar.azimuth));
    F.setAt(0, 2, this->rPolar.distance * qCos(this->rPolar.zenith) * qCos(this->rPolar.azimuth));
    F.setAt(1, 0, qSin(this->rPolar.zenith) * qSin(this->rPolar.azimuth));
    F.setAt(1, 1, this->rPolar.distance * qSin(this->rPolar.zenith) * qCos(this->rPolar.azimuth));
    F.setAt(1, 2, this->rPolar.distance * qCos(this->rPolar.zenith) * qSin(this->rPolar.azimuth));
    F.setAt(2, 0, qCos(this->rPolar.zenith));
    F.setAt(2, 1, 0.0);
    F.setAt(2, 2, this->rPolar.distance * -qSin(this->rPolar.zenith));

    OiMat Sll(3,3);
    Sll.setAt(0, 0, this->rPolar.sigmaDistance * this->rPolar.sigmaDistance);
    Sll.setAt(1, 1, this->rPolar.sigmaAzimuth * this->rPolar.sigmaAzimuth);
    Sll.setAt(2, 2, this->rPolar.sigmaZenith * this->rPolar.sigmaZenith);

    OiMat Qxx = F * Sll * F.t();

    //transform Qxx into homogeneous coordinates
    OiMat Qxx_hc(4,4);
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            Qxx_hc.setAt(i,j, Qxx.getAt(i,j));
        }
    }

    sigmaCartXyz.add(qSqrt(Qxx.getAt(0,0)));
    sigmaCartXyz.add(qSqrt(Qxx.getAt(1,1)));
    sigmaCartXyz.add(qSqrt(Qxx.getAt(2,2)));
    sigmaCartXyz.add(1.0);

    //if(!this->observation.isNull()){
      //  this->observation->getStatistic().qxx = Qxx_hc;
        //this->observation->getStatistic().s0_apriori = 1.0;
        //this->observation->myOriginalStatistic = this->observation->myStatistic;
    //}

    return sigmaCartXyz;

}

QDomElement Reading::toOpenIndyXML(QDomDocument &xmlDoc) const{

    if(xmlDoc.isNull()){
        return QDomElement();
    }

    QDomElement reading = xmlDoc.createElement("reading");

    //add reading attributes
    reading.setAttribute("id", this->id);
    reading.setAttribute("time", this->measuredAt.toString(Qt::ISODate));
    reading.setAttribute("type", getReadingTypeName(this->typeOfReading));

    //add measurements
    QDomElement measurements = xmlDoc.createElement("measurements");
    switch(this->typeOfReading){
    case eCartesianReading:
        if(this->rCartesian.isValid && this->rCartesian.xyz.getSize() >= 3 && this->rCartesian.sigmaXyz.getSize() >= 3){
            QDomElement x = xmlDoc.createElement("measurement");
            x.setAttribute("type", "x");
            x.setAttribute("value", this->rCartesian.xyz.getAt(0));
            x.setAttribute("sigma", this->rCartesian.sigmaXyz.getAt(0));
            measurements.appendChild(x);
            QDomElement y = xmlDoc.createElement("measurement");
            y.setAttribute("type", "y");
            y.setAttribute("value", this->rCartesian.xyz.getAt(1));
            y.setAttribute("sigma", this->rCartesian.sigmaXyz.getAt(1));
            measurements.appendChild(y);
            QDomElement z = xmlDoc.createElement("measurement");
            z.setAttribute("type", "z");
            z.setAttribute("value", this->rCartesian.xyz.getAt(2));
            z.setAttribute("sigma", this->rCartesian.sigmaXyz.getAt(2));
            measurements.appendChild(z);
        }
        break;
    case eDirectionReading:
        if(this->rDirection.isValid){
            QDomElement azimuth = xmlDoc.createElement("measurement");
            azimuth.setAttribute("type", "azimuth");
            azimuth.setAttribute("value", this->rDirection.azimuth);
            azimuth.setAttribute("sigma", this->rDirection.sigmaAzimuth);
            measurements.appendChild(azimuth);
            QDomElement zenith = xmlDoc.createElement("measurement");
            zenith.setAttribute("type", "zenith");
            zenith.setAttribute("value", this->rDirection.zenith);
            zenith.setAttribute("sigma", this->rDirection.sigmaZenith);
            measurements.appendChild(zenith);
        }
        break;
    case eDistanceReading:
        if(this->rDistance.isValid){
            QDomElement distance = xmlDoc.createElement("measurement");
            distance.setAttribute("type", "distance");
            distance.setAttribute("value", this->rDistance.distance);
            distance.setAttribute("sigma", this->rDistance.sigmaDistance);
            measurements.appendChild(distance);
        }
        break;
    case ePolarReading:
        if(this->rPolar.isValid){
            QDomElement azimuth = xmlDoc.createElement("measurement");
            azimuth.setAttribute("type", "azimuth");
            azimuth.setAttribute("value", this->rPolar.azimuth);
            azimuth.setAttribute("sigma", this->rPolar.sigmaAzimuth);
            measurements.appendChild(azimuth);
            QDomElement zenith = xmlDoc.createElement("measurement");
            zenith.setAttribute("type", "zenith");
            zenith.setAttribute("value", this->rPolar.zenith);
            zenith.setAttribute("sigma", this->rPolar.sigmaZenith);
            measurements.appendChild(zenith);
            QDomElement distance = xmlDoc.createElement("measurement");
            distance.setAttribute("type", "distance");
            distance.setAttribute("value", this->rPolar.distance);
            distance.setAttribute("sigma", this->rPolar.sigmaDistance);
            measurements.appendChild(distance);
        }
        break;
    case eTemperatureReading:
        if(this->rTemperature.isValid){
            QDomElement temperature = xmlDoc.createElement("measurement");
            temperature.setAttribute("type", "temperature");
            temperature.setAttribute("value", this->rTemperature.tempDeg);
            temperature.setAttribute("sigma", this->rTemperature.sigmaTempDeg);
            measurements.appendChild(temperature);
        }
        break;
    case eLevelReading:
        if(this->rLevel.isValid){
            QDomElement rx = xmlDoc.createElement("measurement");
            rx.setAttribute("type", "RX");
            rx.setAttribute("value", this->rLevel.RX);
            rx.setAttribute("sigma", this->rLevel.sigmaRX);
            measurements.appendChild(rx);
            QDomElement ry = xmlDoc.createElement("measurement");
            ry.setAttribute("type", "zenith");
            ry.setAttribute("value", this->rLevel.RY);
            ry.setAttribute("sigma", this->rLevel.sigmaRY);
            measurements.appendChild(ry);
            QDomElement rz = xmlDoc.createElement("measurement");
            rz.setAttribute("type", "zenith");
            rz.setAttribute("value", this->rLevel.RZ);
            rz.setAttribute("sigma", this->rLevel.sigmaRZ);
            measurements.appendChild(rz);
        }
        break;
    case eUndefinedReading:
        if(this->rUndefined.isValid && this->rUndefined.values.size() == this->rUndefined.sigmaValues.size()){
            QList<QString> measurementTypes = this->rUndefined.values.keys();
            foreach(QString type, measurementTypes){
                if(this->rUndefined.sigmaValues.contains(type)){
                    QDomElement measurement = xmlDoc.createElement("measurement");
                    measurement.setAttribute("type", type);
                    measurement.setAttribute("value", this->rUndefined.values.value(type));
                    measurement.setAttribute("sigma", this->rUndefined.sigmaValues.value(type));
                    measurements.appendChild(measurement);
                }
            }
        }
        break;
    }
    reading.appendChild(measurements);

    return reading;

}

bool Reading::fromOpenIndyXML(QDomElement &xmlElem){

    if(xmlElem.isNull()){
        return false;
    }

    //set reading attributes
    if(!xmlElem.hasAttribute("id") || !xmlElem.hasAttribute("time") || !xmlElem.hasAttribute("type")){
        return false;
    }
    this->id = xmlElem.attribute("id").toInt();
    this->measuredAt = QDateTime::fromString(xmlElem.attribute("time"), Qt::ISODate);
    this->typeOfReading = getReadingTypeEnum(xmlElem.attribute("type"));

    //get list of measurements
    QDomElement measurements = xmlElem.firstChildElement("measurements");
    if(measurements.isNull()){
        return false;
    }
    QDomNodeList measurementList = measurements.elementsByTagName("measurement");
    if(measurementList.size() == 0){
        return false;
    }

    //initialize measurement variables
    this->rCartesian.xyz = OiVec(4);
    this->rCartesian.xyz.setAt(3, 1.0);
    this->rCartesian.sigmaXyz = OiVec(4);
    this->rCartesian.sigmaXyz.setAt(3, 1.0);

    //fill measurement values
    for(int i = 0; i < measurementList.size(); i++){
        QDomElement measurement = measurementList.at(i).toElement();
        if(!measurement.hasAttribute("type") || !measurement.hasAttribute("value") || !measurement.hasAttribute("sigma")){
            continue;
        }
        if(measurement.attribute("type").compare("x") == 0){
            this->rCartesian.xyz.setAt(0, measurement.attribute("value").toDouble());
        }else if(measurement.attribute("type").compare("y") == 0){
            this->rCartesian.xyz.setAt(1, measurement.attribute("value").toDouble());
        }else if(measurement.attribute("type").compare("z") == 0){
            this->rCartesian.xyz.setAt(2, measurement.attribute("value").toDouble());
        }else if(measurement.attribute("type").compare("azimuth") == 0){
            this->rPolar.azimuth = measurement.attribute("value").toDouble();
            this->rDirection.azimuth = measurement.attribute("value").toDouble();
        }else if(measurement.attribute("type").compare("zenith") == 0){
            this->rPolar.zenith = measurement.attribute("value").toDouble();
            this->rDirection.zenith = measurement.attribute("value").toDouble();
        }else if(measurement.attribute("type").compare("distance") == 0){
            this->rPolar.distance = measurement.attribute("value").toDouble();
            this->rDistance.distance = measurement.attribute("value").toDouble();
        }else if(measurement.attribute("type").compare("RX") == 0){
            this->rLevel.RX = measurement.attribute("value").toDouble();
        }else if(measurement.attribute("type").compare("RY") == 0){
            this->rLevel.RY = measurement.attribute("value").toDouble();
        }else if(measurement.attribute("type").compare("RZ") == 0){
            this->rLevel.RZ = measurement.attribute("value").toDouble();
        }
    }

}
*/

/*!
 * \brief Reading::Reading
 * \param reading
 * \param parent
 */
Reading::Reading(const ReadingPolar &reading, QObject *parent) : Element(parent), hasBackup(false){

    //set the reading and transform into cartesian
    this->typeOfReading = ePolarReading;
    this->rPolar = reading;
    this->toCartesian();
    this->rCartesian.sigmaXyz = this->errorPropagationPolarToCartesian();

    //set default attributes
    this->measuredAt = QDateTime::currentDateTime();
    this->face = eFrontSide;

}

/*!
 * \brief Reading::Reading
 * \param reading
 * \param parent
 */
Reading::Reading(const ReadingCartesian &reading, QObject *parent) : Element(parent), hasBackup(false){

    if(reading.xyz.getSize() != 3 || reading.sigmaXyz.getSize() != 3){
        this->typeOfReading = eCartesianReading;
        return;
    }

    //set the reading and transform into polar
    this->typeOfReading = eCartesianReading;
    this->rCartesian = reading;
    this->toPolar();

    //set default attributes
    this->measuredAt = QDateTime::currentDateTime();
    this->face = eFrontSide;

}

/*!
 * \brief Reading::Reading
 * \param reading
 * \param parent
 */
Reading::Reading(const ReadingDirection &reading, QObject *parent) : Element(parent), hasBackup(false){

    //set the reading
    this->typeOfReading = eDirectionReading;
    this->rDirection = reading;

    //set default attributes
    this->measuredAt = QDateTime::currentDateTime();
    this->face = eFrontSide;

}

/*!
 * \brief Reading::Reading
 * \param reading
 * \param parent
 */
Reading::Reading(const ReadingDistance &reading, QObject *parent) : Element(parent), hasBackup(false){

    //set the reading and
    this->typeOfReading = eDistanceReading;
    this->rDistance = reading;

    //set default attributes
    this->measuredAt = QDateTime::currentDateTime();
    this->face = eFrontSide;

}

/*!
 * \brief Reading::Reading
 * \param reading
 * \param parent
 */
Reading::Reading(const ReadingTemperature &reading, QObject *parent) : Element(parent), hasBackup(false){

    //set the reading
    this->typeOfReading = eTemperatureReading;
    this->rTemperature = reading;

    //set default attributes
    this->measuredAt = QDateTime::currentDateTime();
    this->face = eFrontSide;

}

/*!
 * \brief Reading::Reading
 * \param reading
 * \param parent
 */
Reading::Reading(const ReadingLevel &reading, QObject *parent) : Element(parent), hasBackup(false){

    //set the reading
    this->typeOfReading = eLevelReading;
    this->rLevel = reading;

    //set default attributes
    this->measuredAt = QDateTime::currentDateTime();
    this->face = eFrontSide;

}

/*!
 * \brief Reading::Reading
 * \param reading
 * \param parent
 */
Reading::Reading(const ReadingUndefined &reading, QObject *parent) : Element(parent), hasBackup(false){

    //set the reading
    this->typeOfReading = eUndefinedReading;
    this->rUndefined = reading;

    //set default attributes
    this->measuredAt = QDateTime::currentDateTime();
    this->face = eFrontSide;

}

/*!
 * \brief Reading::Reading
 * \param copy
 * \param parent
 */
Reading::Reading(const Reading &copy, QObject *parent) : Element(copy, parent){

    //copy reading attributes
    this->measuredAt = copy.measuredAt;
    this->mConfig = copy.mConfig;
    this->face = copy.face;
    this->sensor = copy.sensor;
    this->observation = copy.observation;
    this->hasBackup = copy.hasBackup;

    //copy readings
    this->typeOfReading = copy.typeOfReading;
    this->rPolar = copy.rPolar;
    this->backupPolar = copy.backupPolar;
    this->rCartesian = copy.rCartesian;
    this->backupCartesian = copy.backupCartesian;
    this->rDirection = copy.rDirection;
    this->backupDirection = copy.backupDirection;
    this->rDistance = copy.rDistance;
    this->backupDistance = copy.backupDistance;
    this->rTemperature = copy.rTemperature;
    this->backupTemperature = copy.backupTemperature;
    this->rLevel = copy.rLevel;
    this->backupLevel = copy.backupLevel;
    this->rUndefined = copy.rUndefined;
    this->backupUndefined = copy.backupUndefined;

}

/*!
 * \brief Reading::operator =
 * \param copy
 * \return
 */
Reading &Reading::operator=(const Reading &copy){

    //copy reading attributes
    this->measuredAt = copy.measuredAt;
    this->mConfig = copy.mConfig;
    this->face = copy.face;
    this->sensor = copy.sensor;
    this->observation = copy.observation;
    this->hasBackup = copy.hasBackup;

    //copy readings
    this->typeOfReading = copy.typeOfReading;
    this->rPolar = copy.rPolar;
    this->backupPolar = copy.backupPolar;
    this->rCartesian = copy.rCartesian;
    this->backupCartesian = copy.backupCartesian;
    this->rDirection = copy.rDirection;
    this->backupDirection = copy.backupDirection;
    this->rDistance = copy.rDistance;
    this->backupDistance = copy.backupDistance;
    this->rTemperature = copy.rTemperature;
    this->backupTemperature = copy.backupTemperature;
    this->rLevel = copy.rLevel;
    this->backupLevel = copy.backupLevel;
    this->rUndefined = copy.rUndefined;
    this->backupUndefined = copy.backupUndefined;

    return *this;

}

/*!
 * \brief Reading::~Reading
 */
Reading::~Reading(){

}

/*!
 * \brief Reading::getTypeOfReading
 * \return
 */
const ReadingTypes &Reading::getTypeOfReading() const{
    return this->typeOfReading;
}

/*!
 * \brief Reading::getPolarReading
 * \return
 */
const ReadingPolar &Reading::getPolarReading() const{
    return this->rPolar;
}

/*!
 * \brief Reading::getCartesianReading
 * \return
 */
const ReadingCartesian &Reading::getCartesianReading() const{
    return this->rCartesian;
}

/*!
 * \brief Reading::getDirectionReading
 * \return
 */
const ReadingDirection &Reading::getDirectionReading() const{
    return this->rDirection;
}

/*!
 * \brief Reading::getDistanceReading
 * \return
 */
const ReadingDistance &Reading::getDistanceReading() const{
    return this->rDistance;
}

/*!
 * \brief Reading::getTemperatureReading
 * \return
 */
const ReadingTemperature &Reading::getTemperatureReading() const{
    return this->rTemperature;
}

/*!
 * \brief Reading::getLevelReading
 * \return
 */
const ReadingLevel &Reading::getLevelReading() const{
    return this->rLevel;
}

/*!
 * \brief Reading::getUndefinedReading
 * \return
 */
const ReadingUndefined &Reading::getUndefinedReading() const{
    return this->rUndefined;
}

/*!
 * \brief Reading::setPolarReading
 * \param rPolar
 */
void Reading::setPolarReading(const ReadingPolar &rPolar){
    if(this->rPolar.isValid && rPolar.isValid){
        this->rPolar = rPolar;
    }
}

/*!
 * \brief Reading::makeBackup
 */
void Reading::makeBackup(){

    this->backupPolar = this->rPolar;
    this->backupCartesian = this->rCartesian;
    this->backupDirection = this->rDirection;
    this->backupDistance = this->rDistance;
    this->backupTemperature = this->rTemperature;
    this->backupUndefined = this->rUndefined;
    this->backupLevel = this->rLevel;

    this->hasBackup = true;

}

/*!
 * \brief Reading::restoreBackup
 */
void Reading::restoreBackup(){

    //do not restore backup if no backup was made
    if(!this->hasBackup){
        return;
    }

    this->rPolar = this->backupPolar;
    this->rCartesian = this->backupCartesian;
    this->rDirection = this->backupDirection;
    this->rDistance = this->backupDistance;
    this->rTemperature = this->backupTemperature;
    this->rUndefined = this->backupUndefined;
    this->rLevel = this->backupLevel;

}

/*!
 * \brief Reading::toCartesian
 * Returns a vector containing the cartesian coordinates corresponding to the given polar elements
 * \param azimuth
 * \param zenith
 * \param distance
 * \return
 */
OiVec Reading::toCartesian(const double &azimuth, const double &zenith, const double &distance){

    OiVec xyz(3);

    xyz.setAt(0, distance * qSin(zenith) * qCos(azimuth) );
    xyz.setAt(1, distance * qSin(zenith) * qSin(azimuth) );
    xyz.setAt(2, distance * qCos(zenith) );

    return xyz;

}

/*!
 * \brief Reading::toPolar
 * Returns a vector containing the polar elements (azimuth, zenith, distance) corresponding to the given cartesian coordinates
 * \param x
 * \param y
 * \param z
 * \return
 */
OiVec Reading::toPolar(const double &x, const double &y, const double &z){

    OiVec polar(3);

    double azimuth = qAtan2(y,x);
    double s = qSqrt(x*x+y*y+z*z);
    double zenith = acos(z/s);

    polar.setAt( 0, azimuth);
    polar.setAt( 1, zenith);
    polar.setAt( 2, s);

    return polar;

}

/*!
 * \brief Reading::getMeasurementConfig
 * \return
 */
const MeasurementConfig &Reading::getMeasurementConfig(){
    return this->mConfig;
}

/*!
 * \brief Reading::setMeasurementConfig
 * \param mConfig
 */
void Reading::setMeasurementConfig(const MeasurementConfig &mConfig){
    this->mConfig = mConfig;
}

/*!
 * \brief Reading::getMeasuredAt
 * \return
 */
const QDateTime &Reading::getMeasuredAt() const{
    return this->measuredAt;
}

/*!
 * \brief Reading::setMeasuredAt
 * \param measuredAt
 */
void Reading::setMeasuredAt(const QDateTime &measuredAt){
    this->measuredAt = measuredAt;
}

/*!
 * \brief Reading::getFace
 * \return
 */
const SensorFaces &Reading::getFace() const{
    return this->face;
}

/*!
 * \brief Reading::setSensorFace
 * \param face
 */
void Reading::setSensorFace(const SensorFaces &face){
    this->face = face;
}

/*!
 * \brief Reading::getSensor
 * \return
 */
const QPointer<Sensor> &Reading::getSensor() const{
    return this->sensor;
}

/*!
 * \brief Reading::setSensor
 * \param sensor
 */
void Reading::setSensor(const QPointer<Sensor> &sensor){
    if(this->sensor.isNull() && !sensor.isNull()){
        this->sensor = sensor;
    }
}

/*!
 * \brief Reading::getObservation
 * \return
 */
const QPointer<Observation> &Reading::getObservation() const{
    return this->observation;
}

/*!
 * \brief Reading::setObservation
 * \param observation
 */
void Reading::setObservation(const QPointer<Observation> &observation){
    if(!observation.isNull()){
        observation->reading = this;
        this->observation = observation;
    }
}

/*!
 * \brief Reading::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Reading::toOpenIndyXML(QDomDocument &xmlDoc) const{

    if(xmlDoc.isNull()){
        return QDomElement();
    }

    QDomElement reading = xmlDoc.createElement("reading");
/*
    //add reading attributes
    reading.setAttribute("id", this->id);
    reading.setAttribute("time", this->measuredAt.toString(Qt::ISODate));
    reading.setAttribute("type", getReadingTypeName(this->typeOfReading));

    //add measurements
    QDomElement measurements = xmlDoc.createElement("measurements");
    switch(this->typeOfReading){
    case eCartesianReading:
        if(this->rCartesian.isValid && this->rCartesian.xyz.getSize() >= 3 && this->rCartesian.sigmaXyz.getSize() >= 3){
            QDomElement x = xmlDoc.createElement("measurement");
            x.setAttribute("type", "x");
            x.setAttribute("value", this->rCartesian.xyz.getAt(0));
            x.setAttribute("sigma", this->rCartesian.sigmaXyz.getAt(0));
            measurements.appendChild(x);
            QDomElement y = xmlDoc.createElement("measurement");
            y.setAttribute("type", "y");
            y.setAttribute("value", this->rCartesian.xyz.getAt(1));
            y.setAttribute("sigma", this->rCartesian.sigmaXyz.getAt(1));
            measurements.appendChild(y);
            QDomElement z = xmlDoc.createElement("measurement");
            z.setAttribute("type", "z");
            z.setAttribute("value", this->rCartesian.xyz.getAt(2));
            z.setAttribute("sigma", this->rCartesian.sigmaXyz.getAt(2));
            measurements.appendChild(z);
        }
        break;
    case eDirectionReading:
        if(this->rDirection.isValid){
            QDomElement azimuth = xmlDoc.createElement("measurement");
            azimuth.setAttribute("type", "azimuth");
            azimuth.setAttribute("value", this->rDirection.azimuth);
            azimuth.setAttribute("sigma", this->rDirection.sigmaAzimuth);
            measurements.appendChild(azimuth);
            QDomElement zenith = xmlDoc.createElement("measurement");
            zenith.setAttribute("type", "zenith");
            zenith.setAttribute("value", this->rDirection.zenith);
            zenith.setAttribute("sigma", this->rDirection.sigmaZenith);
            measurements.appendChild(zenith);
        }
        break;
    case eDistanceReading:
        if(this->rDistance.isValid){
            QDomElement distance = xmlDoc.createElement("measurement");
            distance.setAttribute("type", "distance");
            distance.setAttribute("value", this->rDistance.distance);
            distance.setAttribute("sigma", this->rDistance.sigmaDistance);
            measurements.appendChild(distance);
        }
        break;
    case ePolarReading:
        if(this->rPolar.isValid){
            QDomElement azimuth = xmlDoc.createElement("measurement");
            azimuth.setAttribute("type", "azimuth");
            azimuth.setAttribute("value", this->rPolar.azimuth);
            azimuth.setAttribute("sigma", this->rPolar.sigmaAzimuth);
            measurements.appendChild(azimuth);
            QDomElement zenith = xmlDoc.createElement("measurement");
            zenith.setAttribute("type", "zenith");
            zenith.setAttribute("value", this->rPolar.zenith);
            zenith.setAttribute("sigma", this->rPolar.sigmaZenith);
            measurements.appendChild(zenith);
            QDomElement distance = xmlDoc.createElement("measurement");
            distance.setAttribute("type", "distance");
            distance.setAttribute("value", this->rPolar.distance);
            distance.setAttribute("sigma", this->rPolar.sigmaDistance);
            measurements.appendChild(distance);
        }
        break;
    case eTemperatureReading:
        if(this->rTemperature.isValid){
            QDomElement temperature = xmlDoc.createElement("measurement");
            temperature.setAttribute("type", "temperature");
            temperature.setAttribute("value", this->rTemperature.tempDeg);
            temperature.setAttribute("sigma", this->rTemperature.sigmaTempDeg);
            measurements.appendChild(temperature);
        }
        break;
    case eLevelReading:
        if(this->rLevel.isValid){
            QDomElement rx = xmlDoc.createElement("measurement");
            rx.setAttribute("type", "RX");
            rx.setAttribute("value", this->rLevel.RX);
            rx.setAttribute("sigma", this->rLevel.sigmaRX);
            measurements.appendChild(rx);
            QDomElement ry = xmlDoc.createElement("measurement");
            ry.setAttribute("type", "zenith");
            ry.setAttribute("value", this->rLevel.RY);
            ry.setAttribute("sigma", this->rLevel.sigmaRY);
            measurements.appendChild(ry);
            QDomElement rz = xmlDoc.createElement("measurement");
            rz.setAttribute("type", "zenith");
            rz.setAttribute("value", this->rLevel.RZ);
            rz.setAttribute("sigma", this->rLevel.sigmaRZ);
            measurements.appendChild(rz);
        }
        break;
    case eUndefinedReading:
        if(this->rUndefined.isValid && this->rUndefined.values.size() == this->rUndefined.sigmaValues.size()){
            QList<QString> measurementTypes = this->rUndefined.values.keys();
            foreach(QString type, measurementTypes){
                if(this->rUndefined.sigmaValues.contains(type)){
                    QDomElement measurement = xmlDoc.createElement("measurement");
                    measurement.setAttribute("type", type);
                    measurement.setAttribute("value", this->rUndefined.values.value(type));
                    measurement.setAttribute("sigma", this->rUndefined.sigmaValues.value(type));
                    measurements.appendChild(measurement);
                }
            }
        }
        break;
    }
    reading.appendChild(measurements);
*/
    return reading;

}

/*!
 * \brief Reading::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Reading::fromOpenIndyXML(QDomElement &xmlElem){

    if(xmlElem.isNull()){
        return false;
    }
/*
    //set reading attributes
    if(!xmlElem.hasAttribute("id") || !xmlElem.hasAttribute("time") || !xmlElem.hasAttribute("type")){
        return false;
    }
    this->id = xmlElem.attribute("id").toInt();
    this->measuredAt = QDateTime::fromString(xmlElem.attribute("time"), Qt::ISODate);
    this->typeOfReading = getReadingTypeEnum(xmlElem.attribute("type"));

    //get list of measurements
    QDomElement measurements = xmlElem.firstChildElement("measurements");
    if(measurements.isNull()){
        return false;
    }
    QDomNodeList measurementList = measurements.elementsByTagName("measurement");
    if(measurementList.size() == 0){
        return false;
    }

    //initialize measurement variables
    this->rCartesian.xyz = OiVec(4);
    this->rCartesian.xyz.setAt(3, 1.0);
    this->rCartesian.sigmaXyz = OiVec(4);
    this->rCartesian.sigmaXyz.setAt(3, 1.0);

    //fill measurement values
    for(int i = 0; i < measurementList.size(); i++){
        QDomElement measurement = measurementList.at(i).toElement();
        if(!measurement.hasAttribute("type") || !measurement.hasAttribute("value") || !measurement.hasAttribute("sigma")){
            continue;
        }
        if(measurement.attribute("type").compare("x") == 0){
            this->rCartesian.xyz.setAt(0, measurement.attribute("value").toDouble());
        }else if(measurement.attribute("type").compare("y") == 0){
            this->rCartesian.xyz.setAt(1, measurement.attribute("value").toDouble());
        }else if(measurement.attribute("type").compare("z") == 0){
            this->rCartesian.xyz.setAt(2, measurement.attribute("value").toDouble());
        }else if(measurement.attribute("type").compare("azimuth") == 0){
            this->rPolar.azimuth = measurement.attribute("value").toDouble();
            this->rDirection.azimuth = measurement.attribute("value").toDouble();
        }else if(measurement.attribute("type").compare("zenith") == 0){
            this->rPolar.zenith = measurement.attribute("value").toDouble();
            this->rDirection.zenith = measurement.attribute("value").toDouble();
        }else if(measurement.attribute("type").compare("distance") == 0){
            this->rPolar.distance = measurement.attribute("value").toDouble();
            this->rDistance.distance = measurement.attribute("value").toDouble();
        }else if(measurement.attribute("type").compare("RX") == 0){
            this->rLevel.RX = measurement.attribute("value").toDouble();
        }else if(measurement.attribute("type").compare("RY") == 0){
            this->rLevel.RY = measurement.attribute("value").toDouble();
        }else if(measurement.attribute("type").compare("RZ") == 0){
            this->rLevel.RZ = measurement.attribute("value").toDouble();
        }
    }
*/
    return false;
}

/*!
 * \brief Reading::toCartesian
 */
void Reading::toCartesian(){

    //check if the polar reading is valid
    if(!this->rPolar.isValid){
        return;
    }

    //transform polar to cartesian
    this->rCartesian.xyz.setAt( 0, this->rPolar.distance * qSin(this->rPolar.zenith) * qCos(this->rPolar.azimuth) );
    this->rCartesian.xyz.setAt( 1, this->rPolar.distance * qSin(this->rPolar.zenith) * qSin(this->rPolar.azimuth) );
    this->rCartesian.xyz.setAt( 2, this->rPolar.distance * qCos(this->rPolar.zenith) );
    this->rCartesian.isValid = true;

}

/*!
 * \brief Reading::toPolar
 */
void Reading::toPolar(){

    //check if the cartesian reading is valid
    if(!this->rCartesian.isValid){
        return;
    }

    //transform cartesian to polar
    double x = this->rCartesian.xyz.getAt(0);
    double y = this->rCartesian.xyz.getAt(1);
    double z = this->rCartesian.xyz.getAt(2);
    this->rPolar.azimuth = qAtan2(y,x);
    this->rPolar.distance = qSqrt(x*x+y*y+z*z);
    this->rPolar.zenith = acos(z/this->rPolar.distance);

}

/*!
 * \brief Reading::errorPropagationPolarToCartesian
 * \return
 */
OiVec Reading::errorPropagationPolarToCartesian(){

    //check if polar reading is valid
    if(!rPolar.isValid){
        return OiVec(3);
    }

    OiVec sigmaXyz;

    OiMat F(3,3);
    F.setAt(0, 0, qSin(this->rPolar.zenith) * qCos(this->rPolar.azimuth));
    F.setAt(0, 1, this->rPolar.distance * qSin(this->rPolar.zenith) * -qSin(this->rPolar.azimuth));
    F.setAt(0, 2, this->rPolar.distance * qCos(this->rPolar.zenith) * qCos(this->rPolar.azimuth));
    F.setAt(1, 0, qSin(this->rPolar.zenith) * qSin(this->rPolar.azimuth));
    F.setAt(1, 1, this->rPolar.distance * qSin(this->rPolar.zenith) * qCos(this->rPolar.azimuth));
    F.setAt(1, 2, this->rPolar.distance * qCos(this->rPolar.zenith) * qSin(this->rPolar.azimuth));
    F.setAt(2, 0, qCos(this->rPolar.zenith));
    F.setAt(2, 1, 0.0);
    F.setAt(2, 2, this->rPolar.distance * -qSin(this->rPolar.zenith));

    OiMat Sll(3,3);
    Sll.setAt(0, 0, this->rPolar.sigmaDistance * this->rPolar.sigmaDistance);
    Sll.setAt(1, 1, this->rPolar.sigmaAzimuth * this->rPolar.sigmaAzimuth);
    Sll.setAt(2, 2, this->rPolar.sigmaZenith * this->rPolar.sigmaZenith);

    OiMat Qxx = F * Sll * F.t();

    sigmaXyz.add(qSqrt(Qxx.getAt(0,0)));
    sigmaXyz.add(qSqrt(Qxx.getAt(1,1)));
    sigmaXyz.add(qSqrt(Qxx.getAt(2,2)));
    sigmaXyz.add(1.0);

    return sigmaXyz;

}
