#include "station.h"
#include <QVariant>

Station::Station(QString name)
{
    this->name = name;

    this->id = Configuration::generateID();
    position = new Point(false);
    position->setFeatureName(name);

    //ini member
    coordSys = new CoordinateSystem();
    coordSys->setFeatureName(this->name);

    sensorPad = new SensorControl(this);
    connect(&sensorPad->getOiEmitter(), SIGNAL(sendString(QString)), this, SLOT(writeToConsole(QString)));

    //move controller to the station thread
    sensorPad->moveToThread(&stationThread);

    //connections between station and sensorcontrol
    connect(this->sensorPad,SIGNAL(commandFinished(bool)),this,SLOT(emitActionFinished(bool)));

    connect(this,SIGNAL(startMeasure(Geometry*,bool)),this->sensorPad,SLOT(measure(Geometry*,bool)));
    connect(this,SIGNAL(startMove(double,double,double,bool)),this->sensorPad,SLOT(move(double,double,double,bool)));
    connect(this,SIGNAL(startMove(double,double,double)),this->sensorPad,SLOT(move(double,double,double)));
    connect(this,SIGNAL(startInitialize()),this->sensorPad,SLOT(initialize()));
    connect(this,SIGNAL(startMotorState()),this->sensorPad,SLOT(motorState()));
    connect(this,SIGNAL(startHome()),this->sensorPad,SLOT(home()));
    connect(this,SIGNAL(startToggleSight()),this->sensorPad,SLOT(toggleSight()));
    connect(this,SIGNAL(startCompensation()),this->sensorPad,SLOT(compensation()));
    connect(this,SIGNAL(startConnect(ConnectionConfig*)),this->sensorPad,SLOT(connectSensor(ConnectionConfig*)));
    connect(this,SIGNAL(startDisconnect()),this->sensorPad,SLOT(disconnectSensor()));
    connect(this,SIGNAL(startReadingStream(int)),this->sensorPad,SLOT(readingStream(int)));
    connect(this,SIGNAL(startSensorStatsStream()),this->sensorPad,SLOT(sensorStatsStream()));
    connect(this,SIGNAL(startSelfDefinedAction(QString)),this->sensorPad,SLOT(doSelfDefinedAction(QString)));
    connect(this,SIGNAL(stopReadingStream()),this->sensorPad,SLOT(stopReadingStream()));
    connect(this,SIGNAL(stopSensorStatsStream()),this->sensorPad,SLOT(stopStatStream()));

    //start the station thread
    stationThread.start();

    this->isUpdated = false;
    this->isDrawn = true;

}

Station::Station(){

    //initialize position
    this->position = new Point(false);

    //initialize sensor control
    sensorPad = new SensorControl(this);
    connect(&sensorPad->getOiEmitter(), SIGNAL(sendString(QString)), this, SLOT(writeToConsole(QString)));

    //move controller to the station thread
    sensorPad->moveToThread(&stationThread);

    //connections between station and sensorcontrol
    connect(this->sensorPad,SIGNAL(commandFinished(bool)),this,SLOT(emitActionFinished(bool)));

    connect(this,SIGNAL(startMeasure(Geometry*,bool)),this->sensorPad,SLOT(measure(Geometry*,bool)));
    connect(this,SIGNAL(startMove(double,double,double,bool)),this->sensorPad,SLOT(move(double,double,double,bool)));
    connect(this,SIGNAL(startMove(double,double,double)),this->sensorPad,SLOT(move(double,double,double)));
    connect(this,SIGNAL(startInitialize()),this->sensorPad,SLOT(initialize()));
    connect(this,SIGNAL(startMotorState()),this->sensorPad,SLOT(motorState()));
    connect(this,SIGNAL(startHome()),this->sensorPad,SLOT(home()));
    connect(this,SIGNAL(startToggleSight()),this->sensorPad,SLOT(toggleSight()));
    connect(this,SIGNAL(startCompensation()),this->sensorPad,SLOT(compensation()));
    connect(this,SIGNAL(startConnect(ConnectionConfig*)),this->sensorPad,SLOT(connectSensor(ConnectionConfig*)));
    connect(this,SIGNAL(startDisconnect()),this->sensorPad,SLOT(disconnectSensor()));
    connect(this,SIGNAL(startReadingStream(int)),this->sensorPad,SLOT(readingStream(int)));
    connect(this,SIGNAL(startSensorStatsStream()),this->sensorPad,SLOT(sensorStatsStream()));
    connect(this,SIGNAL(startSelfDefinedAction(QString)),this->sensorPad,SLOT(doSelfDefinedAction(QString)));
    connect(this,SIGNAL(stopReadingStream()),this->sensorPad,SLOT(stopReadingStream()));
    connect(this,SIGNAL(stopSensorStatsStream()),this->sensorPad,SLOT(stopStatStream()));

    //start the station thread
    stationThread.start();

    this->isUpdated = false;

}

Station::~Station(){

    stationThread.quit();
    stationThread.wait();

    //delete corresponding coordinate system with all observations made from this station
    delete this->coordSys;

    //delete position of this station
    delete this->position;

    //TODO Sensorliste deleten
    delete this->sensorPad;

}

/*!
 * \brief Station::getIsActiveStation
 * \return
 */
bool Station::getIsActiveStation(){
    return this->isActiveStation;
}

/*!
 * \brief Station::setAciteStationState
 * \param isActiveStation
 * \return
 */
bool Station::setActiveStationState(bool isActiveStation){
    if(this->isActiveStation != isActiveStation){
        this->isActiveStation = isActiveStation;
        emit this->activeStationChanged(this->id);
        return true;
    }
    return false;
}

void Station::stopThread(){
    stationThread.quit();
    stationThread.wait();
}

void Station::startThread(){
    stationThread.start();
}

void Station::setInstrumentConfig(SensorConfiguration *sensorConfig){

    sensorPad->InstrumentConfig = sensorConfig;
    sensorPad->instrument->setSensorConfiguration(sensorConfig);

}

SensorConfiguration* Station::getInstrumentConfig(){
    return sensorPad->InstrumentConfig;
}

/*!
 * \brief Station::recalc
 * Execute alls functions in the specified order
 */
void Station::recalc(){

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

    //add used sensors
    if(this->sensorPad != NULL && this->sensorPad->usedSensors.size() > 0){
        QDomElement usedSensors = xmlDoc.createElement("usedSensors");
        foreach(Sensor *s, this->sensorPad->usedSensors){
            if(s != NULL){
                QDomElement sensor = s->toOpenIndyXML(xmlDoc);
                if(!sensor.isNull()){
                    usedSensors.appendChild(sensor);
                }
            }
        }
        station.appendChild(usedSensors);
    }

    //add active sensor
    if(this->sensorPad != NULL && this->sensorPad->instrument != NULL){
        QDomElement activeSensor = this->sensorPad->instrument->toOpenIndyXML(xmlDoc);
        if(!activeSensor.isNull()){
            activeSensor.setTagName("activeSensor");
            station.appendChild(activeSensor);
        }
    }

    //add position
    if(this->position != NULL){
        QDomElement position = xmlDoc.createElement("position");
        position.setAttribute("ref", this->position->getId());
        station.appendChild(position);
    }

    //add coordinate system
    if(this->coordSys != NULL){
        QDomElement stationSystem = xmlDoc.createElement("coordinateSystem");
        stationSystem.setAttribute("ref", this->coordSys->getId());
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

        //set station attributes
        this->isActiveStation = false;

    }

    return result;

}

/*!
 * \brief Station::fromOpenIndyXML
 * \param xml
 * \return
 */
ElementDependencies Station::fromOpenIndyXML(QXmlStreamReader &xml){

    ElementDependencies dependencies;

    //get attributes of the station
    QXmlStreamAttributes attributes = xml.attributes();
    if(attributes.hasAttribute("name")){
        this->name = attributes.value("name").toString();
    }
    if(attributes.hasAttribute("id")) {
        this->id = attributes.value("id").toInt();
        dependencies.elementID = this->id;
        dependencies.typeOfElement = Configuration::eStationElement;
    }

    xml.readNext();

    while( !xml.atEnd() && xml.name().compare("station") != 0 ){

        if(xml.tokenType() == QXmlStreamReader::StartElement) {

            if(xml.name().compare("activeSensor") == 0){

                sensorInfo sInfo;
                if(xml.attributes().hasAttribute("name")){
                    sInfo.name = xml.attributes().value("name").toString();
                }
                if(xml.attributes().hasAttribute("plugin")) {
                    sInfo.plugin = xml.attributes().value("plugin").toString();
                }
                dependencies.addActiveSensor(sInfo);

                SensorConfiguration *sc = new SensorConfiguration();
                this->sensorPad->InstrumentConfig = sc;

                xml.readNext();

            }else if(xml.name().compare("usedSensors") == 0){

                xml.readNext();

                while(xml.name().compare("sensor") == 0
                      || xml.name().compare("readingTypes") == 0
                      || xml.name().compare("type") == 0){

                    if(xml.tokenType() == QXmlStreamReader::StartElement && xml.name().compare("sensor") == 0){

                        sensorInfo sInfo;
                        if(xml.attributes().hasAttribute("name")){
                            sInfo.name = xml.attributes().value("name").toString();
                        }
                        if(xml.attributes().hasAttribute("plugin")) {
                            sInfo.plugin = xml.attributes().value("plugin").toString();
                        }
                        dependencies.addSensorInfo(sInfo);

                        xml.readNext();

                    }

                }

            }else if(xml.name().compare("position") == 0){

                if(xml.attributes().hasAttribute("ref")){
                    dependencies.setStationPosition(xml.attributes().value("ref").toInt());
                }
                xml.readNext();

            }else if(xml.name().compare("coordinatesystem") == 0){

                if(xml.attributes().hasAttribute("ref")){
                    dependencies.setStationCoordSystem(xml.attributes().value("ref").toInt());
                }
                xml.readNext();

            }else if(xml.name().compare("usedFor") == 0){

                readFeatureAttributes(xml,dependencies);
                xml.readNext();

            }else if(xml.name() == "function"){

                this->readFunction(xml, dependencies);
                xml.readNext();

            }else{
                xml.readNext();
            }

        }else{
            xml.readNext();
        }

    }

    return dependencies;
















    /*ElementDependencies dependencies;

    QXmlStreamAttributes attributes = xml.attributes();

    if(attributes.hasAttribute("name")){
        this->name = attributes.value("name").toString();
    }
    if(attributes.hasAttribute("id")) {
        this->id = attributes.value("id").toInt();
        dependencies.elementID = this->id;
        dependencies.typeOfElement = Configuration::eStationElement;
    }

    xml.readNext();

    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "station")) {

        if(xml.tokenType() == QXmlStreamReader::StartElement) {

            /*if(xml.name() == "sensor") {

                QString sensorName;
                QString sensorPlugin;

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "sensor")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {


                        QXmlStreamAttributes sensorAttributes = xml.attributes();

                        if(sensorAttributes.hasAttribute("name")){
                            sensorName = sensorAttributes.value("name").toString();
                        }
                        if(sensorAttributes.hasAttribute("plugin")) {
                            sensorPlugin = sensorAttributes.value("plugin").toString();
                        }

                    }

                    SensorConfiguration *sc = new SensorConfiguration();

                    sensorInfo sInfo;
                    sInfo.name = sensorName;
                    sInfo.plugin = sensorPlugin;
                    dependencies.addSensorInfo(sInfo);


                    xml.readNext();
                }


            }

            if(xml.name() == "activeSensor") {

                QString sensorName;
                QString sensorPlugin;

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "activeSensor")) {

                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes sensorAttributes = xml.attributes();

                        if(sensorAttributes.hasAttribute("name")){
                            sensorName = sensorAttributes.value("name").toString();
                        }
                        if(sensorAttributes.hasAttribute("plugin")) {
                            sensorPlugin = sensorAttributes.value("plugin").toString();
                        }

                    }

                    SensorConfiguration *sc = new SensorConfiguration();

                    sensorInfo sInfo;
                    sInfo.name = sensorName;
                    sInfo.plugin = sensorPlugin;
                    dependencies.addActiveSensor(sInfo);

                    this->sensorPad->InstrumentConfig = sc;

                    xml.readNext();
                }


            }else if(xml.name() == "position"){
                if(xml.attributes().hasAttribute("ref")){
                    dependencies.setStationPosition(xml.attributes().value("ref").toInt());
                }
            }else if(xml.name() == "coordinatesystem"){
                if(xml.attributes().hasAttribute("ref")){
                    dependencies.setStationCoordSystem(xml.attributes().value("ref").toInt());
                }
            }

            if(xml.name() == "function"){

               this->readFunction(xml, dependencies);

            }

            if(xml.name() == "member"){

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "member")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes memberAttributes = xml.attributes();

                        if(memberAttributes.hasAttribute("type")){

                            if (memberAttributes.value("type") == "position"){

                                if(memberAttributes.hasAttribute("ref")){
                                    dependencies.setStationPosition(memberAttributes.value("ref").toInt());
                                }

                            }else if (memberAttributes.value("type") == "coordinatesystem"){

                                if(memberAttributes.hasAttribute("ref")){
                                    dependencies.setStationCoordSystem(memberAttributes.value("ref").toInt());
                                }
                            }else{
                                readFeatureAttributes(xml,dependencies);
                            }
                        }
                    }

                xml.readNext();
               }
        }



        }

        xml.readNext();

    }

    return dependencies;*/
}

/*!
 * \brief Station::emitActionFinished
 * \param wasSuccesful
 * will be emitted when the sensor action is completed
 */
void Station::emitActionFinished(bool wasSuccesful){

    emit actionFinished(wasSuccesful);
}

/*!
 * \brief emitStartStream
 */
void Station::emitStartReadingStream(int readingType){
    emit startReadingStream(readingType);
}

/*!
 * \brief Station::stopStream
 */
void Station::emitStopReadingStream(){
    emit stopReadingStream();
}

void Station::emitStartSensorStatsStream()
{
    emit startSensorStatsStream();
}

void Station::emitStopSensorStatsStream()
{
    emit stopSensorStatsStream();
}


/*!
 * \brief Station::emitStartMeasure
 * \param mConfig
 * \param geom
 * starts the measurement defined by the given MeasurementConfig.
 * The determined observation will be saved in the station coordinatesystem and
 * in the given geometry
 */
void Station::emitStartMeasure(Geometry *geom, bool isActiveCoordSys){
    emit startMeasure(geom,isActiveCoordSys);
}

/*!
 * \brief Station::emitStartMove
 * \param azimuth
 * \param zenith
 * \param dist
 * \param isRelativ
 */
void Station::emitStartMove(double azimuth, double zenith, double dist, bool isRelativ){
    emit startMove(azimuth, zenith, dist, isRelativ);
}

/*!
 * \brief Station::emitStartMove
 * \param x
 * \param y
 * \param z
 */
void Station::emitStartMove(double x, double y, double z){
    emit startMove(x, y, z);
}

/*!
 * \brief Station::emitStartInitialize
 */
void Station::emitStartInitialize(){
    emit startInitialize();
}

/*!
 * \brief Station::emitStartMotorState
 */
void Station::emitStartMotorState(){
    emit startMotorState();
}


void Station::emitStartCompensation(){
    emit startCompensation();
}

void Station::emitSelfDefinedAction(QString s)
{
    emit startSelfDefinedAction(s);
}

/*!
 * \brief Station::emitStartHome
 */
void Station::emitStartHome(){
    emit startHome();
}

/*!
 * \brief Station::emitStartToggleSight
 */
void Station::emitStartToggleSight(){
    emit startToggleSight();
}

/*!
 * \brief Station::emitStartConnect
 */
void Station::emitStartConnect(ConnectionConfig *cConfig){
    emit startConnect(cConfig);
}

/*!
 * \brief Station::emitStartDisconnect
 */
void Station::emitStartDisconnect(){
    emit startDisconnect();
}

QString Station::getDisplayX() const{
    return QString::number(this->position->xyz.getAt(0)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString Station::getDisplayY() const{
    return QString::number(this->position->xyz.getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString Station::getDisplayZ() const{
    return QString::number(this->position->xyz.getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString Station::getDisplayIsCommon() const{
    return QString(position->getIsCommon()?"true":"false");
}

QString Station::getDisplayIsNominal() const{
    return QString(position->getIsNominal()?"true":"false");
}

QString Station::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString Station::getDisplayMConfig() const{
    return this->position->getMeasurementConfig().name;
}

QString Station::getDisplayStdDev() const{

    if(this->position->getStatistic().isValid){
        return QString::number(this->position->getStatistic().stdev * UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
    }else{
        return "-/-";
    }
}

void Station::writeToConsole(QString msg){
    emit this->sendToConsole(msg);
}
