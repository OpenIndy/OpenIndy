#include "p_pseudosensor.h"

PseudoSensor::PseudoSensor()
{
}



PluginMetaData* PseudoSensor::getMetaData() const{

    PluginMetaData* metaData = new PluginMetaData();

    metaData->name = "PseudoSensor";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "mlux";
    metaData->description = "Simuliert einen nicht definierten Sensor";
    metaData->iid = "de.openIndy.Plugin.Sensor.v001";
    //...

    return metaData;

}

QList<Configuration::ReadingTypes>* PseudoSensor::getSupportedReadingTypes() const{

    QList<Configuration::ReadingTypes> *readingTypes = new QList<Configuration::ReadingTypes>;

    readingTypes->append(Configuration::eUndefined);

    return readingTypes;
}

QList<Configuration::SensorFunctionalities> PseudoSensor::getSupportedSensorActions() const
{
    QList<Configuration::SensorFunctionalities> a;

    return a;
}

QList<Configuration::ConnectionTypes>* PseudoSensor::getConnectionType() const{

    QList<Configuration::ConnectionTypes> *connectionTypes = new QList<Configuration::ConnectionTypes>;
    connectionTypes->append(Configuration::eSerial);

    return connectionTypes;
}

QMap<QString,int>* PseudoSensor::getIntegerParameter() const{
    return NULL;
}

QMap<QString,double>* PseudoSensor::getDoubleParameter() const{

    return NULL;

}

QMap <QString, QStringList>* PseudoSensor::getStringParameter() const{

    return NULL;

}

QStringList PseudoSensor::selfDefinedActions() const
{
    QStringList a;
    return a;
}

bool PseudoSensor::doSelfDefinedAction(QString a)
{
    return true;
}

QString PseudoSensor::getUndefinedReadingName() const
{
    return "pseudo reading";
}

QMap<QString, double> *PseudoSensor::getDefaultAccuracy() const
{
    QMap<QString, double>* undefinedSigma = new QMap<QString, double>;

    undefinedSigma->insert("pseudoSigma", 0.54);

    return undefinedSigma;
}

void PseudoSensor::abortAction()
{
    //abort
}

//! laser tracker measures a point and returns an observation
QList<Reading*> PseudoSensor::measure(MeasurementConfig *mc){

    QList<Reading*> readings;

    switch (mc->typeOfReading) {
    case Configuration::eUndefined:{

        Reading *r = new Reading();

        double randomValue = ((double) rand()/RAND_MAX)*(6.283185-0.0001)+1.0;

        r->rUndefined.values.insert("pseudoMeasurement_1",randomValue);
        r->rUndefined.values.insert("pseudoMeasurement_2",randomValue);

        readings.append(r);

        break;
    }
    default:
        break;
    }

    if(readings.size() > 0){
        this->lastReading.first = readings.last()->typeofReading;
        Reading *r = new Reading();
        *r = *readings.last();
        this->lastReading.second = r;
    }

    return readings;
}

QVariantMap PseudoSensor::readingStream(Configuration::ReadingTypes streamFormat)
{
    QVariantMap r;

    double randomValue = ((double) rand()/RAND_MAX)*(6.283185-0.0001)+1.0;

    r.insert("pseudoMeasurement_1",randomValue);
    r.insert("pseudoMeasurement_2",randomValue);
    QThread::msleep(100);
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

    return a;
}

bool PseudoSensor::isBusy()
{
    return false;
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
