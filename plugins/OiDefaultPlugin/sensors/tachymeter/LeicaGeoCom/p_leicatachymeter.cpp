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

    QMap <QString, double>* doubleParameter = new QMap<QString, double>;

    doubleParameter->insert("addition constant",0.0);

    return doubleParameter;

}

QMap <QString, QStringList>* LeicaTachymeter::getStringParameter() const{

    QMap <QString, QStringList>* stringParameter = new QMap<QString, QStringList>;

    QStringList ATR;
    ATR.append("atr on");
    ATR.append("atr off");

    QStringList senseOfRotation;
    senseOfRotation.append("mathematical");
    senseOfRotation.append("geodetic");

    stringParameter->insert("ATR",ATR);
    stringParameter->insert("sense of rotation", senseOfRotation);

    return stringParameter;

}

QStringList LeicaTachymeter::selfDefinedActions() const
{
    QStringList a;
    return a;
}

bool LeicaTachymeter::doSelfDefinedAction(QString a)
{

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

bool LeicaTachymeter::move(double azimuth, double zenith, double distance,bool isrelativ){

    if(isrelativ){

    }else{

        if( this->serial->isOpen()){


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

QList<Reading*> LeicaTachymeter::measure(MeasurementConfig *m){


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


    QList<Reading*> emptyList;
    return emptyList;
}

QVariantMap LeicaTachymeter::readingStream(Configuration::ReadingTypes streamFormat)
{
    QVariantMap m;

    Reading r;
    QString command;
    if(this->serial->isOpen()){

        switch (streamFormat) {
        case Configuration::ePolar:
            /*if(this->executeEDM()){
                command = "%R1Q,2108:5000,1\r\n";
                if(executeCommand(command)){
                    QString measuredData = this->receive();
                    QStringList polarElements = measuredData.split(",");

                    r.rPolar.azimuth = polarElements.at(polarElements.size()-3).toDouble();
                    r.rPolar.zenith = polarElements.at(polarElements.size()-2).toDouble();
                    r.rPolar.distance = polarElements.at(polarElements.size()-1).toDouble();
                    r.typeofReading = Configuration::ePolar;
                    r.rPolar.isValid = true;

                    if(this->myConfiguration->stringParameter.contains("sense of rotation")){
                        QString sense =  this->myConfiguration->stringParameter.value("sense of rotation");
                        if(sense.compare("mathematical") == 0){
                            r.rPolar.azimuth = 2 * PI - r.rPolar.azimuth;
                        }
                    }

                    m.insert("azimuth",r.rPolar.azimuth);
                    m.insert("zenith",r.rPolar.zenith);
                    m.insert("distance",r.rPolar.distance);

                }
            }*/

            //execute quick measurement for tracking distance
            command ="%R1Q,2117:\r\n";
            if(executeCommand(command)){
                QString measuredData = this->receive();
                QStringList polarElements = measuredData.split(",");

                r.rPolar.azimuth = polarElements.at(polarElements.size()-3).toDouble();
                r.rPolar.zenith = polarElements.at(polarElements.size()-2).toDouble();
                r.rPolar.distance = polarElements.at(polarElements.size()-1).toDouble();
                r.typeofReading = Configuration::ePolar;
                r.rPolar.isValid = true;

                if(this->myConfiguration->stringParameter.contains("sense of rotation")){
                    QString sense =  this->myConfiguration->stringParameter.value("sense of rotation");
                    if(sense.compare("mathematical") == 0){
                        r.rPolar.azimuth = 2 * PI - r.rPolar.azimuth;
                    }
                }

                m.insert("azimuth",r.rPolar.azimuth);
                m.insert("zenith",r.rPolar.zenith);
                m.insert("distance",r.rPolar.distance);
            }

            break;
        case Configuration::eCartesian:
            /*if(this->executeEDM()){
                command = "%R1Q,2108:5000,1\r\n";
                if(executeCommand(command)){
                    QString measuredData = this->receive();
                    QStringList polarElements = measuredData.split(",");

                    r.rPolar.azimuth = polarElements.at(polarElements.size()-3).toDouble();
                    r.rPolar.zenith = polarElements.at(polarElements.size()-2).toDouble();
                    r.rPolar.distance = polarElements.at(polarElements.size()-1).toDouble();
                    r.typeofReading = Configuration::eCartesian;
                    r.rPolar.isValid = true;

                    if(this->myConfiguration->stringParameter.contains("sense of rotation")){
                        QString sense =  this->myConfiguration->stringParameter.value("sense of rotation");
                        if(sense.compare("mathematical") == 0){
                            r.rPolar.azimuth = 2 * PI - r.rPolar.azimuth;
                        }
                    }
                    r.toCartesian();
                    m.insert("x",r.rCartesian.xyz.getAt(0));
                    m.insert("y",r.rCartesian.xyz.getAt(1));
                    m.insert("z",r.rCartesian.xyz.getAt(2));
                }
            }*/

            //execute quick measurement for tracking distance
            command = "%R1Q,2117:\r\n";
            if(executeCommand(command)){
                QString measuredData = this->receive();
                QStringList polarElements = measuredData.split(",");

                r.rPolar.azimuth = polarElements.at(polarElements.size()-3).toDouble();
                r.rPolar.zenith = polarElements.at(polarElements.size()-2).toDouble();
                r.rPolar.distance = polarElements.at(polarElements.size()-1).toDouble();
                r.typeofReading = Configuration::eCartesian;
                r.rPolar.isValid = true;

                if(this->myConfiguration->stringParameter.contains("sense of rotation")){
                    QString sense = this->myConfiguration->stringParameter.value("sense of rotation");
                    if(sense.compare("mathematical") == 0){
                        r.rPolar.azimuth = 2* PI - r.rPolar.azimuth;
                    }
                }
                r.toCartesian();
                m.insert("x",r.rCartesian.xyz.getAt(0));
                m.insert("y",r.rCartesian.xyz.getAt(1));
                m.insert("z",r.rCartesian.xyz.getAt(2));
            }

            break;
        case Configuration::eDirection:
            command = "%R1Q,2107:1\r\n";
            if(executeCommand(command)){
                QString measuredData = this->receive();
                QStringList polarElements = measuredData.split(",");

                r.rDirection.azimuth = polarElements.at(polarElements.size()-2).toDouble();
                r.rDirection.zenith = polarElements.at(polarElements.size()-1).toDouble();
                r.rDirection.isValid = true;

                if(this->myConfiguration->stringParameter.contains("sense of rotation")){
                    QString sense =  this->myConfiguration->stringParameter.value("sense of rotation");
                    if(sense.compare("mathematical") == 0){
                        r.rDirection.azimuth = 2 * PI - r.rDirection.azimuth;
                    }
                }

                m.insert("azimuth",r.rDirection.azimuth);
                m.insert("zenith",r.rDirection.zenith);
            }
            break;
        case Configuration::eDistance:
            if(this->executeEDM()){
                command = "%R1Q,2108:5000,1\r\n";
                if(executeCommand(command)){
                    QString measuredData = this->receive();
                    QStringList polarElements = measuredData.split(",");
                    r.rDistance.distance = polarElements.at(polarElements.size()-1).toDouble();
                    r.rDistance.isValid = true;

                    m.insert("distance", r.rDistance.distance);
                }
            }
            break;
        default:
            break;
        }
    }

    return m;



    /*QVariantMap tmpMap;

    if(this->serial->isOpen()){

        QString command = "%R1Q,2107:1\r\n";

        if(executeCommand(command)){

             QString measureData = this->receive();

             QStringList polarElements = measureData.split(",");

             double azimuth = polarElements.at(polarElements.size()-2).toDouble();
             double zenith = polarElements.at(polarElements.size()-1).toDouble();

             if(this->myConfiguration->stringParameter.contains("sense of rotation")){
                 QString sense =  this->myConfiguration->stringParameter.value("sense of rotation");
                 if(sense.compare("mathematical") == 0){
                     azimuth = 2 * PI - azimuth;
                 }
             }

             tmpMap.insert("azimuth",azimuth);
             tmpMap.insert("zenith",zenith);

        }
        return tmpMap;
    }*/
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

                    Reading *r = new Reading();
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

                        if(this->myConfiguration->stringParameter.contains("sense of rotation")){
                            QString sense =  this->myConfiguration->stringParameter.value("sense of rotation");
                            if(sense.compare("mathematical") == 0){
                                r->rPolar.azimuth = 2 * PI - r->rPolar.azimuth;
                            }
                        }
                        r->rPolar.isValid = true;
                        readings.append(r);

                        if(faceCount == 2){
                            this->toggleSightOrientation();
                        }
                    }
                }
            }
        }
    }
    return readings;
}

QList<Reading*> LeicaTachymeter::measureDistance(MeasurementConfig *m){


    QList<Reading*> readings;

    if( this->serial->isOpen()){
        for(int i=0;i<m->iterations;i++){
            if (this->executeEDM()){

                Reading *r = new Reading();

                QString command = "%R1Q,2108:5000,1\r\n";

                myEmitter.emitSendString("edm succesful");

                if(executeCommand(command)){

                     QString measureData = this->receive();

                     myEmitter.emitSendString(measureData);

                     QStringList polarElements = measureData.split(",");

                     r->rDistance.distance = polarElements.at(polarElements.size()-1).toDouble();
                     r->typeofReading = Configuration::eDistance;
                     r->rDistance.isValid = true;

                     readings.append(r);

                }
            }
        }
    }
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

                Reading *r = new Reading();

                QString command = "%R1Q,2107:1\r\n";

                if(executeCommand(command)){

                    QString measureData = this->receive();

                    myEmitter.emitSendString(measureData);

                    QStringList polarElements = measureData.split(",");

                    r->rDirection.azimuth = polarElements.at(polarElements.size()-2).toDouble();
                    r->rDirection.zenith = polarElements.at(polarElements.size()-1).toDouble();
                    r->typeofReading = Configuration::eDirection;

                    if(this->myConfiguration->stringParameter.contains("sense of rotation")){
                        QString sense =  this->myConfiguration->stringParameter.value("sense of rotation");
                        if(sense.compare("mathematical") == 0){
                            r->rDirection.azimuth = 2 * PI - r->rDirection.azimuth;
                        }
                    }
                    r->rDirection.isValid = true;
                    readings.append(r);

                    if(faceCount == 2){
                        this->toggleSightOrientation();
                    }
                }
            }
        }
    }
    return readings;
}

QList<Reading*> LeicaTachymeter::measureCartesian(MeasurementConfig *m){


    QList<Reading*> readings = this->measurePolar(m);

    for(int i = 0; i<readings.size();i++){
        readings.at(i)->toCartesian();
        readings.at(i)->rCartesian.isValid = true;
        readings.at(i)->typeofReading = Configuration::eCartesian;
    }

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
