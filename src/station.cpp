#include "station.h"
#include <QVariant>

Station::Station(QString name)
{
    this->name = name;

    this->id = Configuration::generateID();
    position = new Point();

    //ini member
    coordSys = new CoordinateSystem();
    coordSys->name = this->name;

    instrument = NULL;
    sensorPad = new SensorControl(this);
    //InstrumentConfig = new SensorConfiguration; //TODO null pointer??
    InstrumentConfig = NULL;

    //move controller to the station thread
    sensorPad->moveToThread(&stationThread);

    //connections between station and sensorcontrol
    connect(this->sensorPad,SIGNAL(commandFinished(bool)),this,SLOT(emitActionFinished(bool)));

    connect(this,SIGNAL(startMeasure(Geometry*,bool)),this->sensorPad,SLOT(measure(Geometry*,bool)));
    connect(this,SIGNAL(startStream()),this->sensorPad,SLOT(stream()));
    connect(this,SIGNAL(startMove(double,double,double,bool)),this->sensorPad,SLOT(move(double,double,double,bool)));
    connect(this,SIGNAL(startMove(double,double,double)),this->sensorPad,SLOT(move(double,double,double)));
    connect(this,SIGNAL(startInitialize()),this->sensorPad,SLOT(initialize()));
    connect(this,SIGNAL(startMotorState(bool)),this->sensorPad,SLOT(motorState(bool)));
    connect(this,SIGNAL(startHome()),this->sensorPad,SLOT(home()));
    connect(this,SIGNAL(startToggleSight()),this->sensorPad,SLOT(toggleSight()));
    connect(this,SIGNAL(startCompensation()),this->sensorPad,SLOT(compensation()));
    connect(this,SIGNAL(startSendCommand(QString)),this->sensorPad,SLOT(sendCommandString(QString)));
    connect(this,SIGNAL(startConnect(ConnectionConfig*)),this->sensorPad,SLOT(connectSensor(ConnectionConfig*)));
    connect(this,SIGNAL(startDisconnect()),this->sensorPad,SLOT(disconnectSensor()));

    //start the station thread
    stationThread.start();

    this->isUpdated = false;
    this->isDrawn = true;

}

Station::~Station(){

    stationThread.quit();
    stationThread.wait();
}

void Station::stopThread(){
    stationThread.quit();
    stationThread.wait();
}

void Station::startThread(){
    stationThread.start();
}

void Station::setInstrumentConfig(SensorConfiguration *sensorConfig){

    InstrumentConfig = sensorConfig;
    instrument->setSensorConfiguration(sensorConfig);

}

SensorConfiguration* Station::getInstrumentConfig(){
    return InstrumentConfig;
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

        if(this->instrument != NULL){
            stream.writeStartElement("sensor");
            stream.writeAttribute("name",this->instrument->getMetaData()->name);
            stream.writeAttribute("plugin", this->instrument->getMetaData()->pluginName);
            stream.writeEndElement();
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

                    //toDO emitSignal and fill sc
                    //QString sensorPath = SystemDbManager::getPluginFilePath(sensorName,sensorPlugin);
                    //s->instrument = PluginLoader::loadSensorPlugin(sensorPath,sensorName);
                    this->InstrumentConfig = sc;
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
                    /* ...and next... */
                    xml.readNext();
                }

            }

            if(xml.name() == "function"){

                this->readFunction(xml, dependencies);

            }


        }
        /* ...and next... */
        xml.readNext();
    }


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
void Station::emitStartStream(){
    emit startStream();
}

/*!
 * \brief Station::stopStream
 */
void Station::stopStream(){
    QThread::msleep(1);
    this->instrument->dataStreamIsActive = false;
    emit actionFinished(true);
}

void Station::emitStartCommand(QString cmd){
    emit startSendCommand(cmd);
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
void Station::emitStartMotorState(bool b){
    emit startMotorState(b);
}


void Station::emitStartCompensation(){
    emit startCompensation();
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
