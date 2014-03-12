#include "p_leicatachymeter.h"

LeicaTachymeter::LeicaTachymeter(){

    serial = new QSerialPort();
}



PluginMetaData* LeicaTachymeter::getMetaData(){

    PluginMetaData* metaData = new PluginMetaData();

    metaData->name = "LeicaTachymeter";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "mlux";
    metaData->description = "Plugin zur Steuerung von Leica Tachymetern";
    metaData->iid = "de.openIndy.Plugin.Sensor.TotalStation.v001";
    //...

    return metaData;
}

QList<Configuration::ReadingTypes>* LeicaTachymeter::getSupportedReadingTypes(){

    QList<Configuration::ReadingTypes> *readingTypes = new QList<Configuration::ReadingTypes>;

    readingTypes->append(Configuration::ePolar);
    readingTypes->append(Configuration::eDirection);
    readingTypes->append(Configuration::eDistance);
    readingTypes->append(Configuration::eCartesian);

    return readingTypes;
}

QList<Configuration::ConnectionTypes>* LeicaTachymeter::getConnectionType(){

    QList<Configuration::ConnectionTypes> *connectionTypes = new QList<Configuration::ConnectionTypes>;
    connectionTypes->append(Configuration::eSerial);

    return connectionTypes;
}

QMap<QString,int>* LeicaTachymeter::getIntegerParameter(){
    return NULL;
}

QMap<QString,double>* LeicaTachymeter::getDoubleParameter(){

    QMap <QString, double>* doubleParameter = new QMap<QString, double>;

    doubleParameter->insert("addition constant",0.0);

    return doubleParameter;

}

QMap <QString, QStringList>* LeicaTachymeter::getStringParameter(){

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

bool LeicaTachymeter::checkMeasurementConfig(MeasurementConfig* mc){

    return true;

}

void LeicaTachymeter::dataStream(){

    this->dataStreamIsActive = true;

    QVariantMap *tmpMap = new QVariantMap();

    this->streamFormat = Configuration::eDirection;

    if( this->serial->isOpen()){
        while(this->dataStreamIsActive == true){


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

                 tmpMap->insert("azimuth",azimuth);
                 tmpMap->insert("zenith",zenith);

                }

        myEmitter.emitSendDataMap(tmpMap);
        }
    }

}

void LeicaTachymeter::sendCommandString(QString cmd){

    if(executeCommand(cmd)){

     QString response = this->receive();

     //Console::addLine(response);

    }

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

QList<Reading*> LeicaTachymeter::measurePolar(MeasurementConfig *m){

    QList<Reading*> readings;

    int faceCount = 1;

    if (m->measureTwoSides){
       faceCount = 2;
    }

    if( this->serial->isOpen()){

        for(int i=0;i<m->count;i++){
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
                    r->rPolar.zenith= polarElements.at(polarElements.size()-2).toDouble();
                    r->rPolar.distance = polarElements.at(polarElements.size()-1).toDouble();

                    if(this->myConfiguration->stringParameter.contains("sense of rotation")){
                        QString sense =  this->myConfiguration->stringParameter.value("sense of rotation");
                        if(sense.compare("mathematical") == 0){
                            r->rPolar.azimuth = 2 * PI - r->rPolar.azimuth;
                        }
                    }

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
     for(int i=0;i<m->count;i++){
        if (this->executeEDM()){

            Reading *r = new Reading();

            QString command = "%R1Q,2108:5000,1\r\n";

            myEmitter.emitSendString("edm succesful");

            if(executeCommand(command)){

             QString measureData = this->receive();

             myEmitter.emitSendString(measureData);

             QStringList polarElements = measureData.split(",");

             r->rDistance.distance = polarElements.at(polarElements.size()-1).toDouble();

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

        for(int i=0;i<m->count;i++){
            for(int k = 0; k<faceCount;k++){

                Reading *r = new Reading();

                QString command = "%R1Q,2107:1\r\n";

                if(executeCommand(command)){

                QString measureData = this->receive();

                myEmitter.emitSendString(measureData);

                QStringList polarElements = measureData.split(",");

                r->rDirection.azimuth = polarElements.at(polarElements.size()-2).toDouble();
                r->rDirection.zenith = polarElements.at(polarElements.size()-1).toDouble();

                if(this->myConfiguration->stringParameter.contains("sense of rotation")){
                    QString sense =  this->myConfiguration->stringParameter.value("sense of rotation");
                    if(sense.compare("mathematical") == 0){
                        r->rDirection.azimuth = 2 * PI - r->rDirection.azimuth;
                    }
                }

                readings.append(r);

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
    }


    return readings;

}



QString LeicaTachymeter::receive(){
    QByteArray responseData = this->serial->readAll();
    while (this->serial->waitForReadyRead(10))
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

       if(this->receive()=="%R1P,0,0:0\r\n"){
            return true;
            }
        }

    return false;
}
