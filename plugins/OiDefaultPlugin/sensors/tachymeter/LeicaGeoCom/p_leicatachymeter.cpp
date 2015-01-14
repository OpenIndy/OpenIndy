#include "p_leicatachymeter.h"

LeicaTachymeter::LeicaTachymeter(){

    serial = new QSerialPort();
    laserOn = false;
    fineAdjusted = false;
    this->watchWindowOpen = false;
    this->measureWatchWindow = false;
}

/*!
 * \brief getMetaData
 * \return
 */
PluginMetaData* LeicaTachymeter::getMetaData() const{

    PluginMetaData* metaData = new PluginMetaData();

    metaData->name = "LeicaTachymeter";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "OiProject";
    metaData->description = "Plugin zur Steuerung von Leica Tachymetern";
    metaData->iid = "de.openIndy.Plugin.Sensor.TotalStation.v001";

    return metaData;
}

/*!
 * \brief getSupportedReadingTypes
 * \return
 */
QList<Configuration::ReadingTypes>* LeicaTachymeter::getSupportedReadingTypes() const{

    QList<Configuration::ReadingTypes> *readingTypes = new QList<Configuration::ReadingTypes>;

    readingTypes->append(Configuration::ePolar);
    readingTypes->append(Configuration::eDirection);
    readingTypes->append(Configuration::eDistance);
    readingTypes->append(Configuration::eCartesian);

    return readingTypes;
}

/*!
 * \brief getSupportedSensorActions
 * \return
 */
QList<Configuration::SensorFunctionalities> LeicaTachymeter::getSupportedSensorActions() const
{
    QList<Configuration::SensorFunctionalities> sensorActions;

    sensorActions.append(Configuration::eMoveAngle);
    sensorActions.append(Configuration::eToggleSight);

    return sensorActions;

}

/*!
 * \brief getConnectionType
 * \return
 */
QList<Configuration::ConnectionTypes>* LeicaTachymeter::getConnectionType() const{

    QList<Configuration::ConnectionTypes> *connectionTypes = new QList<Configuration::ConnectionTypes>;
    connectionTypes->append(Configuration::eSerial);

    return connectionTypes;
}

/*!
 * \brief getIntegerParameter
 * \return
 */
QMap<QString,int>* LeicaTachymeter::getIntegerParameter() const{
    return NULL;
}

/*!
 * \brief getDoubleParameter
 * \return
 */
QMap<QString,double>* LeicaTachymeter::getDoubleParameter() const{

    return NULL;

}

/*!
 * \brief getStringParameter
 * \return
 */
QMap <QString, QStringList>* LeicaTachymeter::getStringParameter() const{

    QMap <QString, QStringList>* stringParameter = new QMap<QString, QStringList>;

    //reflector only means reflectorless or use reflector. it does not specify a specific prism !!!!!
    QStringList reflector;
    reflector.append("reflector");
    reflector.append("reflectorless");

    //user defined ATR state
    QStringList ATR;
    ATR.append("atr on");
    ATR.append("atr off");

    //user defined tracking state/ mode
    QStringList tracking;
    tracking.append("no");
    tracking.append("yes");

    //user defined tracking accuracy
    QStringList trackAccuracy;
    trackAccuracy.append("point tolerance");
    trackAccuracy.append("angle tolerance");

    //user defined coordinate system definition
    QStringList senseOfRotation;
    senseOfRotation.append("mathematical");
    senseOfRotation.append("geodetic");

    //user defined measure mode
    QStringList measureMode;
    measureMode.append("precise");
    measureMode.append("fast");

    stringParameter->insert("reflector",reflector);
    stringParameter->insert("ATR",ATR);
    stringParameter->insert("stop tracking after measurement", tracking);
    stringParameter->insert("ATR accuracy", trackAccuracy);
    stringParameter->insert("sense of rotation", senseOfRotation);
    stringParameter->insert("measure mode", measureMode);

    return stringParameter;
}

/*!
 * \brief selfDefinedActions defines actions of the total station that are not defined in the sensor class
 * \return
 */
QStringList LeicaTachymeter::selfDefinedActions() const
{
    QStringList ownActions;

    //action to activate the LOCK mode if available.
    ownActions.append("lock to prism"); //start tracking
    ownActions.append("stop prism lock"); //stop tracking
    ownActions.append("stop measurement"); // call this to stop tracking mode
    ownActions.append("laserPointer"); //turn on/off laser pointer

    return ownActions;
}

/*!
 * \brief doSelfDefinedAction calls the self defined actions
 * \param a
 * \return
 */
bool LeicaTachymeter::doSelfDefinedAction(QString a)
{
    if(a == "lock to prism"){

        //first turn laser off
        if(laserOn){
            this->deactivateLaserPointer();
        }
        //activate lock mode for prism tracking
        this->getLOCKState();

    }else if(a == "stop prism lock"){

        //stop lock mode for prism tracking
        this->deactiveLockState();

    }else if(a == "stop measurement"){

        this->stopTrackingMode();
        this->watchWindowOpen = false;

    }else if(a == "laserPointer"){

        if(laserOn){
            this->deactivateLaserPointer();
        }else{
            //first stop tracking
            this->deactiveLockState();
            this->activateLaserPointer();
        }
    }
    return true;
}

/*!
 * \brief getDefaultAccuracy
 * \return
 */
QMap<QString, double>* LeicaTachymeter::getDefaultAccuracy() const
{
   QMap<QString,double> *defaultAccuracy = new QMap<QString,double>;

   defaultAccuracy->insert("sigmaAzimuth",0.000001570);
   defaultAccuracy->insert("sigmaZenith",0.000001570);
   defaultAccuracy->insert("sigmaDistance",0.001);

   return defaultAccuracy;
}

/*!
 * \brief abortAction
 */
void LeicaTachymeter::abortAction()
{
    //abort action here;
}

/*!
 * \brief connectSensor
 * \param connConfig
 * \return
 */
bool LeicaTachymeter::connectSensor(ConnectionConfig* connConfig){

    //set fineAdjusted to false
    this->fineAdjusted = false;

    //set port
    this->serial->setPortName(connConfig->comPort);

    if( this->serial->open(QIODevice::ReadWrite) ){

        if( this->serial->setBaudRate(connConfig->baudRate)
                && this->serial->setDataBits(connConfig->dataBits)
                && this->serial->setParity(connConfig->parity)
                && this->serial->setFlowControl(connConfig->flowControl)
                && this->serial->setStopBits(connConfig->stopBits) ){

            myEmitter.emitSendString("connected");
            return true;
        }
    }

    return false;

}

/*!
 * \brief disconnectSensor
 * \return
 */
bool LeicaTachymeter::disconnectSensor(){

    if(this->serial->isOpen()){
       this->serial->close();
        myEmitter.emitSendString("connection closed");
    }

    return true;
}

/*!
 * \brief toggleSightOrientation
 * \return
 */
bool LeicaTachymeter::toggleSightOrientation(){

    if( this->serial->isOpen()){

        QString command = "%R1Q,9028:0,0,0\r\n";
        //combine the string command


            if(executeCommand(command)){
                myEmitter.emitSendString("toggle sight");
                QString measureData = this->receive();

             return true;
            }
    }
    return false;
}

/*!
 * \brief getLOCKState returns the current lock state. Means, if the total station should lock and follow a prism or not. This function also activates total stations ATR mode
 * \return
 */
bool LeicaTachymeter::getLOCKState()
{
    if(this->serial->isOpen()){

        //check user defined ATR value
        if(!this->myConfiguration.stringParameter.contains("ATR")){ //only if atr is on
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
                //get current state and set to specified state
                if(this->setLOCKState(elements.at(1))){
                    return true;
                }
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
 * \brief deactiveLockState deactivates lock and atr mode of the total station.
 */
void LeicaTachymeter::deactiveLockState()
{
    QString atrOn = this->myConfiguration.stringParameter.value("ATR");

    if(atrOn.compare("atr on") == 0){
        //if ATR is on, then deactivate lock mode
        QString command = "%R1Q,18007:0\r\n";
        this->executeCommand(command);
    }
}

/*!
 * \brief setLOCKState sets current lock state to specified lock state.
 * \param currentState
 * \return
 */
bool LeicaTachymeter::setLOCKState(QString currentState)
{
    QString command = QString("%R1Q,18007:" + QString::number(currentState.toInt()) + "\r\n");

    QString value = this->myConfiguration.stringParameter.value("ATR");

    if(value.compare("atr on") == 0){
        value = "1";
    }else{
        value = "0";
    }

    if(QString::number(currentState.toInt()).compare(value) != 0){

        //if current state is not the same as specified
        command = "%R1Q,18007:" + value + "\r\n";

        bool result = this->checkCommandRC(command);
        if(result){
            myEmitter.emitSendString("atr and lock state changed");
        }else{
            return false;
        }
    }

    //if ATR is on, then start target/prism tracking
    if(value.compare("1") == 0){
        return this->startTargetTracking();
    }
    return true;
}

/*!
 * \brief startTargetTracking locks onto the prism and tracks it.
 * \return
 */
bool LeicaTachymeter::startTargetTracking()
{
    //first set fine adjust mode
    if(!this->setAdjustMode()){
        return false;
    }
    //then do fine adjust
    if(this->fineAdjust()){

        QString command = "%R1Q,9013:\r\n";

        bool result = this->checkCommandRC(command);
        if(result){
            myEmitter.emitSendString("target tracking active.");
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

/*!
 * \brief fineAdjust needs to be called before tracking can be activated.
 * \return
 */
bool LeicaTachymeter::fineAdjust()
{
    //don´t need to call every measurement
    //check if fineadjust was called before
    if(this->fineAdjusted){
        return true;
    }
    //call fine adjust with dH and dVZ for prism search criterias. if does not find one, turns back to initial position
    QString command = "%R1Q,9037:0.08,0.08,0\r\n";

    //execute fine adjust command
    bool result = this->checkCommandRC(command);
    if(result){
        myEmitter.emitSendString("fine adjust successful.");
        //if successful then run target tracking.

        //set bool, that fine adjust was callled
        this->fineAdjusted = true;

        return true;
    }else{
        return false;
    }
}

/*!
 * \brief move to specified angles
 * \param azimuth
 * \param zenith
 * \param distance
 * \param isrelativ
 * \return
 */
bool LeicaTachymeter::move(double azimuth, double zenith, double distance,bool isrelativ){

    if(isrelativ){

    }else{

        if( this->serial->isOpen()){

            //stop prism lock
            this->deactiveLockState();

            if(azimuth <= 0.0){
                azimuth = 2*PI + azimuth;
            }

            //correct the values depending on specified sense of rotation
            if(this->myConfiguration.stringParameter.contains("sense of rotation")){
                QString sense =  this->myConfiguration.stringParameter.value("sense of rotation");
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
            }

        //start laser to find the position
        this->activateLaserPointer();
        }
    }
    return true;
}

/*!
 * \brief move to specified x y z position
 * \param x
 * \param y
 * \param z
 * \return
 */
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

        //stop prism lock
        this->deactiveLockState();

        if(r->rPolar.azimuth <= 0.0){
            r->rPolar.azimuth = 2*PI + r->rPolar.azimuth;
        }

        //correct the values depending on specified sense of rotation
        if(this->myConfiguration.stringParameter.contains("sense of rotation")){
            QString sense =  this->myConfiguration.stringParameter.value("sense of rotation");
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
        }

        //start laser to find the position
        this->activateLaserPointer();
    }
    return true;
}

/*!
 * \brief measure
 * \param m
 * \return
 */
QList<Reading*> LeicaTachymeter::measure(MeasurementConfig *m){

    //if tracking measurements are active, stop them
    this->stopTrackingMode();

    //stop watchwindow if it is open
    this->stopWatchWindowForMeasurement();

    //set target to specified value
    //measurements work with standard mode for precise measurements
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

    //restart watchwindow if it was open before measurement
    this->restartWatchWindowAfterMeasurement();

    QList<Reading*> emptyList;
    return emptyList;
}

/*!
 * \brief readingStream for watchwindow
 * \param streamFormat
 * \return
 */
QVariantMap LeicaTachymeter::readingStream(Configuration::ReadingTypes streamFormat)
{

    this->currentStreamFormat = streamFormat;

    this->watchWindowOpen = true;

    QVariantMap m;

    Reading *r = NULL;

    //set target to specified value
    //Stream works with trk mode, for fast measurements
    if(this->setTargetTypeStream()){
    //if(this->setTargetTypeMeasure()){

        r = this->getStreamValues();

        QThread::msleep(100);

        if(r == NULL){
            return m;
        }

        //return specified values
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

/*!
 * \brief getConnectionState
 * \return
 */
bool LeicaTachymeter::getConnectionState()
{
    return this->serial->isOpen();
}

/*!
 * \brief isReadyForMeasurement
 * \return
 */
bool LeicaTachymeter::isReadyForMeasurement()
{
    return this->getConnectionState();
}

/*!
 * \brief getSensorStats
 * \return
 */
QMap<QString, QString> LeicaTachymeter::getSensorStats()
{
    QMap<QString, QString> stats;

    stats.insert("connected", QString::number(this->serial->isOpen()));

    return stats;
}

/*!
 * \brief isBusy
 * \return
 */
bool LeicaTachymeter::isBusy()
{
    return false;
}

/*!
 * \brief measurePolar
 * \param m
 * \return
 */
QList<Reading*> LeicaTachymeter::measurePolar(MeasurementConfig *m){

    QList<Reading*> readings;

    int faceCount = 1;

    //check for two face measurement
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

                    if(executeCommand(command)){

                        QString measureData = this->receive();

                        QStringList polarElements = measureData.split(",");

                        r->rPolar.azimuth = polarElements.at(polarElements.size()-3).toDouble();
                        r->rPolar.zenith = polarElements.at(polarElements.size()-2).toDouble();
                        r->rPolar.distance = polarElements.at(polarElements.size()-1).toDouble();
                        r->typeofReading = Configuration::ePolar;
                        r->face = this->getCurrentFace(r->rPolar.zenith);

                        //correct the values depending on specified sense of rotation
                        if(this->myConfiguration.stringParameter.contains("sense of rotation")){
                            QString sense =  this->myConfiguration.stringParameter.value("sense of rotation");
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
    this->stopTrackingAfterMeasure();
    //restart watchwindow if it was open before measurement
    this->restartWatchWindowAfterMeasurement();
    return readings;
}

/*!
 * \brief measureDistance
 * \param m
 * \return
 */
QList<Reading*> LeicaTachymeter::measureDistance(MeasurementConfig *m){

    QList<Reading*> readings;

    int faceCount = 1;

    //check for two face measurement
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

                    if(executeCommand(command)){

                        QString measureData = this->receive();

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
    this->stopTrackingAfterMeasure();
    //restart watchwindow if it was open before measurement
    this->restartWatchWindowAfterMeasurement();
    return readings;
}

/*!
 * \brief measureDirection
 * \param m
 * \return
 */
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

                    QStringList polarElements = measureData.split(",");

                    r->rDirection.azimuth = polarElements.at(polarElements.size()-2).toDouble();
                    r->rDirection.zenith = polarElements.at(polarElements.size()-1).toDouble();
                    r->typeofReading = Configuration::eDirection;
                    r->face = this->getCurrentFace(r->rDirection.zenith);

                    //correct the values depending on specified sense of rotation
                    if(this->myConfiguration.stringParameter.contains("sense of rotation")){
                        QString sense =  this->myConfiguration.stringParameter.value("sense of rotation");
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
    this->stopTrackingAfterMeasure();
    //restart watchwindow if it was open before measurement
    this->restartWatchWindowAfterMeasurement();
    return readings;
}

/*!
 * \brief measureCartesian
 * \param m
 * \return
 */
QList<Reading*> LeicaTachymeter::measureCartesian(MeasurementConfig *m){

    QList<Reading*> readings = this->measurePolar(m);

    for(int i = 0; i<readings.size();i++){
        readings.at(i)->toCartesian();
        readings.at(i)->rCartesian.isValid = true;
        readings.at(i)->typeofReading = Configuration::eCartesian;
    }
    this->stopTrackingAfterMeasure();
    //restart watchwindow if it was open before measurement
    this->restartWatchWindowAfterMeasurement();
    return readings;

}

/*!
 * \brief receive return code of geom com function call
 * \return
 */
QString LeicaTachymeter::receive(){
    QByteArray responseData = this->serial->readAll();
    while (this->serial->waitForReadyRead(100))
        responseData += this->serial->readAll();
    QString response = QString(responseData);
    return response;
}

/*!
 * \brief getError
 * \param e
 */
void LeicaTachymeter::getError(QSerialPort::SerialPortError e){
    qDebug() << e;
    this->serial->close();
}

/*!
 * \brief executeCommand executes the geom com command
 * \param command
 * \return
 */
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

        if(QString::number(elements.at(0).toInt()).compare("0") == 0){
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

    QString measureMode = "";

    if(this->myConfiguration.stringParameter.contains("reflector")){
        refl = this->myConfiguration.stringParameter.value("reflector");
    }

    if(this->myConfiguration.stringParameter.contains("measure mode")){
        measureMode = this->myConfiguration.stringParameter.value("measure mode");
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

            if(measureMode.compare("precise") == 0){

                //if(receive.compare("%R1P,0,0:0,0\r\n") != 0){ // if not IR and standard
                if(receive.compare("%R1P,0,0:0,11\r\n") != 0){ // if not IR and precise

                    //switch
                    //command = "%R1Q,17019:0\r\n";  //IR standard
                    command = "%R1Q,17019:11\r\n"; //IR precise
                    if(this->executeCommand(command)){
                        receive = this->receive();
                        this->fineAdjusted = false;
                    }
                }
            }else if(measureMode.compare("fast") == 0){

                //1 IR fast
                if(receive.compare("&R1P,0,0:0,1\r\n") != 0){

                    //switch
                    command = "%R1Q,17019:1\r\n"; //IR fast
                    if(this->executeCommand(command)){
                        receive = this->receive();
                        this->fineAdjust();
                    }
                }
            }

            if(this->getLOCKState()){
                return true;
            }
        }
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

    if(this->myConfiguration.stringParameter.contains("reflector")){
        refl = this->myConfiguration.stringParameter.value("reflector");
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
            //if(receive.compare("%R1P,0,0:0,4\r\n") != 0){ // if not IR and tracking
            if(receive.compare("%R1P,0,0:0,10\r\n") != 0){ // if not IR and synchrotrack
                //switch
                //command = "%R1Q,17019:4\r\n";
                command = "%R1Q,17019:10\r\n";
                if(this->executeCommand(command)){
                    receive = this->receive();
                    this->fineAdjusted = false;
                }
            }
            if(this->getLOCKState()){
                return true;
            }
        }
    }
    return false;
}

/*!
 * \brief setAdjustMode sets the adjust mode to point or angle accuracy
 * \return
 */
bool LeicaTachymeter::setAdjustMode()
{
    if(this->myConfiguration.stringParameter.contains("ATR accuracy")){
        QString atrAcc = this->myConfiguration.stringParameter.value("ATR accuracy");

        QString command = "%R1Q,9030:\r\n";
        if(this->executeCommand(command)){
            QString receive = this->receive();


            if(atrAcc.compare("point tolerance") == 0){//if point tolerance is selected in options

                if(receive.compare("%R1P,0,0:0,0\r\n") == 0){ //if angle tolerance is active

                    command = "%R1Q,9031:1\r\n";
                    return this->executeCommand(command);
                }
                return true;

            }else{ // if angle tolerance is selected in options

                if(receive.compare("%R1P,0,0:0,1\r\n") == 0){ //if point tolerance is active

                    command = "%R1Q,9031:0\r\n";
                    return this->executeCommand(command);
                }
                return true;

            }
        }
    }
    return false;
}

/*!
 * \brief stopTrackingAfterMeasure stops tracking mode after measurement with ATR
 */
void LeicaTachymeter::stopTrackingAfterMeasure()
{
    if(this->myConfiguration.stringParameter.contains("stop tracking after measurement") &&
            this->myConfiguration.stringParameter.contains("ATR")){

        QString atrValue = this->myConfiguration.stringParameter.value("ATR");
        QString trackValue = this->myConfiguration.stringParameter.value("stop tracking after measurement");

        if(atrValue.compare("atr on") == 0 && trackValue.compare("yes") == 0){
            this->deactiveLockState();
        }
    }

}

/*!
 * \brief stopWatchWindowForMeasurement
 */
void LeicaTachymeter::stopWatchWindowForMeasurement()
{
    if(watchWindowOpen){
        this->stopTrackingMode();
        this->measureWatchWindow = true;
    }
}

/*!
 * \brief restartWatchWindowAfterMeasurement
 */
void LeicaTachymeter::restartWatchWindowAfterMeasurement()
{
    if(measureWatchWindow){
        this->measureWatchWindow = false;
        this->readingStream(this->currentStreamFormat);
    }
}

/*!
 * \brief getQuickMeasReading convert receive code to reading
 * \param receive
 * \return
 */
Reading *LeicaTachymeter::getQuickMeasReading(QString receive)
{
    QStringList rc = receive.split(":"); //split at ":" to get RC
    QStringList elements = rc.at(1).split(","); // split at "," to get values

    Reading *r;
    r = new Reading();

    //if return code is not ok, then exit function
    if(elements.at(0).compare("0") != 0){
        return r;
    }

    //get single values from string and set to reading

    r->rPolar.azimuth = elements.at(1).toDouble();
    r->rPolar.zenith = elements.at(2).toDouble();
    r->rPolar.distance = elements.at(3).toDouble();
    r->instrument = this;
    r->typeofReading = Configuration::ePolar;
    r->rPolar.isValid = true;

    //correct the values depending on specified sense of rotation
    if(this->myConfiguration.stringParameter.contains("sense of rotation")){
        QString sense =  this->myConfiguration.stringParameter.value("sense of rotation");
        if(sense.compare("mathematical") == 0){
            r->rPolar.azimuth = 2 * PI - r->rPolar.azimuth;
        }
    }

    return r;
}

/*!
 * \brief activateLaserPointer
 */
void LeicaTachymeter::activateLaserPointer()
{
    QString command = "%R1Q,1004:1\r\n";
    laserOn = true;
    this->executeCommand(command);
}

/*!
 * \brief deactivateLaserPointer
 */
void LeicaTachymeter::deactivateLaserPointer()
{
    QString command = "%R1Q,1004:0\r\n";
    laserOn = false;
    this->executeCommand(command);
}

/*!
 * \brief stopTrackingMode
 */
void LeicaTachymeter::stopTrackingMode()
{
    //stop tracking mode for fast measurements (watchwindow)
    QString command = "%R1Q,17017:6\r\n";
    if(executeCommand(command)){
        QString measuredData = this->receive();
        myEmitter.emitSendString("stop tracking");
    }
}

/*!
 * \brief getStreamValues is the measure method for watch window. should be faster than the normal method
 * \return
 */
Reading *LeicaTachymeter::getStreamValues()
{
    Reading *r = NULL;

    if(this->myConfiguration.stringParameter.contains("reflector")){
        QString value = this->myConfiguration.stringParameter.value("reflector");
        QString command;

        if(value.compare("reflector") == 0){
            //command = "%R1Q,2008:7,1\r\n";
            command = "%R1Q,2008:1,1\r\n";
        }else{
            //command = "%R1Q,2008:8,1\r\n";
            command = "%R1Q,2008:1,1\r\n";
        }

        if(this->executeCommand(command)){
            QString receive = this->receive();
            if(receive.compare("%R1P,0,0:0\r\n") == 0){

                command = "%R1Q,2108:5000,1\r\n";

                if(executeCommand(command)){

                    QString measureData = this->receive();

                    QStringList polarElements = measureData.split(",");

                    r = new Reading();
                    r->rPolar.azimuth = polarElements.at(polarElements.size()-3).toDouble();
                    r->rPolar.zenith = polarElements.at(polarElements.size()-2).toDouble();
                    r->rPolar.distance = polarElements.at(polarElements.size()-1).toDouble();
                    r->typeofReading = Configuration::ePolar;
                    r->face = this->getCurrentFace(r->rPolar.zenith);

                    if(this->myConfiguration.stringParameter.contains("sense of rotation")){
                        QString sense =  this->myConfiguration.stringParameter.value("sense of rotation");
                        if(sense.compare("mathematical") == 0){
                            r->rPolar.azimuth = 2 * PI - r->rPolar.azimuth;
                        }
                    }
                    r->rPolar.isValid = true;
                    r->instrument = this;
                    r->measuredAt = QDateTime::currentDateTime();
                    return r;
                }
            }
        }
    }
    return r;

}

/*!
 * \brief executeEDM for distance measurement
 * \return
 */
bool LeicaTachymeter::executeEDM(){

    myEmitter.emitSendString("start edm measurement");

    //QString edmCommand = "%R1Q,2008:1,1\r\n";  //maybe wrong. 1 = reflector tape? try with the other values

    if(this->myConfiguration.stringParameter.contains("reflector")){
        QString value = this->myConfiguration.stringParameter.value("reflector");
        QString edmCommand;

        if(value.compare("reflector") == 0){
            //edmCommand = "%R1Q,2008:2,1\r\n";
            edmCommand = "%R1Q,2008:1,1\r\n";
        }else{
            //edmCommand = "%R1Q,2008:5,1\r\n";
            edmCommand = "%R1Q,2008:1,1\r\n";
        }

        if(this->executeCommand(edmCommand)){
            QString receive = this->receive();
            if(receive.compare("%R1P,0,0:0\r\n") == 0){
                return true;
            }
        }
    }
    return false;
}
