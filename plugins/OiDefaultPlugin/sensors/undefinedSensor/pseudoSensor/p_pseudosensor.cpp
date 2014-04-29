#include "p_pseudosensor.h"

PseudoSensor::PseudoSensor()
{
}



PluginMetaData* PseudoSensor::getMetaData(){

    PluginMetaData* metaData = new PluginMetaData();

    metaData->name = "PseudoSensor";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "mlux";
    metaData->description = "Simuliert einen nicht definierten Sensor";
    metaData->iid = "de.openIndy.Plugin.Sensor.v001";
    //...

    return metaData;

}

QList<Configuration::ReadingTypes>* PseudoSensor::getSupportedReadingTypes(){

    QList<Configuration::ReadingTypes> *readingTypes = new QList<Configuration::ReadingTypes>;

    readingTypes->append(Configuration::eUndefined);

    return readingTypes;
}

QList<Configuration::ConnectionTypes>* PseudoSensor::getConnectionType(){

    QList<Configuration::ConnectionTypes> *connectionTypes = new QList<Configuration::ConnectionTypes>;
    connectionTypes->append(Configuration::eSerial);

    return connectionTypes;
}

QMap<QString,int>* PseudoSensor::getIntegerParameter(){
    return NULL;
}

QMap<QString,double>* PseudoSensor::getDoubleParameter(){

    return NULL;

}

QMap <QString, QStringList>* PseudoSensor::getStringParameter(){

    return NULL;

}

QMap<QString, double> *PseudoSensor::getDefaultAccuracy()
{
    QMap<QString, double>* undefinedSigma = new QMap<QString, double>;

    undefinedSigma->insert("undef sigma", 0.54);

    return undefinedSigma;
}

//! laser tracker measures a point and returns an observation
QList<Reading*> PseudoSensor::measure(MeasurementConfig *mc){

    QList<Reading*> readings;

    switch (mc->typeOfReading) {
    case Configuration::eUndefined:{

        Reading *r = new Reading();

        double randomValue = ((double) std::rand()/RAND_MAX)*(6.283185-0.0001)+1.0;

        r->rUndefined.values.insert("undefinedMeasurement1",randomValue);
        r->rUndefined.values.insert("undefinedMeasurement2",randomValue);

        readings.append(r);

        break;
    }
    default:
        break;
    }

    return readings;
}

void PseudoSensor::dataStream(){

    this->dataStreamIsActive = true;

    double value = 0.0;

    QVariantMap *m = new QVariantMap();

    while(this->dataStreamIsActive == true){

        value =((double) std::rand()/RAND_MAX)*(10.0-1.0)+1.0;

        m->insert("value",value);

        QThread::msleep(50);
        myEmitter.emitSendDataMap(m);
        QThread::msleep(50);
    }

}

void PseudoSensor::sendCommandString(QString){

}

//! checks if the measurementconfig is vaild
bool PseudoSensor::checkMeasurementConfig(MeasurementConfig*){

    return true;

}

//! connect app with laser tracker
bool PseudoSensor::connectSensor(ConnectionConfig* cConfig){

    if(cConfig != NULL){
        isConnected = true;
        QThread::msleep(1000);
        return true;
    }else{
        return false;
    }

}

//! disconnect app with laser tracker
bool PseudoSensor::disconnectSensor(){

    isConnected = false;
    QThread::msleep(1000);
    return true;

}
