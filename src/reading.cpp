#include "reading.h"

#include "sensor.h"
#include "observation.h"

Reading::Reading()
{
    this->measuredAt = QDateTime::currentDateTime();
}

Reading::~Reading(){

}

void Reading::toCartesian(){

    if(rPolar.isValid){

        this->rCartesian.xyz = OiVec(4);
        this->rCartesian.xyz.setAt( 0, this->rPolar.distance * qSin(this->rPolar.zenith) * qCos(this->rPolar.azimuth) );
        this->rCartesian.xyz.setAt( 1, this->rPolar.distance * qSin(this->rPolar.zenith) * qSin(this->rPolar.azimuth) );
        this->rCartesian.xyz.setAt( 2, this->rPolar.distance * qCos(this->rPolar.zenith) );
        this->rCartesian.xyz.setAt( 3, 1.0 );

    }

}

OiVec Reading::toCartesian(double az, double ze, double dist){
    OiVec g(4);
    g.setAt( 0, dist * qSin(ze) * qCos(az) );
    g.setAt( 1, dist * qSin(ze) * qSin(az) );
    g.setAt( 2, dist * qCos(ze) );
    g.setAt( 3, 1.0 );
    return g;
}

void Reading::toPolar(){
    return;
}

OiVec Reading::toPolar(double x, double y, double z){
    OiVec g(4);

    double azimuth = qAtan2(y,x);
    double s = qSqrt(x*x+y*y+z*z);
    double zenith = acos(z/s);

    g.setAt( 0, azimuth);
    g.setAt( 1, zenith);
    g.setAt( 2, s);
    g.setAt( 3, 1.0 );
    return g;
}

void Reading::makeBackup()
{
    this->backupPolar = this->rPolar;
    this->backupCartesian = this->rCartesian;
    this->backupDirection = this->rDirection;
    this->backupDistance = this->rDistance;
    this->backupTemperature = this->rTemperature;
    this->backupUndefined = this->rUndefined;
    this->backupLevel = this->rLevel;
}

void Reading::restoreBackup()
{
    this->rPolar = this->backupPolar;
    this->rCartesian = this->backupCartesian;
    this->rDirection = this->backupDirection;
    this->rDistance = this->backupDistance;
    this->rTemperature = this->backupTemperature;
    this->rUndefined = this->backupUndefined;
    this->rLevel = this->backupLevel;
}

/*!
 * \brief Reading::errorPropagationPolarToCart
 * Variance propagation to get sigma values for cartesian coordinates
 * \return
 */
OiVec Reading::errorPropagationPolarToCart(){
    OiVec sigmaCartXyz;

    OiMat F(3,3);
    F.setAt(0, 0, qSin(this->rPolar.zenith) * qCos(this->rPolar.azimuth));
    F.setAt(0, 1, this->rPolar.distance * qSin(this->rPolar.zenith) * -qSin(this->rPolar.azimuth));
    F.setAt(0, 2, this->rPolar.distance * qCos(this->rPolar.zenith) * qCos(this->rPolar.azimuth));
    F.setAt(1, 0, qSin(this->rPolar.zenith) * qSin(this->rPolar.azimuth));
    F.setAt(1, 1, this->rPolar.distance * qSin(this->rPolar.zenith) * qCos(this->rPolar.azimuth));
    F.setAt(1, 2, this->rPolar.distance * qCos(this->rPolar.zenith) * qSin(this->rPolar.azimuth));
    F.setAt(2, 0, qCos(this->rPolar.zenith));
    F.setAt(2, 1, 0.0);
    F.setAt(2, 2, this->rPolar.distance * -qSin(this->rPolar.zenith));

    OiMat Sll(3,3);
    Sll.setAt(0, 0, this->rPolar.sigmaDistance * this->rPolar.sigmaDistance);
    Sll.setAt(1, 1, this->rPolar.sigmaAzimuth * this->rPolar.sigmaAzimuth);
    Sll.setAt(2, 2, this->rPolar.sigmaZenith * this->rPolar.sigmaZenith);

    OiMat Qxx = F * Sll * F.t();

    //transform Qxx into homogeneous coordinates
    OiMat Qxx_hc(4,4);
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            Qxx_hc.setAt(i,j, Qxx.getAt(i,j));
        }
    }

    sigmaCartXyz.add(qSqrt(Qxx.getAt(0,0)));
    sigmaCartXyz.add(qSqrt(Qxx.getAt(1,1)));
    sigmaCartXyz.add(qSqrt(Qxx.getAt(2,2)));
    sigmaCartXyz.add(1.0);

    if(this->obs != NULL){
        this->obs->myStatistic.qxx = Qxx_hc;
        this->obs->myStatistic.s0_apriori = 1.0;
        this->obs->myOriginalStatistic = this->obs->myStatistic;
    }

    return sigmaCartXyz;
}
