#include "p_pseudotracker.h"


/*!
 * \brief PseudoTracker::PseudoTracker
 */
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

/*!
 * \brief PseudoTracker::getMetaData
 * \return
 */
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

/*!
 * \brief PseudoTracker::getSupportedReadingTypes
 * \return
 */
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

/*!
 * \brief PseudoTracker::getSupportedSensorActions
 * \return
 */
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

/*!
 * \brief PseudoTracker::getConnectionType
 * \return
 */
QList<Configuration::ConnectionTypes>* PseudoTracker::getConnectionType(){
    QList<Configuration::ConnectionTypes> *connectionTypes = new QList<Configuration::ConnectionTypes>;
    connectionTypes->append(Configuration::eNetwork);
    connectionTypes->append(Configuration::eSerial);

    return connectionTypes;
}

/*!
 * \brief PseudoTracker::getIntegerParameter
 * \return
 */
QMap<QString,int>* PseudoTracker::getIntegerParameter(){
    return NULL;
}

/*!
 * \brief PseudoTracker::getDoubleParameter
 * \return
 */
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

/*!
 * \brief PseudoTracker::getStringParameter
 * \return
 */
QMap <QString, QStringList>* PseudoTracker::getStringParameter(){

    QMap <QString, QStringList>* stringParameter = new QMap<QString, QStringList>;

    QStringList SMRTypes;

    SMRTypes.append("1.5''");
    SMRTypes.append("1.0''");
    SMRTypes.append("0.5''");

    stringParameter->insert("active probe",SMRTypes);

    return stringParameter;

}

/*!
 * \brief PseudoTracker::selfDefinedActions
 * \return
 */
QStringList PseudoTracker::selfDefinedActions()
{
    QStringList ownActions;

    ownActions.append("echo");

    return ownActions;
}

/*!
 * \brief PseudoTracker::doSelfDefinedAction
 * \param a
 * \return
 */
bool PseudoTracker::doSelfDefinedAction(QString a)
{
    if(a == "echo"){
      writeToConsole(a);
    }
    return true;
}

/*!
 * \brief PseudoTracker::getDefaultAccuracy
 * \return
 */
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

/*!
 * \brief PseudoTracker::abortAction
 */
void PseudoTracker::abortAction()
{
    //abort action
}


/*!
 * \brief PseudoTracker::connectSensor
 * \param cConfig
 * \return
 *
 *  connect app with laser tracker
 */
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


/*!
 * \brief PseudoTracker::disconnectSensor
 * \return
 *
 *  disconnect app with laser tracker
 */
bool PseudoTracker::disconnectSensor(){
    qDebug() << "pseudo tracker disconnect";
    isConnected = false;
    QThread::msleep(1000);
    return true;
}

/*!
 * \brief PseudoTracker::initialize
 * \return
 *
 *  starts initialization
 */
bool PseudoTracker::initialize(){

    qDebug() << "pseudo tracker is initialized";
    myInit = true;
    QThread::msleep(1000);
    return true;
}

/*!
 * \brief PseudoTracker::move
 * \param azimuth
 * \param zenith
 * \param distance
 * \param isrelativ
 * \return
 *
 *  move laser tracker to specified position
 */
bool PseudoTracker::move(double azimuth, double zenith, double distance,bool isrelativ){

    qDebug() << "pseudo tracker is moved to:" << azimuth << "," << zenith << "," << distance << "," << isrelativ ;
    myAzimuth = azimuth;
    myZenith = zenith;
    myDistance = distance;


    QThread::msleep(1000);
    return true;
}

/*!
 * \brief PseudoTracker::move
 * \param x
 * \param y
 * \param z
 * \return
 */
bool PseudoTracker::move(double x, double y, double z){

    qDebug() << "pseudo tracker is moved to:" << x << "," << y << "," << z;
    myAzimuth = qAtan2(y,x);
    myDistance = qSqrt(x*x+y*y+z*z);
    myZenith = acos(z/myDistance);

    QThread::msleep(1000);
    return true;
}

/*!
 * \brief PseudoTracker::home
 * \return
 *
 *  sets laser tracke to home position
 */
bool PseudoTracker::home(){

    qDebug() << "pseudo tracker is moved to home" ;
    QThread::msleep(1000);
    return true;

}

/*!
 * \brief PseudoTracker::changeMotorState
 * \return
 *
 *  turns motors on or off
 */
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


/*!
 * \brief PseudoTracker::toggleSightOrientation
 * \return
 *
 *  toggle between frontside and backside
 */
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

/*!
 * \brief PseudoTracker::compensation
 * \return
 */
bool PseudoTracker::compensation() {
    qDebug() << "compensation successful";
    QThread::msleep(5000);
    myCompIt = true;
    return true;
}

/*!
 * \brief PseudoTracker::measure
 * \param mc
 * \return
 */
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

/*!
 * \brief PseudoTracker::readingStream
 * \param streamFormat
 * \return
 */
QVariantMap PseudoTracker::readingStream(Configuration::ReadingTypes streamFormat)
{

    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    double distance = 0.0;
    double azimuth = 0.0;
    double zenith = 0.0;

    QVariantMap m;

    Reading r;

    switch (streamFormat) {
    case Configuration::ePolar:

        r.rPolar.azimuth = myAzimuth;
        r.rPolar.zenith = myZenith;
        r.rPolar.distance = myDistance;
        r.rPolar.isValid = true;

        r.typeofReading = Configuration::ePolar;
        this->noisyPolarReading(&r);

        azimuth = r.rPolar.azimuth;
        zenith = r.rPolar.zenith;
        distance = r.rPolar.distance;

        m.insert("azimuth",azimuth);
        m.insert("zenith",zenith);
        m.insert("distance",distance);

        break;
    case Configuration::eCartesian:

        r.rPolar.azimuth = myAzimuth;
        r.rPolar.zenith = myZenith;
        r.rPolar.distance = myDistance;
        r.rPolar.isValid = true;
        r.typeofReading = Configuration::ePolar;

        this->noisyPolarReading(&r);

        r.typeofReading = Configuration::eCartesian;
        r.toCartesian();

        x =r.rCartesian.xyz.getAt(0);
        y =r.rCartesian.xyz.getAt(1);
        z =r.rCartesian.xyz.getAt(2);

        m.insert("x",x);
        m.insert("y",y);
        m.insert("z",z);

        break;
    case Configuration::eDistance:

        r.rDistance.distance = myDistance;
        r.rDistance.isValid = true;

        r.typeofReading = Configuration::eDistance;

        distance = r.rDistance.distance;

        m.insert("distance",distance);

        break;
    case Configuration::eDirection:

        r.rDirection.azimuth = myAzimuth;
        r.rDirection.zenith = myZenith;
        r.rDirection.isValid = true;

        r.typeofReading = Configuration::eDirection;

        azimuth = r.rDirection.azimuth;
        zenith = r.rDirection.zenith;

        m.insert("azimuth",azimuth);
        m.insert("zenith",zenith);

        break;
    case Configuration::eTemperatur:
        break;
    case Configuration::eLevel:
        break;
    case Configuration::eUndefined:
        break;
    default:
        break;
    }

    QThread::msleep(300);

    return m;

}

/*!
 * \brief PseudoTracker::getConnectionState
 * \return
 */
bool PseudoTracker::getConnectionState()
{
    return isConnected;
}

/*!
 * \brief PseudoTracker::isReadyForMeasurement
 * \return
 */
bool PseudoTracker::isReadyForMeasurement()
{
    return true;
}

/*!
 * \brief PseudoTracker::getSensorStats
 * \return
 */
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

    QThread::msleep(300);

    return stats;

}

/*!
 * \brief PseudoTracker::isBusy
 * \return
 */
bool PseudoTracker::isBusy()
{
    return false;
}

/*!
 * \brief PseudoTracker::measurePolar
 * \param m
 * \return
 */
QList<Reading*> PseudoTracker::measurePolar(MeasurementConfig *m){

    QList<Reading*> readings;

    Reading *p = new Reading();
    p->typeofReading = m->typeOfReading;


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

/*!
 * \brief PseudoTracker::measureDistance
 * \param m
 * \return
 */
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

/*!
 * \brief PseudoTracker::measureDirection
 * \param m
 * \return
 */
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

/*!
 * \brief PseudoTracker::measureCartesian
 * \param m
 * \return
 *
 */
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

/*!
 * \brief PseudoTracker::randomX
 * \param d
 * \param m
 * \param s
 * \return
 *
 *  This method generates a random number
 *  depending on the specified distribution.
 *
 *  d = method of distribution:
 *  0 : uniform distribution (m-s,m+s)
 *  1 : normal distributed around m (s=sigma)
 *  2 : triangular distribution around m (m-s,m+s)
 */
double PseudoTracker::randomX(int d, double m, double s)
{
    double rv = 0.0;


      switch(d)
      {
        case 0 :
          rv = 2.0*(double)rand()/(double)RAND_MAX-1.0;
          rv = m+s*rv;
          break;
        case 1 :
          rv = randomNorm();
          rv = m+s*rv;
          break;
        case 2 :
          rv = randomTriangular(m,m-s,m+s);
          break;
        default :
          rv = -2;
          break;
        }

      return(rv);
}

/*!
 * \brief PseudoTracker::randomNorm
 * \return
 *
 * This method generates a normally distributed random number.
 */
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

/*!
 * \brief PseudoTracker::randomTriangular
 * \param c
 * \param a
 * \param b
 * \return
 *
 *  This method generates a triangular distributed random number.
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

/*!
 * \brief PseudoTracker::noisyPolarReading
 * \param r
 *
 * This method noisy a given polar Reading using randomly generated numbers.
 * It is used the error model described by  Hughes B, Sun W, Forbes A, Lewis A
 * 2010 Determining laser tracker alignment errors using
 * a network measurement CMSC Journal Autumn 2010, 26-32
 *
 */
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
    mu = randomX(1,0,mu);
    ex = randomX(1,0,ex);
    by = randomX(1,0,by);
    bz = randomX(1,0,bz);
    alpha = randomX(1,0,alpha);
    gamma = randomX(1,0,gamma);
    Aa1 = randomX(1,0,Aa1);
    Ba1 = randomX(1,0,Ba1);
    Aa2 = randomX(1,0,Aa2);
    Ba2 = randomX(1,0,Ba2);
    Ae0 = randomX(1,0,Ae0);
    Ae1 = randomX(1,0,Ae1);
    Be1 = randomX(1,0,Be1);
    Ae2 = randomX(1,0,Ae2);
    Be2 = randomX(1,0,Be2);

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

    OiVec ebb;
    ebb.add(-ex);
    ebb.add(by);
    ebb.add(bz);

    OiVec e00;
    e00.add(ex);
    e00.add(0.0);
    e00.add(0.0);

    OiVec xAxis;
    xAxis.add(1);
    xAxis.add(0);
    xAxis.add(0);


    OiMat Rz_Azimuth = OiMat::getRotationMatrix(az,Rotation::Z_AXIS);
    OiMat Rx_alpha = OiMat::getRotationMatrix(alpha,Rotation::X_AXIS);
    OiMat Ry_zenith = OiMat::getRotationMatrix(ze-(M_PI/2.0),Rotation::Y_AXIS);
    OiMat Rx_minusAlpha = OiMat::getRotationMatrix(-1.0*alpha,Rotation::X_AXIS);
    OiMat Rz_gamma = OiMat::getRotationMatrix(gamma,Rotation::Z_AXIS);


    OiVec b(3);
    b = Rz_Azimuth*e00 + Rz_Azimuth*Rx_alpha*Ry_zenith*Rx_minusAlpha*ebb;;

    OiVec n(3);
    n = Rz_Azimuth*Rx_alpha*Ry_zenith*Rx_minusAlpha*Rz_gamma*xAxis;


    OiVec p(3);
    p = b+d*n;

    r->rPolar.azimuth = qAtan2(p.getAt(1),p.getAt(0));
    r->rPolar.distance = qSqrt(p.getAt(0)*p.getAt(0)+p.getAt(1)*p.getAt(1)+p.getAt(2)*p.getAt(2));
    r->rPolar.zenith = acos(p.getAt(2)/r->rPolar.distance);

   /* r->rPolar.azimuth =  az;
    r->rPolar.zenith= ze;
    r->rPolar.distance = d;*/

}
