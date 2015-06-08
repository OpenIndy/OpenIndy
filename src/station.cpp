#include "station.h"

#include "coordinatesystem.h"
#include "sensorcontrol.h"
#include "reading.h"
#include "sensor.h"
#include "oijob.h"
#include "featurewrapper.h"

/*!
 * \brief Station::Station
 * \param parent
 */
Station::Station(QObject *parent) : Feature(parent), isActiveStation(false){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setStation(this);
    }

    //create a point object as the station's position
    this->position = new Point(false);

    //create a coordinate system object as the station's coordinate system
    this->stationSystem = new CoordinateSystem(QPointer<Station>(this));
    QObject::connect(this, SIGNAL(featureNameChanged(const int&, const QString&)), this, SLOT(stationNameChanged(const int&, const QString&)), Qt::DirectConnection);

    //create a sensor control object, connect it and move it to thread
    QPointer<Station> stationPointer(this);
    this->sensorControl = new SensorControl(stationPointer);
    this->connectSensorControl();
    this->sensorControl->moveToThread(&this->stationThread);
    this->stationThread.start();

}

/*!
 * \brief Station::Station
 * \param name
 * \param parent
 */
Station::Station(const QString &name, QObject *parent) : Feature(parent), isActiveStation(false){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setStation(this);
    }

    this->name = name;

    //create a point object as the station's position
    this->position = new Point(false);
    this->position->setFeatureName(name);

    //create a coordinate system object as the station's coordinate system
    this->stationSystem = new CoordinateSystem(QPointer<Station>(this));
    this->stationSystem->setFeatureName(this->name);
    QObject::connect(this, SIGNAL(featureNameChanged(const int&, const QString&)), this, SLOT(stationNameChanged(const int&, const QString&)), Qt::DirectConnection);

    //create a sensor control object, connect it and move it to thread
    QPointer<Station> stationPointer(this);
    this->sensorControl = new SensorControl(stationPointer);
    this->connectSensorControl();
    this->sensorControl->moveToThread(&this->stationThread);
    this->stationThread.start();

}

/*!
 * \brief Station::Station
 * \param copy
 * \param parent
 */
Station::Station(const Station &copy, QObject *parent) : Feature(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setStation(this);
    }

    //copy general station attributes
    this->isActiveStation = copy.isActiveStation;
    if(!copy.position.isNull()){
        this->position = new Point(*copy.position.data());
    }

    //copy measured readings
    //this->cartesianReadings = copy.cartesianReadings;
    //this->directionReadings = copy.directionReadings;
    //this->distanceReadings = copy.distanceReadings;
    //this->polarReadings = copy.polarReadings;
    //this->levelReadings = copy.levelReadings;
    //this->temperatureRadings = copy.temperatureRadings;
    //this->undefinedReadings = copy.undefinedReadings;

}

/*!
 * \brief Station::operator =
 * \param copy
 * \return
 */
Station &Station::operator=(const Station &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setStation(this);
    }

    //copy general station attributes
    this->isActiveStation = copy.isActiveStation;
    if(!copy.position.isNull()){
        this->position = new Point(*copy.position.data());
    }

    //copy measured readings
    //this->cartesianReadings = copy.cartesianReadings;
    //this->directionReadings = copy.directionReadings;
    //this->distanceReadings = copy.distanceReadings;
    //this->polarReadings = copy.polarReadings;
    //this->levelReadings = copy.levelReadings;
    //this->temperatureRadings = copy.temperatureRadings;
    //this->undefinedReadings = copy.undefinedReadings;

    return *this;

}

/*!
 * \brief Station::~Station
 */
Station::~Station(){

    this->stationThread.quit();
    this->stationThread.wait();

    //delete corresponding coordinate system with all observations made from this station
    if(!this->stationSystem.isNull()){
        delete this->stationSystem;
    }

    //delete position of this station
    if(this->position.isNull()){
        delete this->position;
    }

    //delete sensor control
    if(!this->sensorControl.isNull()){
        delete this->sensorControl;
    }

}

/*!
 * \brief Station::getIsActiveStation
 * \return
 */
const bool &Station::getIsActiveStation() const{
    return this->isActiveStation;
}

/*!
 * \brief Station::setActiveStationState
 * \param isActiveStation
 */
void Station::setActiveStationState(const bool &isActiveStation){
    if(this->isActiveStation != isActiveStation){
        this->isActiveStation = isActiveStation;
        emit this->activeStationChanged(this->id);
    }
}

/*!
 * \brief Station::getPosition
 * \return
 */
const QPointer<Point> &Station::getPosition() const{
    return this->position;
}

/*!
 * \brief Station::getCoordinateSystem
 * \return
 */
const QPointer<CoordinateSystem> &Station::getCoordinateSystem() const{
    return this->stationSystem;
}

/*!
 * \brief Station::setSensor
 * \param sensor
 */
void Station::setSensor(const QPointer<Sensor> &sensor){
    if(!sensor.isNull() && !this->sensorControl.isNull()){
        this->sensorControl->setSensor(sensor);
        emit this->sensorChanged(this->id);
    }
}

/*!
 * \brief Station::getSensorListener
 * \return
 */
const QPointer<const SensorListener> Station::getSensorListener() const{
    if(!this->sensorControl.isNull()){
        return this->sensorControl->getSensorListener();
    }
    return QPointer<const SensorListener>();
}

/*!
 * \brief Station::getActiveSensorType
 * \return
 */
SensorTypes Station::getActiveSensorType() const{

    //check sensor control
    if(this->sensorControl.isNull() || this->sensorControl->getSensor().isNull()){
        return eUndefinedSensor;
    }

    SensorTypes sensorType = this->sensorControl->getSensor()->getSensorConfiguration().getTypeOfSensor();

    return sensorType;

}

/*!
 * \brief Station::getSupportedReadingTypes
 * \return
 */
QList<ReadingTypes> Station::getSupportedReadingTypes() const{

    QList<ReadingTypes> readingTypes;

    //check sensor
    if(this->sensorControl.isNull() || this->sensorControl->getSensor().isNull()){
        return readingTypes;
    }

    readingTypes = this->sensorControl->getSensor()->getSupportedReadingTypes();
    return readingTypes;

}

/*!
 * \brief Station::getSupportedConnectionTypes
 * \return
 */
QList<ConnectionTypes> Station::getSupportedConnectionTypes() const{

    QList<ConnectionTypes> connectionTypes;

    //check sensor
    if(this->sensorControl.isNull() || this->sensorControl->getSensor().isNull()){
        return connectionTypes;
    }

    connectionTypes = this->sensorControl->getSensor()->getSupportedConnectionTypes();
    return connectionTypes;

}

/*!
 * \brief Station::getSupportedSensorActions
 * \return
 */
QList<SensorFunctions> Station::getSupportedSensorActions() const{

    QList<SensorFunctions> sensorFunctions;

    //check sensor
    if(this->sensorControl.isNull() || this->sensorControl->getSensor().isNull()){
        return sensorFunctions;
    }

    sensorFunctions = this->sensorControl->getSensor()->getSupportedSensorActions();
    return sensorFunctions;

}

/*!
 * \brief Station::getSelfDefinedActions
 * \return
 */
QStringList Station::getSelfDefinedActions() const{

    QStringList selfDefinedActions;

    //check sensor
    if(this->sensorControl.isNull() || this->sensorControl->getSensor().isNull()){
        return selfDefinedActions;
    }

    selfDefinedActions = this->sensorControl->getSensor()->getSelfDefinedActions();
    return selfDefinedActions;

}

/*!
 * \brief Station::recalc
 */
void Station::recalc(){
    Feature::recalc();
}

/*!
 * \brief Station::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Station::toOpenIndyXML(QDomDocument &xmlDoc){

    QDomElement station = Feature::toOpenIndyXML(xmlDoc);

    if(station.isNull()){
        return station;
    }

    station.setTagName("station");

    //add station attributes
    station.setAttribute("activeStation", this->getIsActiveStation());

    //add used sensors
    if(!this->sensorControl.isNull() && this->sensorControl->getUsedSensors().size() > 0){
        QDomElement usedSensors = xmlDoc.createElement("usedSensors");
        foreach(const QPointer<Sensor> &s, this->sensorControl->getUsedSensors()){
            if(!s.isNull()){
                QDomElement sensor = s->toOpenIndyXML(xmlDoc);
                if(!sensor.isNull()){
                    usedSensors.appendChild(sensor);
                }
            }
        }
        station.appendChild(usedSensors);
    }

    //add active sensor
    if(!this->sensorControl.isNull() && !this->sensorControl->getSensor().isNull()){
        QDomElement activeSensor = this->sensorControl->getSensor()->toOpenIndyXML(xmlDoc);
        if(!activeSensor.isNull()){
            activeSensor.setTagName("activeSensor");
            station.appendChild(activeSensor);
        }
    }

    //add position
    if(!this->position.isNull()){
        QDomElement position = xmlDoc.createElement("position");
        position.setAttribute("ref", this->position->getId());
        station.appendChild(position);
    }

    //add coordinate system
    if(!this->stationSystem.isNull()){
        QDomElement stationSystem = xmlDoc.createElement("coordinateSystem");
        stationSystem.setAttribute("ref", this->stationSystem->getId());
        station.appendChild(stationSystem);
    }

    return station;

}

/*!
 * \brief Station::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Station::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Feature::fromOpenIndyXML(xmlElem);

    if(result){

        if(!xmlElem.hasAttribute("activeStation")){
            return false;
        }

        //set station attributes
        this->isActiveStation = xmlElem.attribute("activeStation").toInt();

    }

    return result;

}

/*!
 * \brief Station::getDisplayX
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Station::getDisplayX(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->position.isNull()){
        return this->position->getDisplayX(type, digits, showDiff);
    }
    return QString("-/-");
}

/*!
 * \brief Station::getDisplayY
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Station::getDisplayY(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->position.isNull()){
        return this->position->getDisplayY(type, digits, showDiff);
    }
    return QString("-/-");
}

/*!
 * \brief Station::getDisplayZ
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Station::getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->position.isNull()){
        return this->position->getDisplayZ(type, digits, showDiff);
    }
    return QString("-/-");
}

/*!
 * \brief Station::setUpFeatureId
 * Generate a new unique id when the current job was set
 */
void Station::setUpFeatureId(){

    //check job
    if(this->job.isNull()){
        return;
    }

    //generate unique id for station's position
    if(!this->position.isNull()){
        this->position->id = this->job->generateUniqueId();
    }

    //generate unique id for station's coordinate system
    if(!this->stationSystem.isNull()){
        this->stationSystem->id = this->job->generateUniqueId();
    }

    //generate unique ids for station's readings
    foreach(const QPointer<Reading> &reading, this->cartesianReadings){
        if(!reading.isNull()){
            reading->id = this->job->generateUniqueId();
        }
    }
    foreach(const QPointer<Reading> &reading, this->polarReadings){
        if(!reading.isNull()){
            reading->id = this->job->generateUniqueId();
        }
    }
    foreach(const QPointer<Reading> &reading, this->directionReadings){
        if(!reading.isNull()){
            reading->id = this->job->generateUniqueId();
        }
    }
    foreach(const QPointer<Reading> &reading, this->distanceReadings){
        if(!reading.isNull()){
            reading->id = this->job->generateUniqueId();
        }
    }
    foreach(const QPointer<Reading> &reading, this->temperatureRadings){
        if(!reading.isNull()){
            reading->id = this->job->generateUniqueId();
        }
    }
    foreach(const QPointer<Reading> &reading, this->levelReadings){
        if(!reading.isNull()){
            reading->id = this->job->generateUniqueId();
        }
    }
    foreach(const QPointer<Reading> &reading, this->undefinedReadings){
        if(!reading.isNull()){
            reading->id = this->job->generateUniqueId();
        }
    }

    //feature specific
    Feature::setUpFeatureId();

}

/*!
 * \brief Station::stationNameChanged
 * Update the station system name whenever the station's name has changed
 * \param featureId
 * \param oldName
 */
void Station::stationNameChanged(const int &featureId, const QString &oldName){
    if(!this->stationSystem.isNull()){
        this->stationSystem->setFeatureName(this->name);
    }
}

/*!
 * \brief Station::connectSensorControl
 */
void Station::connectSensorControl(){

    //connect sensor actions
    QObject::connect(this, &Station::connectSensor, this->sensorControl.data(), &SensorControl::connectSensor, Qt::AutoConnection);
    QObject::connect(this, &Station::disconnectSensor, this->sensorControl.data(), &SensorControl::disconnectSensor, Qt::AutoConnection);

    QObject::connect(this, &Station::measure, this->sensorControl.data(), &SensorControl::measure, Qt::AutoConnection);

    void (Station:: *movePolarSignal)(const double &azimuth, const double &zenith, const double &distance, const bool &isRelative) = &Station::move;
    void (SensorControl:: *movePolarSlot)(const double &azimuth, const double &zenith, const double &distance, const bool &isRelative) = &SensorControl::move;
    QObject::connect(this, movePolarSignal, this->sensorControl.data(), movePolarSlot);

    void (Station:: *moveCartesianSignal)(const double &x, const double &y, const double &z) = &Station::move;
    void (SensorControl:: *moveCartesianSlot)(const double &x, const double &y, const double &z) = &SensorControl::move;
    QObject::connect(this, moveCartesianSignal, this->sensorControl.data(), moveCartesianSlot);

    QObject::connect(this, &Station::initialize, this->sensorControl.data(), &SensorControl::initialize, Qt::AutoConnection);
    QObject::connect(this, &Station::motorState, this->sensorControl.data(), &SensorControl::motorState, Qt::AutoConnection);
    QObject::connect(this, &Station::home, this->sensorControl.data(), &SensorControl::home, Qt::AutoConnection);
    QObject::connect(this, &Station::toggleSight, this->sensorControl.data(), &SensorControl::toggleSight, Qt::AutoConnection);
    QObject::connect(this, &Station::compensation, this->sensorControl.data(), &SensorControl::compensation, Qt::AutoConnection);
    QObject::connect(this, &Station::selfDefinedAction, this->sensorControl.data(), &SensorControl::selfDefinedAction, Qt::AutoConnection);

    //connect sensor action results
    QObject::connect(this->sensorControl.data(), &SensorControl::commandFinished, this, &Station::commandFinished, Qt::AutoConnection);
    QObject::connect(this->sensorControl.data(), &SensorControl::measurementFinished, this, &Station::addReadings, Qt::AutoConnection);
    QObject::connect(this->sensorControl.data(), &SensorControl::measurementFinished, this, &Station::measurementFinished, Qt::AutoConnection);

}

/*!
 * \brief Station::addReadings
 * \param geomId
 * \param readings
 */
void Station::addReadings(const int &geomId, const QList<QPointer<Reading> > &readings){

    foreach(const QPointer<Reading> &reading, readings){

        //check reading
        if(reading.isNull()){
            continue;
        }

        switch(reading->getTypeOfReading()){
        case eCartesianReading:
            this->cartesianReadings.append(reading);
            break;
        case eDirectionReading:
            this->directionReadings.append(reading);
            break;
        case eDistanceReading:
            this->distanceReadings.append(reading);
            break;
        case ePolarReading:
            this->polarReadings.append(reading);
            break;
        case eLevelReading:
            this->levelReadings.append(reading);
            break;
        case eTemperatureReading:
            this->temperatureRadings.append(reading);
            break;
        }

    }

}

/*!
 * \brief Station::removeReading
 * \param reading
 */
void Station::removeReading(const QPointer<Reading> &reading){

    this->cartesianReadings.removeOne(reading);
    this->directionReadings.removeOne(reading);
    this->distanceReadings.removeOne(reading);
    this->polarReadings.removeOne(reading);
    this->levelReadings.removeOne(reading);
    this->temperatureRadings.removeOne(reading);
    this->undefinedReadings.removeOne(reading);

}

/*!
 * \brief Station::setJob
 * \param job
 */
void Station::setJob(const QPointer<OiJob> &job){

    //check and set job
    if(job.isNull()){
        return;
    }
    this->job = job;

    //pass job to the station system
    if(!this->stationSystem.isNull()){
        this->stationSystem->setJob(job);
    }

    //pass the job to the station point
    if(!this->position.isNull()){
        this->position->setJob(job);
    }

    //generate feature id
    this->setUpFeatureId();

}
