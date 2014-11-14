#include "p_leicatachymeter.h"

LeicaTachymeter::LeicaTachymeter(){

    serial = new QSerialPort();

}

PluginMetaData* LeicaTachymeter::getMetaData() const{

    PluginMetaData* metaData = new PluginMetaData();

    metaData->name = "LeicaTachymeter";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "mlux";
    metaData->description = "Plugin zur Steuerung von Leica Tachymetern";
    metaData->iid = "de.openIndy.Plugin.Sensor.TotalStation.v001";
    //...

    return metaData;
}

QList<Configuration::ReadingTypes>* LeicaTachymeter::getSupportedReadingTypes() const{

    QList<Configuration::ReadingTypes> *readingTypes = new QList<Configuration::ReadingTypes>;

    readingTypes->append(Configuration::ePolar);
    readingTypes->append(Configuration::eDirection);
    readingTypes->append(Configuration::eDistance);
    readingTypes->append(Configuration::eCartesian);

    return readingTypes;
}

QList<Configuration::SensorFunctionalities> LeicaTachymeter::getSupportedSensorActions() const
{
    QList<Configuration::SensorFunctionalities> sensorActions;

    sensorActions.append(Configuration::eMoveAngle);
    sensorActions.append(Configuration::eToggleSight);

    return sensorActions;

}

QList<Configuration::ConnectionTypes>* LeicaTachymeter::getConnectionType() const{

    QList<Configuration::ConnectionTypes> *connectionTypes = new QList<Configuration::ConnectionTypes>;
    connectionTypes->append(Configuration::eSerial);

    return connectionTypes;
}

QMap<QString,int>* LeicaTachymeter::getIntegerParameter() const{
    return NULL;
}

QMap<QString,double>* LeicaTachymeter::getDoubleParameter() const{

    //not used yet !!

    /*QMap <QString, double>* doubleParameter = new QMap<QString, double>;

    doubleParameter->insert("addition constant",0.0);

    return doubleParameter;*/
    return NULL;

}

QMap <QString, QStringList>* LeicaTachymeter::getStringParameter() const{

    QMap <QString, QStringList>* stringParameter = new QMap<QString, QStringList>;

    //reflector only means reflectorless or use reflector. it does not specify a specific prism !!!!!
    QStringList reflector;
    reflector.append("reflectorless");
    reflector.append("reflector");

    QStringList ATR;
    ATR.append("atr on");
    ATR.append("atr off");

    QStringList senseOfRotation;
    senseOfRotation.append("mathematical");
    senseOfRotation.append("geodetic");

    stringParameter->insert("reflector",reflector);
    stringParameter->insert("ATR",ATR);
    stringParameter->insert("sense of rotation", senseOfRotation);

    return stringParameter;
}

QStringList LeicaTachymeter::selfDefinedActions() const
{
    QStringList ownActions;

    //actiion to activate the LOCK mode if available.
    ownActions.append("LOCK");
    ownActions.append("stop"); // call this to stop tracking mode

    return ownActions;
}

bool LeicaTachymeter::doSelfDefinedAction(QString a)
{
    if(a == "LOCK"){
        //activate tracking mode
        this->getLOCKState();
    }else if(a == "stop"){
        //stop tracking mode of total station
        QString command = "%R1Q,17017:6\r\n";
        if(executeCommand(command)){
            QString measuredData = this->receive();
            myEmitter.emitSendString("stop tracking");
            myEmitter.emitSendString(measuredData);
        }
    }

    return true;
}


QMap<QString, double>* LeicaTachymeter::getDefaultAccuracy() const
{
   QMap<QString,double> *defaultAccuracy = new QMap<QString,double>;

   defaultAccuracy->insert("sigmaAzimuth",0.000001570);
   defaultAccuracy->insert("sigmaZenith",0.000001570);
   defaultAccuracy->insert("sigmaDistance",0.001);

   return defaultAccuracy;
}

void LeicaTachymeter::abortAction()
{
    //abort action here;
}

//! connect app with laser tracker
bool LeicaTachymeter::connectSensor(ConnectionConfig* connConfig){

    //set port
    this->serial->setPortName(connConfig->comPort);

    if( this->serial->open(QIODevice::ReadWrite) ){

        myEmitter.emitSendString("open port");

        if( this->serial->setBaudRate(connConfig->baudRate)
                && this->serial->setDataBits(connConfig->dataBits)
                && this->serial->setParity(connConfig->parity)
                && this->serial->setFlowControl(connConfig->flowControl)
                && this->serial->setStopBits(connConfig->stopBits) ){

            myEmitter.emitSendString("port properties set");
            myEmitter.emitSendString("connection open");
            return true;
        }
      }

    return false;

}

//! disconnect app with laser tracker
bool LeicaTachymeter::disconnectSensor(){

    if(this->serial->isOpen()){
       this->serial->close();
        myEmitter.emitSendString("connection closed");
    }

    return true;
}

bool LeicaTachymeter::toggleSightOrientation(){

    if( this->serial->isOpen()){

        QString command = "%R1Q,9028:0,0,0\r\n";
        //combine the string command


            if(executeCommand(command)){
             myEmitter.emitSendString("changing side");
             QString measureData = this->receive();
             myEmitter.emitSendString(measureData);

             return true;
            }
    }
    return false;
}

bool LeicaTachymeter::getATRState()
{
    if(this->serial->isOpen()){

        //check user defined ATR value
        if(!this->myConfiguration->stringParameter.contains("ATR")){
            return false;
        }

        //check ATR state
        QString command = "%R1Q,18006:\r\n";

        if(executeCommand(command)){
            QString measureData = this->receive();
            //split to get return code and on/off state
            QStringList responseElements = measureData.split(":");
            QStringList elements = responseElements.at(1).split(",");

            //if return code of function = 0 (ATR supported)
            if(elements.at(0).compare("0") == 0){
                if(this->setATRState(elements.at(1))){
                    return true;
                }
                //return true;
            }else{
                return false;
            }
        }
        return false;
    }else{
        return false;
    }
}

bool LeicaTachymeter::setATRState(QString ATRstate)
{
    QString command = QString("%R1Q,18005:" +QString::number(ATRstate.toInt()) + "\r\n") ;
    //user defined ATR value
    QString value = this->myConfiguration->stringParameter.value("ATR");

    if(value.compare("atr on") == 0){
        value = "1";
    }else if(value.compare("atr off") == 0){
        value = "0";
    }

    //compare current ATR state and defined ATR state and change if needed
    if(QString::number(ATRstate.toInt()).compare(value) != 0){

        command = "%R1Q,18005:" + value + "\r\n";

        bool result = this->checkCommandRC(command);
        if(result){
            myEmitter.emitSendString("ATR state changed.");
            return true;
        }else{
            return false;
        }
    }
    return true;
}

bool LeicaTachymeter::getLOCKState()
{
    if(this->serial->isOpen()){

        //check user defined ATR value
        if(!this->myConfiguration->stringParameter.contains("ATR")){
            return false;
        }

        //getUserLockState
        QString command = "%R1Q,18008:\r\n";

        if(executeCommand(command)){
            QString measureData = this->receive();
            QStringList responseElements = measureData.split(":");
            QStringList elements = responseElements.at(1).split(",");

            //if LOCK is supported
            if(elements.at(0).compare("0") == 0){
                if(this->setLOCKState(elements.at(1))){
                    return true;
                }
                //return true;
            }else{
                return false;
            }
        }
        return false;
    }else{
        myEmitter.emitSendString("not connected");
        return false;
    }
}

/*!
 * \brief deactiveLockState
 */
void LeicaTachymeter::deactiveLockState()
{
    /*QString atrOn = this->myConfiguration->stringParameter.value("ATR");

    if(atrOn.compare("atr on") == 0){
        QString command = "%R1Q,18007:0\r\n";
        this->executeCommand(command);
    }*/
}

bool LeicaTachymeter::setLOCKState(QString currentState)
{
    QString command = QString("%R1Q,18007:" + QString::number(currentState.toInt()) + "\r\n");

    QString value = this->myConfiguration->stringParameter.value("ATR");

    if(value.compare("atr on") == 0){
        value = "1";
    }else{
        value = "0";
    }

    if(QString::number(currentState.toInt()).compare(value) != 0){
        command = "%R1Q,18007:" + value + "\r\n";

        bool result = this->checkCommandRC(command);
        if(result){
            myEmitter.emitSendString("atr and locke state changed");
            return true;
        }else{
            return false;
        }
    }
    return true;

    /*bool on = false;
    //if is currently off, then turn on
    if(currentState.compare("0") == 0){
        myEmitter.emitSendString("activating LOCK mode.");
        command = "%R1Q,18007:1\r\n";
        on = true;

    //if is currently on, then turn off
    }else if(currentState.compare("1") == 0){
        myEmitter.emitSendString("deactivating LOCK mode.");
        command = "%R1Q,18007:0\r\n";
        on = false;
    }

    bool result = this->checkCommandRC(command);
    if(result){
        myEmitter.emitSendString("LOCK state changed");
        if(on){
            //if current state is "on", then fine adjust
            this->fineAdjust();
        }
        return true;
    }else{
        return false;
    }*/
}

bool LeicaTachymeter::startTargetTracking()
{
    QString command = "%R1Q,9013:\r\n";

    bool result = this->checkCommandRC(command);
    if(result){
        myEmitter.emitSendString("target tracking active.");
        return true;
    }else{
        return false;
    }
}

bool LeicaTachymeter::fineAdjust()
{
    QString command = "%R1Q,9037:0.08,0.08,0\r\n";

    //execute fine adjust command
    bool result = this->checkCommandRC(command);
    if(result){
        myEmitter.emitSendString("fine adjust successful.");
        //if successful then run target tracking.
        this->startTargetTracking();
        return true;
    }else{
        return false;
    }
}

/* //handle with set target type function!!
int LeicaTachymeter::getEDMMode()
{
    QString command = "%R1Q,2021:\r\n";
    if(executeCommand(command)){
        QString measureData = this->receive();
        QStringList elements = measureData.split(":");
        elements = elements.at(1).split(",");

        return elements.at(1).toInt();
    }
    return -1;
}*/

bool LeicaTachymeter::move(double azimuth, double zenith, double distance,bool isrelativ){

    if(isrelativ){

    }else{

        if( this->serial->isOpen()){

            if(azimuth <= 0.0){
                azimuth = 2*PI + azimuth;
            }

            if(this->myConfiguration->stringParameter.contains("sense of rotation")){
                QString sense =  this->myConfiguration->stringParameter.value("sense of rotation");
                if(sense.compare("mathematical") == 0){
                    azimuth = 2*PI - azimuth;
                }
            }

            QString command="%R1Q,9027:";
            command.append(QString::number(azimuth));
            command.append(",");
            command.append(QString::number(zenith));
            command.append(",0,0,0\r\n");

            if(executeCommand(command)){
                myEmitter.emitSendString("moving...");
                QString measureData = this->receive();
                myEmitter.emitSendString("completed.");
                myEmitter.emitSendString(measureData );
            }
        }
    }
    return true;
}

bool LeicaTachymeter::move(double x, double y, double z)
{
    //convert xyz to azimuth, zenith and distance for total station
    Reading *r;
    r = new Reading();
    OiVec p = r->toPolar(x, y, z);
    r->typeofReading = Configuration::ePolar;
    r->rPolar.azimuth = p.getAt(0);
    r->rPolar.zenith = p.getAt(1);
    r->rPolar.distance = p.getAt(2);
    r->rPolar.isValid = true;


    if(this->serial->isOpen()){

        if(r->rPolar.azimuth <= 0.0){
            r->rPolar.azimuth = 2*PI + r->rPolar.azimuth;
        }

        if(this->myConfiguration->stringParameter.contains("sense of rotation")){
            QString sense =  this->myConfiguration->stringParameter.value("sense of rotation");
            if(sense.compare("mathematical") == 0){
                r->rPolar.azimuth = 2*PI - r->rPolar.azimuth;
            }
        }

        QString command = "%R1Q,9027:";
        command.append(QString::number(r->rPolar.azimuth));
        command.append(",");
        command.append(QString::number(r->rPolar.zenith));
        command.append(",0,0,0\r\n");

        if(executeCommand(command)){
            myEmitter.emitSendString("moving...");
            QString measureData = this->receive();
            myEmitter.emitSendString("completed");
            myEmitter.emitSendString(measureData);
        }
    }
    return true;
}

QList<Reading*> LeicaTachymeter::measure(MeasurementConfig *m){

    //check and set ATR state
    //TODO run at each measurement?? if not maybe reactivate after tracking needed
    //this->getATRState();


    //set target to specified value
    if(this->setTargetTypeMeasure()){

        switch (m->typeOfReading){
        case Configuration::ePolar:
            return this->measurePolar(m);
            break;
        case Configuration::eCartesian:
            return this->measureCartesian(m);
            break;
        case Configuration::eDirection:
            return this->measureDirection(m);
            break;
        case Configuration::eDistance:
            return this->measureDistance(m);
            break;
        default:
            break;
        }
    }

    QList<Reading*> emptyList;
    return emptyList;
}

QVariantMap LeicaTachymeter::readingStream(Configuration::ReadingTypes streamFormat)
{

    QVariantMap m;

    Reading *r = NULL;

    //set target to specified value
    if(this->setTargetTypeStream()){

        MeasurementConfig *mconfig;
        mconfig = new MeasurementConfig();

        mconfig->count =1;
        mconfig->iterations =1;
        mconfig->measureTwoSides = false;
        mconfig->typeOfReading = Configuration::ePolar;

        r = this->measurePolar(mconfig).first();

        QThread::msleep(100);

        if(r == NULL){
            return m;
        }

        switch (streamFormat) {
        case Configuration::ePolar:
            m.insert("azimuth", r->rPolar.azimuth);
            m.insert("zenith", r->rPolar.zenith);
            m.insert("distance", r->rPolar.distance);
            break;
        case Configuration::eCartesian:
            r->toCartesian();
            m.insert("x", r->rCartesian.xyz.getAt(0));
            m.insert("y", r->rCartesian.xyz.getAt(1));
            m.insert("z", r->rCartesian.xyz.getAt(2));
            break;
        case Configuration::eDirection:
            m.insert("azimuth", r->rPolar.azimuth);
            m.insert("zenith", r->rPolar.zenith);
            break;
        case Configuration::eDistance:
            m.insert("distance", r->rPolar.distance);
            break;
        default:
            break;
        }
    }
    return m;
}

bool LeicaTachymeter::getConnectionState()
{
    return this->serial->isOpen();
}

bool LeicaTachymeter::isReadyForMeasurement()
{
    return this->getConnectionState();
}

QMap<QString, QString> LeicaTachymeter::getSensorStats()
{
    QMap<QString, QString> stats;

    stats.insert("connected", QString::number(this->serial->isOpen()));

    return stats;
}

bool LeicaTachymeter::isBusy()
{
    return false;
}

QList<Reading*> LeicaTachymeter::measurePolar(MeasurementConfig *m){

    QList<Reading*> readings;

    int faceCount = 1;

    if (m->measureTwoSides){
       faceCount = 2;
    }

    if( this->serial->isOpen()){

        for(int i=0;i<m->iterations;i++){
            for(int k = 0; k<faceCount;k++){
                if (this->executeEDM()){

                    Reading *r;
                    r = new Reading();
                    QString command = "%R1Q,2108:5000,1\r\n";
                    myEmitter.emitSendString("edm succesful");

                    if(executeCommand(command)){

                        QString measureData = this->receive();

                        myEmitter.emitSendString(measureData);

                        QStringList polarElements = measureData.split(",");

                        r->rPolar.azimuth = polarElements.at(polarElements.size()-3).toDouble();
                        r->rPolar.zenith = polarElements.at(polarElements.size()-2).toDouble();
                        r->rPolar.distance = polarElements.at(polarElements.size()-1).toDouble();
                        r->typeofReading = Configuration::ePolar;
                        r->face = this->getCurrentFace(r->rPolar.zenith);

                        if(this->myConfiguration->stringParameter.contains("sense of rotation")){
                            QString sense =  this->myConfiguration->stringParameter.value("sense of rotation");
                            if(sense.compare("mathematical") == 0){
                                r->rPolar.azimuth = 2 * PI - r->rPolar.azimuth;
                            }
                        }
                        r->rPolar.isValid = true;
                        r->instrument = this;
                        r->measuredAt = QDateTime::currentDateTime();
                        readings.append(r);

                        if(faceCount == 2){
                            this->toggleSightOrientation();
                        }
                    }
                }
            }
        }
    }
    this->deactiveLockState();
    return readings;
}

QList<Reading*> LeicaTachymeter::measureDistance(MeasurementConfig *m){

    QList<Reading*> readings;

    int faceCount = 1;

    if (m->measureTwoSides){
       faceCount = 2;
    }

    if( this->serial->isOpen()){

        for(int i=0;i<m->iterations;i++){
            for(int k = 0; k<faceCount;k++){
                if (this->executeEDM()){

                    Reading *r;
                    r = new Reading();
                    QString command = "%R1Q,2108:5000,1\r\n";
                    myEmitter.emitSendString("edm succesful");

                    if(executeCommand(command)){

                        QString measureData = this->receive();

                        myEmitter.emitSendString(measureData);

                        QStringList polarElements = measureData.split(",");

                        double zenith = polarElements.at(polarElements.size()-2).toDouble();
                        r->rDistance.distance = polarElements.at(polarElements.size()-1).toDouble();
                        r->typeofReading = Configuration::eDistance;
                        r->face = this->getCurrentFace(zenith);

                        r->rDistance.isValid = true;
                        r->instrument = this;
                        r->measuredAt = QDateTime::currentDateTime();
                        readings.append(r);

                        if(faceCount == 2){
                            this->toggleSightOrientation();
                        }
                    }
                }
            }
        }
    }
    this->deactiveLockState();
    return readings;
}

QList<Reading*> LeicaTachymeter::measureDirection(MeasurementConfig *m){

    QList<Reading*> readings;

    int faceCount = 1;

    if (m->measureTwoSides){
       faceCount = 2;
    }


    if( this->serial->isOpen()){

        for(int i=0;i<m->iterations;i++){
            for(int k = 0; k<faceCount;k++){

                Reading *r;
                r = new Reading();

                QString command = "%R1Q,2107:1\r\n";

                if(executeCommand(command)){

                    QString measureData = this->receive();

                    myEmitter.emitSendString(measureData);

                    QStringList polarElements = measureData.split(",");

                    r->rDirection.azimuth = polarElements.at(polarElements.size()-2).toDouble();
                    r->rDirection.zenith = polarElements.at(polarElements.size()-1).toDouble();
                    r->typeofReading = Configuration::eDirection;
                    r->face = this->getCurrentFace(r->rDirection.zenith);

                    if(this->myConfiguration->stringParameter.contains("sense of rotation")){
                        QString sense =  this->myConfiguration->stringParameter.value("sense of rotation");
                        if(sense.compare("mathematical") == 0){
                            r->rDirection.azimuth = 2 * PI - r->rDirection.azimuth;
                        }
                    }
                    r->rDirection.isValid = true;
                    r->instrument = this;
                    r->measuredAt = QDateTime::currentDateTime();
                    readings.append(r);

                    if(faceCount == 2){
                        this->toggleSightOrientation();
                    }
                }
            }
        }
    }
    this->deactiveLockState();
    return readings;
}

QList<Reading*> LeicaTachymeter::measureCartesian(MeasurementConfig *m){

    QList<Reading*> readings = this->measurePolar(m);

    for(int i = 0; i<readings.size();i++){
        readings.at(i)->toCartesian();
        readings.at(i)->rCartesian.isValid = true;
        readings.at(i)->typeofReading = Configuration::eCartesian;
    }

    this->deactiveLockState();
    return readings;

}

QString LeicaTachymeter::receive(){
    QByteArray responseData = this->serial->readAll();
    while (this->serial->waitForReadyRead(100))
        responseData += this->serial->readAll();
    QString response = QString(responseData);
    return response;
}

void LeicaTachymeter::getError(QSerialPort::SerialPortError e){
    qDebug() << e;
    this->serial->close();
}

bool LeicaTachymeter::executeCommand(QString command){

    QByteArray data = command.toLocal8Bit();

    this->serial->write(data, data.length());

    if(this->serial->waitForBytesWritten(10000)){
        if(this->serial->waitForReadyRead(10000)){
            return true;
        }
    }

    return false;
}

/*!
 * \brief checkCommandRC executes the command and checks if it was successfully via the RC of the function
 * \param command
 * \return
 */
bool LeicaTachymeter::checkCommandRC(QString command)
{
    if(executeCommand(command)){
        QString measureData = this->receive();

        QStringList responseElements = measureData.split(":");
        QStringList elements = responseElements.at(1).split(",");

        if(elements.at(0).compare("0") == 0){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

/*!
 * \brief getCurrentFace returns front sight or back sight, depending on current sight of instrument
 * \param zenith
 * \return
 */
Configuration::Faces LeicaTachymeter::getCurrentFace(double zenith)
{
    if((zenith >= 0.0) && (zenith < PI)){
        return Configuration::eFrontside;
    }else{
        return Configuration::eBackside;
    }
}

/*!
 * \brief setTargetType checks current target type and switches to current selected, if needed.
 * 0 = with reflector; 1 = reflectorless. sets standard mode for measurements
 */
bool LeicaTachymeter::setTargetTypeMeasure()
{
    QString refl = "";
    bool reflless = false;

    if(this->myConfiguration->stringParameter.contains("reflector")){
        refl = this->myConfiguration->stringParameter.value("reflector");
    }

    //check if reflectorless was selected
    if(refl.compare("reflectorless") == 0){
        reflless = true;
    }

    //get current setting if IR or RL standard or tracking
    QString command = "%R1Q,17018:\r\n";
    if(this->executeCommand(command)){
        QString receive = this->receive();


        if(reflless){
            if(receive.compare("%R1P,0,0:0,3\r\n") != 0){ //if not RL and standard
                //switch to reflectorless standard
                command = "%R1Q,17019:3\r\n";
                if(this->executeCommand(command)){
                    receive = this->receive();
                    return true;
                }
            }
            return true;
        }else{
            if(receive.compare("%R1P,0,0:0,0\r\n") != 0){ // if not IR and standard
                //switch to reflector standard
                command = "%R1Q,17019:0\r\n";
                if(this->executeCommand(command)){
                    receive = this->receive();
                    //return true;
                }
            }
            /*if(this->getATRState()){
                return true;
            }*/
            if(this->getLOCKState()){
                return true;
            }
        }
        //return true;
    }
    return false;
}

/*!
 * \brief setTargetTypeStream set tracking measure mode for reading stream
 * \return
 */
bool LeicaTachymeter::setTargetTypeStream()
{
    QString refl = "";
    bool reflless = false;

    if(this->myConfiguration->stringParameter.contains("reflector")){
        refl = this->myConfiguration->stringParameter.value("reflector");
    }

    //check if reflectorless was selected
    if(refl.compare("reflectorless") == 0){
        reflless = true;
    }

    //get current setting if IR or RL standard or tracking
    QString command = "%R1Q,17018:\r\n";
    if(this->executeCommand(command)){
        QString receive = this->receive();


        if(reflless){
            if(receive.compare("%R1P,0,0:0,6\r\n") != 0){ //if not RL and tracking
                //switch to reflectorless tracking
                command = "%R1Q,17019:6\r\n";
                if(this->executeCommand(command)){
                    receive = this->receive();
                    return true;
                }
            }
            return true;
        }else{
            if(receive.compare("%R1P,0,0:0,4\r\n") != 0){ // if not IR and tracking
            //switch to reflector tracking
                command = "%R1Q,17019:4\r\n";
                if(this->executeCommand(command)){
                    receive = this->receive();
                    //return true;
                }
            }
            if(this->getATRState()){
                return true;
            }
        }
        //return true;
    }
    return false;
}

bool LeicaTachymeter::executeEDM(){

    myEmitter.emitSendString("start edm measurement");

    QString edmCommand = "%R1Q,2008:1,1\r\n";

    if(this->executeCommand(edmCommand)){
        QString receive = this->receive();
        if(receive.compare("%R1P,0,0:0\r\n") == 0){
            return true;
        }
    }
    return false;
}
