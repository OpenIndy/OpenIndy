#include "reading.h"

#include "sensor.h"
#include "observation.h"

Reading::Reading()
{
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
    qDebug() << "distance: " << dist;
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

/*!
 * \brief Reading::errorPropagationPolarToCart
 * Variance propagation to get sigma values for cartesian coordinates
 * \return
 */
OiVec Reading::errorPropagationPolarToCart(){
    OiVec sigmaCartXyz = OiVec(3);

    //partial derivative
    double dxds = qSin(this->rPolar.zenith)*qCos(this->rPolar.azimuth);
    double dxdr = this->rPolar.distance * qSin(this->rPolar.zenith)*-qSin(this->rPolar.azimuth);
    double dxdv = this->rPolar.distance * qCos(this->rPolar.zenith)*qCos(this->rPolar.azimuth);

    double dyds = qSin(this->rPolar.zenith)*qSin(this->rPolar.azimuth);
    double dydr = this->rPolar.distance * qSin(this->rPolar.zenith)*qCos(this->rPolar.azimuth);
    double dydv = this->rPolar.distance * qCos(this->rPolar.zenith)*qSin(this->rPolar.azimuth);

    double dzds = qCos(this->rPolar.zenith);
    double dzdr = 0.0;
    double dzdv = this->rPolar.distance * -qSin(this->rPolar.zenith);

    //sigma reading
    double sS = this->rPolar.sigmaDistance;
    double sR = this->rPolar.sigmaAzimuth;
    double sV = this->rPolar.sigmaZenith;

    //sigma xyz
    double sigmaX = (dxds*dxds)*(sS*sS)+(dxdr*dxdr)*(sR*sR)+(dxdv*dxdv)*(sV*sV);
    sigmaX = qSqrt(sigmaX);

    double sigmaY = (dyds*dyds)*(sS*sS)+(dydr*dydr)*(sR*sR)+(dydv*dydv)*(sV*sV);
    sigmaY = qSqrt(sigmaY);

    double sigmaZ = (dzds*dzds)*(sS*sS)+(dzdr*dzdr)*(sR*sR)+(dzdv*dzdv)*(sV*sV);
    sigmaZ = qSqrt(sigmaZ);

    sigmaCartXyz.setAt(0,sigmaX);
    sigmaCartXyz.setAt(1,sigmaY);
    sigmaCartXyz.setAt(2,sigmaZ);
    sigmaCartXyz.add(1.0);


    return sigmaCartXyz;
}
