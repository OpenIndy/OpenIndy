#include "p_pseudotracker.h"



PseudoTracker::PseudoTracker(){
    myAzimuth = 0.00001;
    myZenith = 0.00001;
    myDistance =0.000001;
    myMotor = false;
    myInit = false;
    myCompIt = false;
    isConnected = false;
    side = 1;

}

PluginMetaData* PseudoTracker::getMetaData(){

    PluginMetaData* metaData = new PluginMetaData();

    metaData->name = "PseudoTracker";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "mlux";
    metaData->description = "Simuliert einen Laser Tracker";
    metaData->iid = "de.openIndy.Plugin.Sensor.LaserTracker.v001";
    //...

    return metaData;
}


QList<Configuration::ReadingTypes>* PseudoTracker::getSupportedReadingTypes(){

    QList<Configuration::ReadingTypes> *readingTypes = new QList<Configuration::ReadingTypes>;

    readingTypes->append(Configuration::ePolar);
    readingTypes->append(Configuration::eDirection);
    readingTypes->append(Configuration::eDistance);
    readingTypes->append(Configuration::eCartesian);
    readingTypes->append(Configuration::eTemperatur);
    readingTypes->append(Configuration::eLevel);

    return readingTypes;
}

QList<Configuration::SensorFunctionalities> PseudoTracker::getSupportedSensorActions()
{
    QList<Configuration::SensorFunctionalities> sensorActions;

    sensorActions.append(Configuration::eHome);
    sensorActions.append(Configuration::eInitialize);
    sensorActions.append(Configuration::eMoveAngle);
    sensorActions.append(Configuration::eMoveXYZ);
    sensorActions.append(Configuration::eToggleSight);
    sensorActions.append(Configuration::eCompensation);
    sensorActions.append(Configuration::eMotorState);

    return sensorActions;

}

QList<Configuration::ConnectionTypes>* PseudoTracker::getConnectionType(){
    QList<Configuration::ConnectionTypes> *connectionTypes = new QList<Configuration::ConnectionTypes>;
    connectionTypes->append(Configuration::eNetwork);
    connectionTypes->append(Configuration::eSerial);

    return connectionTypes;
}

QMap<QString,int>* PseudoTracker::getIntegerParameter(){
    return NULL;
}

QMap<QString,double>* PseudoTracker::getDoubleParameter(){

    QMap<QString,double>* trackerErrors = new QMap<QString,double>;

    trackerErrors->insert("lambda [mm]",0.000403);
    trackerErrors->insert("mu",0.000005);
    trackerErrors->insert("ex [mm]",0.0000122);
    trackerErrors->insert("by [mm]",0.0000654);
    trackerErrors->insert("bz [mm]",0.0000974);
    trackerErrors->insert("alpha [arc sec]",0.128);
    trackerErrors->insert("gamma [arc sec]",0.079);
    trackerErrors->insert("Aa1 [arc sec]",0.064);
    trackerErrors->insert("Ba1 [arc sec]",0.080);
    trackerErrors->insert("Aa2 [arc sec]",0.073);
    trackerErrors->insert("Ba2 [arc sec]",0.090);
    trackerErrors->insert("Ae0 [arc sec]",0.223);
    trackerErrors->insert("Ae1 [arc sec]",0.152);
    trackerErrors->insert("Be1 [arc sec]",0.183);
    trackerErrors->insert("Ae2 [arc sec]",0.214);
    trackerErrors->insert("Be2 [arc sec]",0.179);

    return trackerErrors;

}

QMap <QString, QStringList>* PseudoTracker::getStringParameter(){

    QMap <QString, QStringList>* stringParameter = new QMap<QString, QStringList>;

    QStringList SMRTypes;

    SMRTypes.append("1.5''");
    SMRTypes.append("1.0''");
    SMRTypes.append("0.5''");

    stringParameter->insert("active probe",SMRTypes);

    return stringParameter;

}

QStringList PseudoTracker::selfDefinedActions()
{
    QStringList ownActions;

    ownActions.append("echo");

    return ownActions;
}

bool PseudoTracker::doSelfDefinedAction(QString a)
{
    if(a == "echo"){
      writeToConsole(a);
    }
    return true;
}

QMap<QString, double>* PseudoTracker::getDefaultAccuracy()
{
    QMap<QString,double> *defaultAccuracy = new QMap<QString,double>;

    defaultAccuracy->insert("sigmaAzimuth",0.000001570);
    defaultAccuracy->insert("sigmaZenith",0.000001570);
    defaultAccuracy->insert("sigmaDistance",0.000025);
    defaultAccuracy->insert("sigmaX",0.000025);
    defaultAccuracy->insert("sigmaY",0.000025);
    defaultAccuracy->insert("sigmaZ",0.000025);
    defaultAccuracy->insert("sigmaTempDeg", 0.5);
    defaultAccuracy->insert("sigmaAngleXZ", 0.000001570);
    defaultAccuracy->insert("sigmaAngleYZ", 0.000001570);

    return defaultAccuracy;
}

void PseudoTracker::abortAction()
{
    //abort action
}


//! connect app with laser tracker
bool PseudoTracker::connectSensor(ConnectionConfig *cConfig){

    if(cConfig != NULL){
        qDebug() << cConfig->ip;
        qDebug() << cConfig->port;
        isConnected = true;
        QThread::msleep(1000);
        return true;
    }else{
        qDebug() << "null pointer";
        return false;
    }



}

//! disconnect app with laser tracker
bool PseudoTracker::disconnectSensor(){
    qDebug() << "pseudo tracker disconnect";
    isConnected = false;
    QThread::msleep(1000);
    return true;
}

//! starts initialization
bool PseudoTracker::initialize(){

    qDebug() << "pseudo tracker is initialized";
    myInit = true;
    QThread::msleep(1000);
    return true;
}

//! move laser tracker to specified position
bool PseudoTracker::move(double azimuth, double zenith, double distance,bool isrelativ){

    qDebug() << "pseudo tracker is moved to:" << azimuth << "," << zenith << "," << distance << "," << isrelativ ;
    myAzimuth = azimuth;
    myZenith = zenith;
    myDistance = distance;


    QThread::msleep(1000);
    return true;
}

bool PseudoTracker::move(double x, double y, double z){

    qDebug() << "pseudo tracker is moved to:" << x << "," << y << "," << z;
    myAzimuth = qAtan2(y,x);
    myDistance = qSqrt(x*x+y*y+z*z);
    myZenith = acos(z/myDistance);

    QThread::msleep(1000);
    return true;
}

//! sets laser tracke to home position
bool PseudoTracker::home(){

    qDebug() << "pseudo tracker is moved to home" ;
    QThread::msleep(1000);
    return true;

}

//! turns motors on or off
bool PseudoTracker::changeMotorState(){

    qDebug() << "pseudo tracker changed motor state" ;
    if(myMotor){
        myMotor = false;
    }else{
        myMotor = true;
    }
    QThread::msleep(1000);
    return true;

}

//! toggle between frontside and backside
bool PseudoTracker::toggleSightOrientation(){

    qDebug() << "pseudo tracker toggeld Sight orientation" ;
    if(side = 1){
       side = 2;
    }else{
        side = 1;
    }
    QThread::msleep(1000);
    return true;
}

bool PseudoTracker::compensation() {
    qDebug() << "compensation successful";
    QThread::msleep(5000);
    myCompIt = true;
    return true;
}


QList<Reading*> PseudoTracker::measure(MeasurementConfig *mc){

    switch (mc->typeOfReading) {
    case Configuration::ePolar:{
        return measurePolar(mc);
        break;
    }case Configuration::eDistance:{
        return measureDistance(mc);
        break;
    }case Configuration::eDirection:{
        return measureDirection(mc);
        break;
    }case Configuration::eCartesian:{
        return measureCartesian(mc);
        break;
    }
    default:
        break;
    }

    QList<Reading*> readings;
    return readings;
}

QVariantMap PseudoTracker::readingStream(Configuration::ReadingTypes streamFormat)
{

    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    QVariantMap m;

        Reading r;

        r.rPolar.azimuth = myAzimuth;
        r.rPolar.zenith = myZenith;
        r.rPolar.distance = myDistance;
        r.rPolar.isValid = true;

        r.toCartesian();

        double dx = ((double) std::rand()/RAND_MAX)*(10.0-1.0)+1.0;
        double dy = ((double) std::rand()/RAND_MAX)*(10.0-1.0)+1.0;
        double dz = ((double) std::rand()/RAND_MAX)*(10.0-1.0)+1.0;

        dx = dx/100;
        dy = dy/100;
        dz = dz/100;

        x =r.rCartesian.xyz.getAt(0)+dx;
        y =r.rCartesian.xyz.getAt(1)+dy;
        z =r.rCartesian.xyz.getAt(2)+dz;

        m.insert("x",x);
        m.insert("y",y);
        m.insert("z",z);

    QThread::msleep(300);


    return m;

}

bool PseudoTracker::getConnectionState()
{
    return isConnected;
}

bool PseudoTracker::isReadyForMeasurement()
{
    return true;
}

QMap<QString, QString> PseudoTracker::getSensorStats()
{
    QMap<QString, QString> stats;

    stats.insert("connected",QString::number(isConnected));
    stats.insert("side", QString::number(side));
    stats.insert("myAzimuth", QString::number(myAzimuth));
    stats.insert("myZenith", QString::number(myZenith));
    stats.insert("myDistance", QString::number(myDistance));
    stats.insert("myMotor", QString::number(myMotor));
    stats.insert("myInit", QString::number(myInit));
    stats.insert("myCompIt", QString::number(myCompIt));

    return stats;

}

bool PseudoTracker::isBusy()
{
    return false;
}


QList<Reading*> PseudoTracker::measurePolar(MeasurementConfig *m){

    QList<Reading*> readings;

    Reading *p = new Reading();
    p->typeofReading = m->typeOfReading;



    /*double daz = ((double) std::rand()/RAND_MAX)*(10.0-1.0)+1.0;
    double dze = ((double) std::rand()/RAND_MAX)*(10.0-1.0)+1.0;
    double dd = ((double) std::rand()/RAND_MAX)*(20.0-1.0)+1.0;

    daz = daz/1000;
    dze = dze/1000;
    dd = dd/10000;

    p->rPolar.azimuth = myAzimuth+daz;
    p->rPolar.zenith = myZenith+dze;
    p->rPolar.distance = myDistance+dd;*/

    p->rPolar.azimuth = myAzimuth;
    p->rPolar.zenith = myZenith;
    p->rPolar.distance = myDistance;

    this->noisyPolarReading(p);

    p->rPolar.fsBs = m->face;

    p->instrument = this;
    p->measuredAt = QDateTime::currentDateTime();

    QThread::msleep(1000);

    readings.append(p);
    return readings;

}

QList<Reading*> PseudoTracker::measureDistance(MeasurementConfig *m){

    QList<Reading*> readings;

    Reading *p = new Reading();

    double dd = ((double) std::rand()/RAND_MAX)*(20.0-1.0)+1.0;

    dd = dd/10000;

    p->rDistance.distance = myDistance + dd;
    p->instrument = this;
    p->measuredAt = QDateTime::currentDateTime();

    QThread::msleep(1000);

    readings.append(p);
    return readings;
}

QList<Reading*> PseudoTracker::measureDirection(MeasurementConfig *m){

    QList<Reading*> readings;

    Reading *p = new Reading();

    double daz = ((double) std::rand()/RAND_MAX)*(10.0-1.0)+1.0;
    double dze = ((double) std::rand()/RAND_MAX)*(10.0-1.0)+1.0;

    daz = daz/1000;
    dze = dze/1000;

    p->rDirection.azimuth = myAzimuth+daz;
    p->rDirection.zenith = myZenith+dze;
    p->rDirection.fsBs = m->face;

    p->instrument = this;
    p->measuredAt = QDateTime::currentDateTime();

    QThread::msleep(1000);

    readings.append(p);
    return readings;
}

QList<Reading*> PseudoTracker::measureCartesian(MeasurementConfig *m){

    QList<Reading*> readings;

    Reading *p = new Reading();

    double dx = ((double) std::rand()/RAND_MAX)*(30.0-1.0)+1.0;
    double dy = ((double) std::rand()/RAND_MAX)*(30.0-1.0)+1.0;
    double dz = ((double) std::rand()/RAND_MAX)*(30.0-1.0)+1.0;

    dx = dx/10000;
    dy = dy/10000;
    dz = dz/10000;

    p->rCartesian.xyz.setAt(0, (myDistance * qSin(myZenith) * qCos(myAzimuth))+dx);
    p->rCartesian.xyz.setAt(1, (myDistance * qSin(myZenith) * qSin(myAzimuth))+dy);
    p->rCartesian.xyz.setAt(2, (myDistance * qCos(myZenith))+dz);
    p->rCartesian.xyz.setAt(3, 1);

    p->instrument = this;
    p->measuredAt = QDateTime::currentDateTime();

    QThread::msleep(1000);

    readings.append(p);
    return readings;
}

double PseudoTracker::randomX(int d, double m, double s)
{
    double rv = 0.0;
     // d = Art der Verteilung:
     // 0 : Wert zw. m-s und m+s (gleichverteilt)
     // 1 : Wert normalverteilt um m mit s=s
     // 2 : Dreiecksverteilung

      switch(d)
      {
        case 0 :
          rv = 2.0*(double)rand()/(double)RAND_MAX-1.0;  // Wert zw.  -1 und   1 (gleichverteilt)
          rv = m+s*rv;                                 // Wert zw. m-s und m+s (gleichverteilt)
          break;
        case 1 :
          rv = randomNorm();                          // Wert normalverteilt um Null mit s=1
          rv = m+s*rv;                                 // Wert normalverteilt um m  mit s=s
          break;
        case 2 :
          rv = randomTriangular(m,m-s,m+s);                                     // Wert dreiecksverteilt
          break;
        default :
          rv = -2;                                      // Wert = m, d.h. const. - keine Verteilung
          break;
        }

      return(rv);
}

double PseudoTracker::randomNorm()
{
    static int   iset=0;
    static double gset;
    double rnum;
    double fac;
    double rsq;
    double v1,v2;

    if ( iset == 0 )
      {
        do {
      v1  = 2.0*(double)rand()/(double)RAND_MAX-1.0;
      v2  = 2.0*(double)rand()/(double)RAND_MAX-1.0;
      rsq = v1*v1+v2*v2;
        } while ( rsq >= 1.0 || rsq == 0.0);

        fac = sqrt(-2.0*log(rsq)/rsq);
        rnum = v1*fac;
        gset = v2*fac;
        iset = 1;
      }
    else
      {
        rnum = gset;
        iset = 0;
      }
    return rnum;
}

/*
 *Dreicecksverteilung
 */
double PseudoTracker::randomTriangular(double c, double a, double b)
{
    double U = (double) rand() / (double) RAND_MAX;
       double F = (c - a) / (b - a);
       if (U <= F)
          return a + sqrt(U * (b - a) * (c - a));
       else
           return b - sqrt((1 - U) * (b - a) * (b - c));
}

void PseudoTracker::noisyPolarReading(Reading *r)
{
    if(r->typeofReading != Configuration::ePolar){
        return;
    }

    double lambda = this->myConfiguration->doubleParameter.value("lambda [mm]")/1000;
    double mu = this->myConfiguration->doubleParameter.value("mu");
    double ex = this->myConfiguration->doubleParameter.value("ex [mm]")/1000;
    double by = this->myConfiguration->doubleParameter.value("by [mm]")/1000;
    double bz = this->myConfiguration->doubleParameter.value("bz [mm]")/1000;
    double alpha = this->myConfiguration->doubleParameter.value("alpha [arc sec]")*(M_PI/648000.0);
    double gamma = this->myConfiguration->doubleParameter.value("gamma [arc sec]")*(M_PI/648000.0);
    double Aa1 = this->myConfiguration->doubleParameter.value("Aa1 [arc sec]")*(M_PI/648000.0);
    double Ba1 = this->myConfiguration->doubleParameter.value("Ba1 [arc sec]")*(M_PI/648000.0);
    double Aa2 = this->myConfiguration->doubleParameter.value("Aa2 [arc sec]")*(M_PI/648000.0);
    double Ba2 = this->myConfiguration->doubleParameter.value("Ba2 [arc sec]")*(M_PI/648000.0);
    double Ae0 = this->myConfiguration->doubleParameter.value("Ae0 [arc sec]")*(M_PI/648000.0);
    double Ae1 = this->myConfiguration->doubleParameter.value("Ae1 [arc sec]")*(M_PI/648000.0);
    double Be1 = this->myConfiguration->doubleParameter.value("Be1 [arc sec]")*(M_PI/648000.0);
    double Ae2 = this->myConfiguration->doubleParameter.value("Ae2 [arc sec]")*(M_PI/648000.0);
    double Be2 = this->myConfiguration->doubleParameter.value("Be2 [arc sec]")*(M_PI/648000.0);

    lambda = randomX(1,0,lambda);
    lambda = randomX(1,0,mu);
    lambda = randomX(1,0,ex);
    lambda = randomX(1,0,by);
    lambda = randomX(1,0,bz);
    lambda = randomX(1,0,alpha);
    lambda = randomX(1,0,gamma);
    lambda = randomX(1,0,Aa1);
    lambda = randomX(1,0,Ba1);
    lambda = randomX(1,0,Aa2);
    lambda = randomX(1,0,Ba2);
    lambda = randomX(1,0,Ae0);
    lambda = randomX(1,0,Ae1);
    lambda = randomX(1,0,Be1);
    lambda = randomX(1,0,Ae2);
    lambda = randomX(1,0,Be2);

    double az = r->rPolar.azimuth;
    double ze = r->rPolar.zenith;
    double d = r->rPolar.distance;

    d = (1+mu)*d+lambda;

    double azF1 = Aa1*cos(az) + Ba1*sin(az);
    double azF2 = Aa2*cos(2*az) + Ba2*sin(2*az);

    az = az+azF1+azF2;

    double zeF1 = Ae1*cos(ze) + Be1*sin(ze);
    double zeF2 = Ae2*cos(2*ze) + Be2*sin(2*ze);

    ze = ze+Ae0+zeF1+zeF2;


    r->rPolar.azimuth =  az;
    r->rPolar.zenith= ze;
    r->rPolar.distance = d;

}
