#include "p_leicatachymeter.h"

/*!
 * \brief LeicaTachymeter::init
 */
void LeicaTachymeter::init(){

    //set plugin meta data
    this->metaData.name = "LeicaTachymeter";
    this->metaData.pluginName = "OpenIndy Default Plugin";
    this->metaData.author = "oi";
    this->metaData.description = "Control Leica Tachymeter";
    this->metaData.iid = "de.openIndy.plugin.sensor.totalStation.v001";

    //set supported reading types
    this->supportedReadingTypes.append(ePolarReading);
    this->supportedReadingTypes.append(eDirectionReading);
    this->supportedReadingTypes.append(eDistanceReading);
    this->supportedReadingTypes.append(eCartesianReading);

    //set supported sensor actions
    this->supportedSensorActions.append(eMoveAngle);
    this->supportedSensorActions.append(eToggleSight);

    //set supported connection types
    this->supportedConnectionTypes.append(eSerialConnection);

    //set string parameter
    this->stringParameters.insert("reflector", "reflectorless");
    this->stringParameters.insert("reflector", "reflector");
    this->stringParameters.insert("ATR", "atr off");
    this->stringParameters.insert("ATR", "atr on");
    this->stringParameters.insert("stop tracking after measurement", "yes");
    this->stringParameters.insert("stop tracking after measurement", "no");
    this->stringParameters.insert("ATR accuracy", "angle tolerance");
    this->stringParameters.insert("ATR accuracy", "point tolerance");
    this->stringParameters.insert("sense of rotation", "geodetic");
    this->stringParameters.insert("sense of rotation", "mathematical");
    this->stringParameters.insert("measure mode", "fast");
    this->stringParameters.insert("measure mode", "precise");
    this->stringParameters.insert("laser beam after aim", "no");
    this->stringParameters.insert("laser beam after aim", "yes");

    //set self defined actions
    this->selfDefinedActions.append("lock to prism"); //start tracking
    this->selfDefinedActions.append("stop prism lock"); //stop tracking
    this->selfDefinedActions.append("stop measurement"); // call this to stop tracking mode
    this->selfDefinedActions.append("laserPointer"); //turn on/off laser pointer

    //set default accuracy
    this->defaultAccuracy.insert("sigmaAzimuth",0.000001570);
    this->defaultAccuracy.insert("sigmaZenith",0.000001570);
    this->defaultAccuracy.insert("sigmaDistance",0.001);

    //general tachy inits
    serial = new QSerialPort();
    laserOn = false;
    fineAdjusted = false;
    this->watchWindowOpen = false;
    this->measureWatchWindow = false;

}

/*!
 * \brief LeicaTachymeter::doSelfDefinedAction
 * \param action
 * \return
 */
bool LeicaTachymeter::doSelfDefinedAction(const QString &action)
{

    qDebug() << "you pressed: " << action;

    if(action.compare("l") == 0){

        qDebug() << "leica tachy l pressed";

        //TODO laser ausschalten bzw. einschalten
        //first turn laser off
        if(laserOn){
            this->deactivateLaserPointer();
        }
        //activate lock mode for prism tracking
        this->getLOCKState();

    }

    if(action == "lock to prism"){

        //first turn laser off
        if(laserOn){
            this->deactivateLaserPointer();
        }
        //activate lock mode for prism tracking
        this->getLOCKState();

    }else if(action == "stop prism lock"){

        //stop lock mode for prism tracking
        this->deactiveLockState();

    }else if(action == "stop measurement"){

        this->stopTrackingMode();
        this->watchWindowOpen = false;

    }else if(action == "laserPointer"){

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
 * \brief LeicaTachymeter::abortAction
 * \return
 */
bool LeicaTachymeter::abortAction(){
    return false;
    //abort action here;
}

/*!
 * \brief LeicaTachymeter::connectSensor
 * \return
 */
bool LeicaTachymeter::connectSensor(){

    //set fineAdjusted to false
    this->fineAdjusted = false;

    //set port
    this->serial->setPortName(this->sensorConfiguration.getConnectionConfig().comPort);

    if( this->serial->open(QIODevice::ReadWrite) ){

        if( this->serial->setBaudRate(this->sensorConfiguration.getConnectionConfig().baudRate)
                && this->serial->setDataBits(this->sensorConfiguration.getConnectionConfig().dataBits)
                && this->serial->setParity(this->sensorConfiguration.getConnectionConfig().parity)
                && this->serial->setFlowControl(this->sensorConfiguration.getConnectionConfig().flowControl)
                && this->serial->setStopBits(this->sensorConfiguration.getConnectionConfig().stopBits) ){

            emit this->sendMessage("connected");
            return true;
        }
    }

    return false;

}

/*!
 * \brief LeicaTachymeter::disconnectSensor
 * \return
 */
bool LeicaTachymeter::disconnectSensor(){

    if(this->serial->isOpen()){
       this->serial->close();
        emit this->sendMessage("connection closed");
    }

    return true;
}

/*!
 * \brief LeicaTachymeter::toggleSightOrientation
 * \return
 */
bool LeicaTachymeter::toggleSightOrientation(){

    if( this->serial->isOpen()){

        QString command = "%R1Q,9028:0,0,0\r\n";
        //combine the string command


            if(executeCommand(command)){
                emit this->sendMessage("toggle sight");
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
        if(!this->sensorConfiguration.getStringParameter().contains("ATR")){ //only if atr is on
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
        emit this->sendMessage("not connected");
        return false;
    }
}

/*!
 * \brief deactiveLockState deactivates lock and atr mode of the total station.
 */
void LeicaTachymeter::deactiveLockState()
{
    QString atrOn = this->sensorConfiguration.getStringParameter().value("ATR");

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

    QString value = this->sensorConfiguration.getStringParameter().value("ATR");

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
            emit this->sendMessage("atr and lock state changed");
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
            emit this->sendMessage("target tracking active.");
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
        emit this->sendMessage("fine adjust successful.");
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
bool LeicaTachymeter::move(const double &azimuth, const double &zenith, const double &distance, const bool &isRelative){

    /*if(isRelative){

    }else{

        if( this->serial->isOpen()){

            //check if prism lock should be aborted before move/aim
            if(this->sensorConfiguration.getStringParameter().contains("laser beam after aim")){
                QString laserAim = this->sensorConfiguration.getStringParameter().value("laser beam after aim");
                if(laserAim.compare("yes") == 0){
                    //stop prism lock
                    this->deactiveLockState();
                }else{
                    //do not stop tracking and prism lock
                }
            }

            if(x <= 0.0){
                x = 2*PI + x;
            }

            //correct the values depending on specified sense of rotation
            if(this->sensorConfiguration.stringParameter.contains("sense of rotation")){
                QString sense =  this->sensorConfiguration.getStringParameter().value("sense of rotation");
                if(sense.compare("mathematical") == 0){
                    x = 2*PI - x;
                }
            }

            QString command="%R1Q,9027:";
            command.append(QString::number(x));
            command.append(",");
            command.append(QString::number(y));
            command.append(",0,0,0\r\n");

            if(executeCommand(command)){
                emit this->sendMessage("moving...");
                QString measureData = this->receive();
            }

            if(this->myConfiguration.stringParameter.contains("laser beam after aim")){
                QString laserAim = this->myConfiguration.stringParameter.value("laser beam after aim");
                if(laserAim.compare("yes") == 0){
                    //start laser to find the position
                    this->activateLaserPointer();
                }else{
                    //do not start laser
                }
            }
        }
    }*/
    return true;
}

/*!
 * \brief move to specified x y z position
 * \param x
 * \param y
 * \param z
 * \return
 */
bool LeicaTachymeter::move(const double &x, const double &y, const double &z){

    //convert xyz to azimuth, zenith and distance for total station
    OiVec p = Reading::toPolar(x, y, z);
    ReadingPolar rPolar;
    rPolar.azimuth = p.getAt(0);
    if(rPolar.azimuth <= 0.0){
        rPolar.azimuth = 2*PI + rPolar.azimuth;
    }
    rPolar.zenith = p.getAt(1);
    rPolar.distance = p.getAt(2);

    if(this->serial->isOpen()){

        //check if prism lock should be aborted before move/aim
        if(this->sensorConfiguration.getStringParameter().contains("laser beam after aim")){
            QString laserAim = this->sensorConfiguration.getStringParameter().value("laser beam after aim");
            if(laserAim.compare("yes") == 0){
                //stop prism lock
                this->deactiveLockState();
            }else{
                //do not stop tracking and prism lock
            }
        }

        //correct the values depending on specified sense of rotation
        if(this->sensorConfiguration.getStringParameter().contains("sense of rotation")){
            QString sense =  this->sensorConfiguration.getStringParameter().value("sense of rotation");
            if(sense.compare("mathematical") == 0){
                rPolar.azimuth = 2*PI - rPolar.azimuth;
            }
        }

        QString command = "%R1Q,9027:";
        command.append(QString::number(rPolar.azimuth));
        command.append(",");
        command.append(QString::number(rPolar.zenith));
        command.append(",0,0,0\r\n");

        if(executeCommand(command)){
            emit this->sendMessage("moving...");
            QString measureData = this->receive();
        }

        if(this->sensorConfiguration.getStringParameter().contains("laser beam after aim")){
            QString laserAim = this->sensorConfiguration.getStringParameter().value("laser beam after aim");
            if(laserAim.compare("yes") == 0){
                //start laser to find the position
                this->activateLaserPointer();
            }else{
                //do not start laser
            }
        }
    }
    return true;
}

/*!
 * \brief measure
 * \param m
 * \return
 */
QList<QPointer<Reading> > LeicaTachymeter::measure(const MeasurementConfig &mConfig){

    QList<QPointer<Reading> > emptyList;

    //if tracking measurements are active, stop them
    this->stopTrackingMode();

    //stop watchwindow if it is open
    this->stopWatchWindowForMeasurement();

    //set target to specified value
    //measurements work with standard mode for precise measurements
    if(this->setTargetTypeMeasure()){

        switch (mConfig.getTypeOfReading()){
        case ePolarReading:
            emptyList = this->measurePolar(mConfig);
            break;
        case eCartesianReading:
            emptyList = this->measureCartesian(mConfig);
            break;
        case eDirectionReading:
            emptyList = this->measureDirection(mConfig);
            break;
        case eDistanceReading:
            emptyList = this->measureDistance(mConfig);
            break;
        default:
            break;
        }
    }

    //restart watchwindow if it was open before measurement
    this->restartWatchWindowAfterMeasurement();

    if(emptyList.size() > 0){
        this->lastReading.first = emptyList.last()->getTypeOfReading();
        QPointer<Reading> r = new Reading(*emptyList.last());
        this->lastReading.second = r;
    }

    return emptyList;
}

/*!
 * \brief readingStream for watchwindow
 * \param streamFormat
 * \return
 */
QVariantMap LeicaTachymeter::readingStream(const ReadingTypes &streamFormat)
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
        case ePolarReading:
            m.insert("azimuth", r->getPolarReading().azimuth);
            m.insert("zenith", r->getPolarReading().zenith);
            m.insert("distance", r->getPolarReading().distance);
            break;
        case eCartesianReading:
            m.insert("x", r->getCartesianReading().xyz.getAt(0));
            m.insert("y", r->getCartesianReading().xyz.getAt(1));
            m.insert("z", r->getCartesianReading().xyz.getAt(2));
            break;
        case eDirectionReading:
            m.insert("azimuth", r->getPolarReading().azimuth);
            m.insert("zenith", r->getPolarReading().zenith);
            break;
        case eDistanceReading:
            m.insert("distance", r->getPolarReading().distance);
            break;
        default:
            break;
        }
    }

    this->lastReading.first = r->getTypeOfReading();
    Reading *myReading = new Reading(*r);
    this->lastReading.second = myReading;

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
bool LeicaTachymeter::getIsReadyForMeasurement()
{
    return this->getConnectionState();
}

/*!
 * \brief getSensorStats
 * \return
 */
QMap<QString, QString> LeicaTachymeter::getSensorStatus()
{
    QMap<QString, QString> stats;

    stats.insert("connected", QString::number(this->serial->isOpen()));

    return stats;
}

/*!
 * \brief isBusy
 * \return
 */
bool LeicaTachymeter::getIsBusy()
{
    return false;
}

/*!
 * \brief measurePolar
 * \param m
 * \return
 */
QList<QPointer<Reading> > LeicaTachymeter::measurePolar(const MeasurementConfig &mConfig){

    QList<QPointer<Reading> > readings;

    int faceCount = 1;

    //check for two face measurement
    if (mConfig.getMeasureTwoSides()){
       faceCount = 2;
    }

    if( this->serial->isOpen()){

        for(int i=0;i<mConfig.getIterations();i++){
            for(int k = 0; k<faceCount;k++){
                if (this->executeEDM()){

                    Reading *r;
                    QString command = "%R1Q,2108:5000,1\r\n";

                    if(executeCommand(command)){

                        QString measureData = this->receive();

                        QStringList polarElements = measureData.split(",");

                        ReadingPolar rPolar;

                        rPolar.azimuth = polarElements.at(polarElements.size()-3).toDouble();
                        rPolar.zenith = polarElements.at(polarElements.size()-2).toDouble();
                        rPolar.distance = polarElements.at(polarElements.size()-1).toDouble();

                        //correct the values depending on specified sense of rotation
                        if(this->sensorConfiguration.getStringParameter().contains("sense of rotation")){
                            QString sense =  this->sensorConfiguration.getStringParameter().value("sense of rotation");
                            if(sense.compare("mathematical") == 0){
                                rPolar.azimuth = 2 * PI - rPolar.azimuth;
                            }
                        }
                        rPolar.isValid = true;

                        r = new Reading(rPolar);
                        r->setSensorFace(this->getCurrentFace(rPolar.zenith));
                        r->setSensor(this);
                        r->setMeasuredAt(QDateTime::currentDateTime());
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
QList<QPointer<Reading> > LeicaTachymeter::measureDistance(const MeasurementConfig &mConfig){

    QList<QPointer<Reading> > readings;

    int faceCount = 1;

    //check for two face measurement
    if (mConfig.getMeasureTwoSides()){
       faceCount = 2;
    }

    if( this->serial->isOpen()){

        for(int i=0;i<mConfig.getIterations();i++){
            for(int k = 0; k<faceCount;k++){
                if (this->executeEDM()){

                    QString command = "%R1Q,2108:5000,1\r\n";

                    if(executeCommand(command)){

                        QString measureData = this->receive();

                        QStringList polarElements = measureData.split(",");

                        double zenith = polarElements.at(polarElements.size()-2).toDouble();

                        ReadingDistance rDistance;
                        rDistance.distance = polarElements.at(polarElements.size()-1).toDouble();
                        rDistance.isValid = true;

                        QPointer<Reading> reading = new Reading(rDistance);
                        reading->setSensorFace(this->getCurrentFace(zenith));

                        reading->setSensor(this);
                        reading->setMeasuredAt(QDateTime::currentDateTime());
                        readings.append(reading);

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
QList<QPointer<Reading> > LeicaTachymeter::measureDirection(const MeasurementConfig &mConfig){

    QList<QPointer<Reading> > readings;

    int faceCount = 1;

    if (mConfig.getMeasureTwoSides()){
       faceCount = 2;
    }


    if( this->serial->isOpen()){

        for(int i=0;i<mConfig.getIterations();i++){
            for(int k = 0; k<faceCount;k++){

                QString command = "%R1Q,2107:1\r\n";

                if(executeCommand(command)){

                    QString measureData = this->receive();

                    QStringList polarElements = measureData.split(",");

                    ReadingDirection rDirection;

                    rDirection.azimuth = polarElements.at(polarElements.size()-2).toDouble();
                    rDirection.zenith = polarElements.at(polarElements.size()-1).toDouble();

                    //correct the values depending on specified sense of rotation
                    if(this->sensorConfiguration.getStringParameter().contains("sense of rotation")){
                        QString sense =  this->sensorConfiguration.getStringParameter().value("sense of rotation");
                        if(sense.compare("mathematical") == 0){
                            rDirection.azimuth = 2 * PI - rDirection.azimuth;
                        }
                    }
                    rDirection.isValid = true;

                    QPointer<Reading> r = new Reading(rDirection);
                    r->setSensorFace(this->getCurrentFace(rDirection.zenith));
                    r->setSensor(this);
                    r->setMeasuredAt(QDateTime::currentDateTime());
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
QList<QPointer<Reading> > LeicaTachymeter::measureCartesian(const MeasurementConfig &mConfig){

    QList<QPointer<Reading> > readings = this->measurePolar(mConfig);

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
SensorFaces LeicaTachymeter::getCurrentFace(double zenith)
{
    if((zenith >= 0.0) && (zenith < PI)){
        return eFrontSide;
    }else{
        return eBackSide;
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

    if(this->sensorConfiguration.getStringParameter().contains("reflector")){
        refl = this->sensorConfiguration.getStringParameter().value("reflector");
    }

    if(this->sensorConfiguration.getStringParameter().contains("measure mode")){
        measureMode = this->sensorConfiguration.getStringParameter().value("measure mode");
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

    if(this->sensorConfiguration.getStringParameter().contains("reflector")){
        refl = this->sensorConfiguration.getStringParameter().value("reflector");
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
    if(this->sensorConfiguration.getStringParameter().contains("ATR accuracy")){
        QString atrAcc = this->sensorConfiguration.getStringParameter().value("ATR accuracy");

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
    if(this->sensorConfiguration.getStringParameter().contains("stop tracking after measurement") &&
            this->sensorConfiguration.getStringParameter().contains("ATR")){

        QString atrValue = this->sensorConfiguration.getStringParameter().value("ATR");
        QString trackValue = this->sensorConfiguration.getStringParameter().value("stop tracking after measurement");

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
QPointer<Reading> LeicaTachymeter::getQuickMeasReading(QString receive)
{
    QStringList rc = receive.split(":"); //split at ":" to get RC
    QStringList elements = rc.at(1).split(","); // split at "," to get values

    QPointer<Reading> r;

    //if return code is not ok, then exit function
    if(elements.at(0).compare("0") != 0){
        return r;
    }

    //get single values from string and set to reading

    ReadingPolar rPolar;

    rPolar.azimuth = elements.at(1).toDouble();
    rPolar.zenith = elements.at(2).toDouble();
    rPolar.distance = elements.at(3).toDouble();
    rPolar.isValid = true;

    //correct the values depending on specified sense of rotation
    if(this->sensorConfiguration.getStringParameter().contains("sense of rotation")){
        QString sense =  this->sensorConfiguration.getStringParameter().value("sense of rotation");
        if(sense.compare("mathematical") == 0){
            rPolar.azimuth = 2 * PI - rPolar.azimuth;
        }
    }

    r = new Reading(rPolar);
    r->setSensor(this);

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
        emit this->sendMessage("stop tracking");
    }
}

/*!
 * \brief getStreamValues is the measure method for watch window. should be faster than the normal method
 * \return
 */
QPointer<Reading> LeicaTachymeter::getStreamValues()
{
    Reading *r = NULL;

    if(this->sensorConfiguration.getStringParameter().contains("reflector")){
        QString value = this->sensorConfiguration.getStringParameter().value("reflector");
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

                    ReadingPolar rPolar;

                    rPolar.azimuth = polarElements.at(polarElements.size()-3).toDouble();
                    rPolar.zenith = polarElements.at(polarElements.size()-2).toDouble();
                    rPolar.distance = polarElements.at(polarElements.size()-1).toDouble();

                    if(this->sensorConfiguration.getStringParameter().contains("sense of rotation")){
                        QString sense =  this->sensorConfiguration.getStringParameter().value("sense of rotation");
                        if(sense.compare("mathematical") == 0){
                            rPolar.azimuth = 2 * PI - rPolar.azimuth;
                        }
                    }
                    rPolar.isValid = true;

                    r = new Reading(rPolar);
                    r->setSensorFace(this->getCurrentFace(rPolar.zenith));

                    r->setSensor(this);
                    r->setMeasuredAt(QDateTime::currentDateTime());

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

    emit this->sendMessage("start edm measurement");

    //QString edmCommand = "%R1Q,2008:1,1\r\n";  //maybe wrong. 1 = reflector tape? try with the other values

    if(this->sensorConfiguration.getStringParameter().contains("reflector")){
        QString value = this->sensorConfiguration.getStringParameter().value("reflector");
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
