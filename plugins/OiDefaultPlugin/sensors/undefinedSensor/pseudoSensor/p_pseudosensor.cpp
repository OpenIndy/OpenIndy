#include "p_pseudosensor.h"

PseudoSensor::PseudoSensor()
{
    this->sensorActionInProgress = false;
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

QList<Configuration::SensorFunctionalities> PseudoSensor::getSupportedSensorActions()
{
    QList<Configuration::SensorFunctionalities> a;

    return a;
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

QStringList PseudoSensor::selfDefinedActions()
{
    QStringList a;
    return a;
}

bool PseudoSensor::doSelfDefinedAction(QString a)
{
    return true;
}

QString PseudoSensor::getUndefinedReadingName()
{
    return "pseudo reading";
}

QMap<QString, double> *PseudoSensor::getDefaultAccuracy()
{
    QMap<QString, double>* undefinedSigma = new QMap<QString, double>;

    undefinedSigma->insert("pseudo sigma", 0.54);

    return undefinedSigma;
}

void PseudoSensor::abortAction()
{
    //abort
}

//! laser tracker measures a point and returns an observation
QList<Reading*> PseudoSensor::measure(MeasurementConfig *mc){

    this->sensorActionInProgress = true;
    QList<Reading*> readings;

    switch (mc->typeOfReading) {
    case Configuration::eUndefined:{

        Reading *r = new Reading();

        double randomValue = ((double) std::rand()/RAND_MAX)*(6.283185-0.0001)+1.0;

        r->rUndefined.values.insert("pseudoMeasurement_1",randomValue);
        r->rUndefined.values.insert("pseudoMeasurement_2",randomValue);

        readings.append(r);

        break;
    }
    default:
        break;
    }

    this->sensorActionInProgress = false;
    return readings;
}

QVariantMap PseudoSensor::readingStream(Configuration::ReadingTypes streamFormat)
{
    this->sensorActionInProgress = true;
    QVariantMap r;

    double randomValue = ((double) std::rand()/RAND_MAX)*(6.283185-0.0001)+1.0;

    r.insert("pseudoMeasurement_1",randomValue);
    r.insert("pseudoMeasurement_2",randomValue);
    QThread::msleep(100);
    this->sensorActionInProgress = false;
    return r;

}

bool PseudoSensor::getConnectionState()
{
    return isConnected;
}

bool PseudoSensor::isReadyForMeasurement()
{
    return true;
}

QMap<QString, QString> PseudoSensor::getSensorStats()
{
    QMap<QString, QString> a;
    a.insert("connected",QString::number(this->sensorActionInProgress));
    return a;
}

bool PseudoSensor::isBusy()
{
    return this->sensorActionInProgress;
}


//! connect app with laser tracker
bool PseudoSensor::connectSensor(ConnectionConfig* cConfig){

    this->sensorActionInProgress = true;
    if(cConfig != NULL){
        isConnected = true;
        QThread::msleep(1000);
        this->sensorActionInProgress = false;
        return true;
    }else{
        this->sensorActionInProgress = false;
        return false;
    }


}

//! disconnect app with laser tracker
bool PseudoSensor::disconnectSensor(){

    this->sensorActionInProgress = true;
    isConnected = false;
    QThread::msleep(1000);
    this->sensorActionInProgress = false;
    return true;

}
