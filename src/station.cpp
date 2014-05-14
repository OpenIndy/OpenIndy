#include "station.h"
#include <QVariant>

Station::Station(QString name)
{
    this->name = name;

    this->id = Configuration::generateID();
    position = new Point();
    position->name = name;

    //ini member
    coordSys = new CoordinateSystem();
    coordSys->name = this->name;

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
    connect(this,SIGNAL(startReadingStream(Configuration::ReadingTypes)),this->sensorPad,SLOT(readingStream(Configuration::ReadingTypes)));
    connect(this,SIGNAL(startSensorStatsStream()),this->sensorPad,SLOT(sensorStatsStream()));
    connect(this,SIGNAL(startSelfDefinedAction(QString)),this->sensorPad,SLOT(doSelfDefinedAction(QString)));
    connect(this,SIGNAL(stopReadingStream()),this->sensorPad,SLOT(stopReadingStream()));
    connect(this,SIGNAL(stopSensorStatsStream()),this->sensorPad,SLOT(stopStatStream()));

    //start the station thread
    stationThread.start();

    this->isUpdated = false;
    this->isDrawn = true;

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

bool Station::toOpenIndyXML(QXmlStreamWriter &stream){

        stream.writeStartElement("station");
        stream.writeAttribute("id", QString::number(this->id));
        stream.writeAttribute("name", this->name);

        if(this->sensorPad->instrument != NULL){
            stream.writeStartElement("activeSensor");
            stream.writeAttribute("name",this->sensorPad->instrument->getMetaData()->name);
            stream.writeAttribute("plugin", this->sensorPad->instrument->getMetaData()->pluginName);
            stream.writeEndElement();
        }

        if(this->sensorPad->usedSensors.size()>0){
            for(int i = 0; i<this->sensorPad->usedSensors.size();i++){
                stream.writeStartElement("sensor");
                stream.writeAttribute("name",this->sensorPad->usedSensors.at(i)->getMetaData()->name);
                stream.writeAttribute("plugin", this->sensorPad->usedSensors.at(i)->getMetaData()->pluginName);
                stream.writeEndElement();
            }
        }

        if(this->position != NULL){

            stream.writeStartElement("member");
            stream.writeAttribute("type", "position");
            stream.writeAttribute("ref", QString::number(this->position->id));
            stream.writeEndElement();

        }

        if(this->coordSys != NULL){
            stream.writeStartElement("member");
            stream.writeAttribute("type", "coordinatesystem");
            stream.writeAttribute("ref", QString::number(this->coordSys->id));
            stream.writeEndElement();
        }

        this->writeFeatureAttributes(stream);

        stream.writeEndElement();

        return true;

}

ElementDependencies Station::fromOpenIndyXML(QXmlStreamReader &xml){

    ElementDependencies dependencies;


    QXmlStreamAttributes attributes = xml.attributes();

    if(attributes.hasAttribute("name")){
        this->name = attributes.value("name").toString();
    }
    if(attributes.hasAttribute("id")) {
        this->id = attributes.value("id").toInt();
        dependencies.elementID = this->id;
        dependencies.typeOfElement = Configuration::eStationElement;
    }

    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named station.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "station")) {

        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found first name. */
            if(xml.name() == "sensor") {

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

                    /* ...and next... */
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
                    /* ...and next... */
                    xml.readNext();
                }


            }

            if(xml.name() == "member"){

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "member")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes memberAttributes = xml.attributes();

                        if(memberAttributes.hasAttribute("type")){

                            if (memberAttributes.value("type") == "position"){

                                if(memberAttributes.hasAttribute("ref")){
                                    this->position->id = memberAttributes.value("ref").toInt();
                                }

                            }else if (memberAttributes.value("type") == "coordinatesystem"){

                                if(memberAttributes.hasAttribute("ref")){
                                    this->coordSys->id = memberAttributes.value("ref").toInt();
                                }
                            }else{
                                readFeatureAttributes(xml,dependencies);
                            }
                        }
                    }
                /* ...and next... */
                xml.readNext();
               }
        }

        if(xml.name() == "function"){

           this->readFunction(xml, dependencies);

        }
        /* ...and next... */
     }

    xml.readNext();

    }

    return dependencies;
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
void Station::emitStartReadingStream(Configuration::ReadingTypes r){
    emit startReadingStream(r);
}

/*!
 * \brief Station::stopStream
 */
void Station::emitStopReadingStream(){
    sensorPad->isReadingStreamActive = false;
    emit actionFinished(true);
}

void Station::emitStartSensorStatsStream()
{
    emit startSensorStatsStream();
}

void Station::emitStopSensorStatsStream()
{
    sensorPad->isSensorStatStreamActive = false;
    emit actionFinished(true);
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
    return QString(position->isCommon?"true":"false");
}

QString Station::getDisplayIsNominal() const{
    return QString(position->isNominal?"true":"false");
}

QString Station::getDisplayObs() const{
    return QString::number(this->position->myObservations.size());
}

QString Station::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString Station::getDisplayMConfig() const{
    return this->position->mConfig.name;
}

QString Station::getDisplayStdDev() const{

    if(this->position->myStatistic.isValid){
        return QString::number(this->position->myStatistic.stdev * UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
    }else{
        return "-/-";
    }
}

void Station::writeToConsole(QString msg){
    emit this->sendToConsole(msg);
}
