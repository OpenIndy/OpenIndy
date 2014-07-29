#include "simplepolarmeasurement.h"

SimplePolarMeasurement::SimplePolarMeasurement()
{
    this->distributions.append("normal");
    this->distributions.append("uniform");
    this->distributions.append("triangular");

    newIteration = true;
}

PluginMetaData *SimplePolarMeasurement::getMetaData()
{
    PluginMetaData* metaData = new PluginMetaData();

    metaData->name = "SimplePolarMeasurement";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "mlux";
    metaData->description = "simulation of a simple polar measurement system";
    metaData->iid = "de.openIndy.Plugin.Simulation.v001";
    //...

    return metaData;
}

double erfFunc(double x)
{
    // constants
    double a1 =  0.254829592;
    double a2 = -0.284496736;
    double a3 =  1.421413741;
    double a4 = -1.453152027;
    double a5 =  1.061405429;
    double p  =  0.3275911;

    // Save the sign of x
    int sign = 1;
    if (x < 0)
        sign = -1;
    x = fabs(x);

    // A&S formula 7.1.26
    double t = 1.0/(1.0 + p*x);
    double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);

    return sign*y;
}

double densityNormal(double x, double expectation, double uncertainty,double lowerLimit, double upperLimit)
{
    double e = -0.5*(((x-expectation)/(uncertainty))*((x-expectation)/(uncertainty)));

    double result = (1.0/(uncertainty*sqrt(2.0*M_PI)))*qExp(e);

    return result;
}

double distributionNormal(double x, double expectation, double uncertainty,double lowerLimit, double upperLimit)
{
    return 0.5*(1.0+erfFunc((x-expectation)/(sqrt(2.0*uncertainty*uncertainty))));

}

double densityUniform(double x, double expectation, double uncertainty,double lowerLimit, double upperLimit)
{

    double result = 1/(upperLimit-lowerLimit);

    return result;
}

double distributionUniform(double x, double expectation, double uncertainty,double lowerLimit, double upperLimit)
{
    double result = (x-lowerLimit)/(upperLimit-lowerLimit);

    return result;
}


double densityTriangular(double x, double expectation, double uncertainty,double lowerLimit, double upperLimit)
{

    double result = 0.0;

    if(lowerLimit <= x && x <= expectation){
         result = (2*(x-lowerLimit))/((upperLimit-lowerLimit)*(expectation-lowerLimit));
    }else if(expectation < x && x <= upperLimit) {
         result = (2*(upperLimit-x))/((upperLimit-lowerLimit)*(upperLimit-expectation));
    }

    return result;
}

double distributionTriangular(double x, double expectation, double uncertainty,double lowerLimit, double upperLimit)
{
    double result = 0.0;

    if(lowerLimit <= x && x <= expectation){
         result = ((x-lowerLimit)*(x-lowerLimit))/((upperLimit-lowerLimit)*(expectation-lowerLimit));
    }else if(expectation < x && x <= upperLimit) {
         result =1.0- (((upperLimit-x)*(upperLimit-x))/((upperLimit-lowerLimit)*(upperLimit-expectation)));
    }

    return result;
}


QMap<QString, UncertaintyComponent> SimplePolarMeasurement::getSensorUncertainties()
{
    QMap<QString, UncertaintyComponent> sensorUncertainties;

    //######################### lambda #########################
    UncertaintyComponent lambda;

    lambda.name = "lambda";
    lambda.value = 0.0;
    lambda.uncertainty = 0.000403;
    lambda.distribution = this->distributions.at(0);
    lambda.distributions = this->distributions;
    lambda.errorUnit = "[mm]";

    lambda.description="lambda - Range offset in millimeter";

    sensorUncertainties.insert("lambda",lambda);

    //######################### mu #########################
    UncertaintyComponent mu;

    mu.name = "mu";
    mu.value = 0.0;
    mu.uncertainty = 0.000005;
    mu.distribution = this->distributions.at(0);
    mu.distributions = this->distributions;
    mu.errorUnit = "[-]";

    mu.description="mu - Scale factor for range";

    sensorUncertainties.insert("mu",mu);

    //######################### ex #########################
    UncertaintyComponent ex;

    ex.name = "ex";
    ex.value = 0.0;
    ex.uncertainty = 0.0000122;
    ex.distribution = this->distributions.at(0);
    ex.distributions = this->distributions;
    ex.errorUnit = "[mm]";

    ex.description="ex - Transit axis offset from the standing axis";

    sensorUncertainties.insert("ex",ex);

    //######################### by #########################
    UncertaintyComponent by;

    by.name = "by";
    by.value = 0.0;
    by.uncertainty = 0.0000654;
    by.distribution = this->distributions.at(0);
    by.distributions = this->distributions;
    by.errorUnit =  "[mm]";

    by.description="by - Beam offset (y-direction) from the origin";

    sensorUncertainties.insert("by",by);

    //######################### bz #########################
    UncertaintyComponent bz;

    bz.name = "bz";
    bz.value = 0.0;
    bz.uncertainty = 0.0000974;
    bz.distribution = this->distributions.at(0);
    bz.distributions = this->distributions;
    bz.errorUnit =  "[mm]";

    bz.description="bz - Beam offset (z-direction) from the origin";

    sensorUncertainties.insert("bz",bz);

    //######################### alpha #########################
    UncertaintyComponent alpha;

    alpha.name = "alpha";
    alpha.value = 0.0;
    alpha.uncertainty = 0.128;
    alpha.distribution = this->distributions.at(0);
    alpha.distributions = this->distributions;
    alpha.errorUnit =  "[arcsec]";

    alpha.description="alpha - Transit axis angle in the yz-plane";

    sensorUncertainties.insert("alpha",alpha);

    //######################### gamma #########################
    UncertaintyComponent gamma;

    gamma.name = "gamma";
    gamma.value = 0.0;
    gamma.uncertainty = 0.079;
    gamma.distribution = this->distributions.at(0);
    gamma.distributions = this->distributions;
    gamma.errorUnit = "[arcsec]";

    gamma.description="gamma - Beam axis angle in the xy-plane";

    sensorUncertainties.insert("gamma",gamma);

    //######################### Aa1 #########################
    UncertaintyComponent Aa1;

    Aa1.name = "Aa1";
    Aa1.value = 0.0;
    Aa1.uncertainty = 0.064;
    Aa1.distribution = this->distributions.at(0);
    Aa1.distributions = this->distributions;
    Aa1.errorUnit = "[arcsec]";

    Aa1.description="Aa1 - Azimuth scale error, first order";

    sensorUncertainties.insert("Aa1",Aa1);

    //######################### Ba1 #########################
    UncertaintyComponent Ba1;

    Ba1.name = "Aa1";
    Ba1.value = 0.0;
    Ba1.uncertainty = 0.080;
    Ba1.distribution = this->distributions.at(0);
    Ba1.distributions = this->distributions;
    Ba1.errorUnit = "[arcsec]";

    Ba1.description="Ba1 - Azimuth scale error, first order";

    sensorUncertainties.insert("Ba1",Ba1);

    //######################### Aa2 #########################
    UncertaintyComponent Aa2;

    Aa2.name = "Aa2";
    Aa2.value = 0.0;
    Aa2.uncertainty = 0.073;
    Aa2.distribution = this->distributions.at(0);
    Aa2.distributions = this->distributions;
    Aa2.errorUnit = "[arcsec]";

    Aa2.description="Aa2 - Azimuth scale error, second order";

    sensorUncertainties.insert("Aa2",Aa2);

    //######################### Ba2 #########################
    UncertaintyComponent Ba2;

    Ba2.name = "Ba2";
    Ba2.value = 0.0;
    Ba2.uncertainty = 0.090;
    Ba2.distribution = this->distributions.at(0);
    Ba2.distributions = this->distributions;
    Ba2.errorUnit = "[arcsec]";

    Ba2.description="Ba2 - Azimuth scale error, second order";

    sensorUncertainties.insert("Ba2",Ba2);

    //######################### Ba2 #########################
    UncertaintyComponent Ae0;

    Ae0.name = "Ae0";
    Ae0.value = 0.0;
    Ae0.uncertainty = 0.223;
    Ae0.distribution = this->distributions.at(0);
    Ae0.distributions = this->distributions;
    Ae0.errorUnit = "[arcsec]";

    Ae0.description="Ae0 - Elevation angle offset";

    sensorUncertainties.insert("Ae0",Ae0);

    //######################### Ae1 #########################
    UncertaintyComponent Ae1;

    Ae1.name = "Ae1";
    Ae1.value = 0.0;
    Ae1.uncertainty = 0.152;
    Ae1.distribution = this->distributions.at(0);
    Ae1.distributions = this->distributions;
    Ae1.errorUnit = "[arcsec]";

    Ae1.description="Ae1 - Elevation scale error, first order";

    sensorUncertainties.insert("Ae1",Ae1);

    //######################### Be1 #########################
    UncertaintyComponent Be1;

    Be1.name = "Be1";
    Be1.value = 0.0;
    Be1.uncertainty = 0.183;
    Be1.distribution = this->distributions.at(0);
    Be1.distributions = this->distributions;
    Be1.errorUnit = "[arcsec]";

    Be1.description="Be1 - Elevation scale error, first order";

    sensorUncertainties.insert("Be1",Be1);

    //######################### Ae2 #########################
    UncertaintyComponent Ae2;

    Ae2.name = "Ae2";
    Ae2.value = 0.0;
    Ae2.uncertainty = 0.214;
    Ae2.distribution = this->distributions.at(0);
    Ae2.distributions = this->distributions;
    Ae2.errorUnit = "[arcsec]";

    Ae2.description="Ae2 - Elevation scale error, second order";

    sensorUncertainties.insert("Ae2",Ae2);

    //######################### Be2 #########################
    UncertaintyComponent Be2;

    Be2.name = "Be2";
    Be2.value = 0.0;
    Be2.uncertainty = 0.214;
    Be2.distribution = this->distributions.at(0);
    Be2.distributions = this->distributions;
    Be2.errorUnit = "[arcsec]";

    Be2.description="Be2 - Elevation scale error, second order";

    sensorUncertainties.insert("Be2",Be2);


    return sensorUncertainties;
}

QMap<QString, UncertaintyComponent> SimplePolarMeasurement::getObjectUncertainties()
{
    QMap<QString, UncertaintyComponent> objectUncertainties;

    //######################### coefficient of thermal expansion #########################
    UncertaintyComponent cOfE;

    cOfE.name = "coefficient of thermal expansion";
    cOfE.value = 11.8;
    cOfE.uncertainty = 2;
    cOfE.distribution = this->distributions.at(0);
    cOfE.distributions = this->distributions;
    cOfE.errorUnit = "1/K";

    cOfE.description="The degree of expansion divided by the change in temperaturer";

    objectUncertainties.insert("coefficientOfExpansion",cOfE);

    //######################### materialTemperature #########################
    UncertaintyComponent t;

    t.name = "material temperature";
    t.value = 20;
    t.uncertainty = 1;
    t.distribution = this->distributions.at(0);
    t.distributions = this->distributions;
    t.errorUnit = "[celsius]";

    t.description="influence of temperaturer on the object and sensor";

    objectUncertainties.insert("materialTemperature",t);


    return objectUncertainties;
}

QMap<QString, UncertaintyComponent> SimplePolarMeasurement::getEnviromentUncertainties()
{
    QMap<QString, UncertaintyComponent> enviromentUncertainties;

    //######################### temperature #########################
    UncertaintyComponent t;

    t.name = "temperature";
    t.value = 20.0;
    t.uncertainty = 0.5;
    t.distribution = this->distributions.at(0);
    t.distributions = this->distributions;
    t.errorUnit = "[celsius]";

    t.description="influence of temperaturer on the object and sensor";

    enviromentUncertainties.insert("temperature",t);

    //######################### vertical temperature gradient#########################
    UncertaintyComponent vtg;

    vtg.name = "vertical temperature gradient";
    vtg.value = 0.4;
    vtg.uncertainty = 0.1;
    vtg.distribution = this->distributions.at(0);
    vtg.distributions = this->distributions;
    vtg.errorUnit = "[celsius/m]";

    vtg.description="gradient to calculate the vertical beam refraction";

    enviromentUncertainties.insert("verticalTemperatureGradient",vtg);

    //######################### horizontal temperature gradient#########################
    UncertaintyComponent htg;

    htg.name = "horizontal temperature gradient";
    htg.value = 0.1;
    htg.uncertainty = 0.1;
    htg.distribution = this->distributions.at(0);
    htg.distributions = this->distributions;
    htg.errorUnit = "[celsius/m]";

    htg.description="gradient to calculate the horizontal beam refraction";

    enviromentUncertainties.insert("horizontalTemperatureGradient",htg);

    //######################### pressure #########################
    UncertaintyComponent p;

    p.name = "pressure";
    p.value = 101325.0 ;
    p.uncertainty = 10.0;
    p.distribution = this->distributions.at(0);
    p.distributions = this->distributions;
    p.errorUnit = "[pascal]";

    p.description="influence of pressure(air)  on the sensor";

    enviromentUncertainties.insert("pressure",p);

    //######################### vertical pressure Gradient#########################
    UncertaintyComponent hpg;

    hpg.name = "vertical pressure gradient";
    hpg.value = -0.8;
    hpg.uncertainty = 0.1;
    hpg.distribution = this->distributions.at(0);
    hpg.distributions = this->distributions;
    hpg.errorUnit = "[pascal/m]";

    hpg.description="gradient to calculate the vertical beam refraction";

    enviromentUncertainties.insert("verticalPressureGradient",hpg);

    //######################### humidity #########################
    UncertaintyComponent h;

    h.name = "humidity";
    h.value = 50.0 ;
    h.uncertainty = 1.0;
    h.distribution = this->distributions.at(0);
    h.distributions = this->distributions;
    h.errorUnit = "[percent %]";

    h.description="influence of humidity(air)  on the sensor";

    enviromentUncertainties.insert("humidity",h);

    return enviromentUncertainties;
}

QMap<QString, UncertaintyComponent> SimplePolarMeasurement::getHumanInfluence()
{
    QMap<QString, UncertaintyComponent> humanInfluence;

    //######################### azimuth #########################
    UncertaintyComponent a;

    a.name = "delta_azimuth";
    a.value = 0.0;
    a.uncertainty = 0.0001;
    a.distribution = this->distributions.at(0);
    a.distributions = this->distributions;
    a.errorUnit = "[milliGrad]";

    a.description="influence of the user on the azimuth measurement";

    humanInfluence.insert("delta_azimuth",a);

    //######################### zenith #########################
    UncertaintyComponent z;

    z.name = "delta_zenith";
    z.value = 0.0;
    z.uncertainty = 0.0001;
    z.distribution = this->distributions.at(0);
    z.distributions = this->distributions;
    z.errorUnit = "[milliGrad]";

    z.description="influence of the user on the zenith measurement";

    humanInfluence.insert("delta_zenith",z);

    //######################### distance #########################
    UncertaintyComponent d;

    d.name = "delta_distance";
    d.value = 0.0;
    d.uncertainty = 0.0001;
    d.distribution = this->distributions.at(0);
    d.distributions = this->distributions;
    d.errorUnit = "[mm]";

    d.description="influence of the user on the distance measurement";

    humanInfluence.insert("delta_distance",d);


    return humanInfluence;
}

QMap<QString, int> *SimplePolarMeasurement::getIntegerParameter()
{
        return NULL;
}

QMap<QString, double> *SimplePolarMeasurement::getDoubleParameter()
{
    QMap<QString,double>* doubleParam = new QMap<QString,double>;

    doubleParam->insert("wavelength [micrometer]",0.633);

    return doubleParam;

}

QMap<QString, QStringList> *SimplePolarMeasurement::getStringParameter()
{
    QMap <QString, QStringList>* stringParameter = new QMap<QString, QStringList>;

    QStringList dice;
    QStringList diceDefaultNo;

    dice.append("yes");
    dice.append("no");

    diceDefaultNo.append("no");
    diceDefaultNo.append("yes");

    stringParameter->insert("use sensor errors",dice);
    stringParameter->insert("use environment errors", dice);
    stringParameter->insert("use object errors", diceDefaultNo);
    stringParameter->insert("use human errors", diceDefaultNo);


    return stringParameter;
}

bool SimplePolarMeasurement::analyseSimulationData(UncertaintyData &d)
{


   if(d.values.size() != 0){

       double sumDD = 0.0;

       d.maxValue = d.values.at(0);
       d.minValue = d.values.at(0);

       for(int i = 0; i<d.values.size();i++){
           if(d.values.at(i) > d.maxValue){
               d.maxValue = d.values.at(i) ;
           }
           if(d.values.at(i)  < d.minValue){
               d.minValue = d.values.at(i) ;
           }
           sumDD += d.values.at(i);
       }

       d.expectation = sumDD/d.values.size();


      this->calcUncertainty(d);
      this->checkDistribution(d);

       if(d.distribution.compare("normal")==0){
           d.densityFunction = densityNormal;
           d.distributionFunction = distributionNormal;
       }else if(d.distribution.compare("uniform")==0){
           d.densityFunction = densityUniform;
           d.distributionFunction = distributionUniform;
       }else if(d.distribution.compare("triangular")==0){
           d.densityFunction = densityTriangular;
           d.distributionFunction = distributionTriangular;
       }


   }else{
        return false;
   }



   return true;
}

double SimplePolarMeasurement::getCorrelationCoefficient(QList<double> x, QList<double> y)
{

    if(x.size() != y.size()){
        return 0.0;
    }

    double sumXX = 0.0;
    double sumYY = 0.0;

    for(int i = 0; i<x.size();i++){
        sumXX += x.at(i);
        sumYY += y.at(i);
    }

    double mX = sumXX/x.size();
    double mY = sumYY/y.size();

    double sumXi2 =0.0;
    double sumYi2 =0.0;
    double sumXY = 0.0;

    for(int i = 0; i<x.size();i++){
       sumXi2 += (x.at(i)-mX)*(x.at(i)-mX);
       sumYi2 += (y.at(i)-mY)*(y.at(i)-mY);
       sumXY += (x.at(i)-mX)*(y.at(i)-mY);
    }

    double r = sumXY/(sqrt(sumXi2*sumYi2));

    return r;
}

bool SimplePolarMeasurement::distort(Reading *r,OiMat objectRelation,bool newIterationStart)
{

    newIteration = newIterationStart;

    if(stringParameter->value("use sensor errors").compare("yes")==0){
       distortionBySensor(r);
    }

    if(stringParameter->value("use environment errors").compare("yes")==0){
        distortionByEnviroment(r);
    }

    if(stringParameter->value("use object errors").compare("yes")==0){
        distortionByObject(r,objectRelation);
    }

    if(stringParameter->value("use human errors").compare("yes")==0){
       distortionByHuman(r);
    }

    return true;
}

double SimplePolarMeasurement::distortComponent(UncertaintyComponent u)
{

    if(u.uncertainty==0.0){
        return u.value;
    }

    if(u.distribution.compare("normal")==0){
        std::normal_distribution<double> dist_Normal(u.value,u.uncertainty);
        std::random_device rd;
        std::default_random_engine generator(rd());

        return dist_Normal(generator);

    }else if(u.distribution.compare("uniform")==0){

        double a = u.value-u.uncertainty;
        double b = u.value+u.uncertainty;

        std::uniform_real_distribution<double> dist_Uniform(a,b);
        std::random_device rd;
        std::default_random_engine generator(rd());

        return dist_Uniform(generator);

    }else if (u.distribution.compare("triangular")==0){

        double a = u.value-u.uncertainty;
        double b = u.value+u.uncertainty;
        double c = u.value;

        std::uniform_real_distribution<double> dist_Uniform(0,1);
        std::random_device rd;
        std::default_random_engine generator(rd());

        double U = dist_Uniform(generator);
        double F = (c - a) / (b - a);

           if (U <= F)
              return a + sqrt(U * (b - a) * (c - a));
           else
               return b - sqrt((1 - U) * (b - a) * (b - c));
    }


    return u.value;
}

bool SimplePolarMeasurement::distortionBySensor(Reading *r)
{
    if(r->typeofReading != Configuration::ePolar){
        return false;
    }


    double lambda = distortComponent(givenUncertainties.sensorUncertainties.value("lambda"))/1000.0;
    double mu = distortComponent(givenUncertainties.sensorUncertainties.value("mu"));
    double ex = distortComponent(givenUncertainties.sensorUncertainties.value("ex"))/1000.0;
    double by = distortComponent(givenUncertainties.sensorUncertainties.value("by"))/1000.0;
    double bz = distortComponent(givenUncertainties.sensorUncertainties.value("bz"))/1000.0;
    double alpha = distortComponent(givenUncertainties.sensorUncertainties.value("alpha"))*(M_PI/648000.0);
    double gamma = distortComponent(givenUncertainties.sensorUncertainties.value("gamma"))*(M_PI/648000.0);
    double Aa1 = distortComponent(givenUncertainties.sensorUncertainties.value("Aa1"))*(M_PI/648000.0);
    double Ba1 = distortComponent(givenUncertainties.sensorUncertainties.value("Ba1"))*(M_PI/648000.0);
    double Aa2 = distortComponent(givenUncertainties.sensorUncertainties.value("Aa2"))*(M_PI/648000.0);
    double Ba2 = distortComponent(givenUncertainties.sensorUncertainties.value("Ba2"))*(M_PI/648000.0);
    double Ae0 = distortComponent(givenUncertainties.sensorUncertainties.value("Ae0"))*(M_PI/648000.0);
    double Ae1 = distortComponent(givenUncertainties.sensorUncertainties.value("Ae1"))*(M_PI/648000.0);
    double Be1 = distortComponent(givenUncertainties.sensorUncertainties.value("Be1"))*(M_PI/648000.0);
    double Ae2 = distortComponent(givenUncertainties.sensorUncertainties.value("Ae2"))*(M_PI/648000.0);
    double Be2 = distortComponent(givenUncertainties.sensorUncertainties.value("Be2"))*(M_PI/648000.0);

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

    return true;
}

bool SimplePolarMeasurement::distortionByEnviroment(Reading *r)
{

    if(newIteration){

        double refTemperature = givenUncertainties.enviromentUncertainties.value("temperature").value;
        double temperature = distortComponent(givenUncertainties.enviromentUncertainties.value("temperature"));

        double verticalTempGradient =  distortComponent(givenUncertainties.enviromentUncertainties.value("verticalTemperatureGradient"));
        double horizontalTempGradient =  distortComponent(givenUncertainties.enviromentUncertainties.value("horizontalTemperatureGradient"));
        double verticalPreGradient =  distortComponent(givenUncertainties.enviromentUncertainties.value("verticalPressureGradient"));

        double refPressure = givenUncertainties.enviromentUncertainties.value("pressure").value;
        double pressure = distortComponent(givenUncertainties.enviromentUncertainties.value("pressure"));

        double refHumidity = givenUncertainties.enviromentUncertainties.value("humidity").value;
        double humidity = distortComponent(givenUncertainties.enviromentUncertainties.value("humidity"));

        double wavelength = doubleParameter->value("wavelength [micrometer]");

        refraction = this->edlenRefractionCalculation(refTemperature,refPressure,refHumidity,wavelength);
        distortedRefraction = this->edlenRefractionCalculation(temperature,pressure,humidity,wavelength);

        verticalDn = this->edlenRefractionCalculation(refTemperature+verticalTempGradient,refPressure+verticalPreGradient,refHumidity,wavelength);
        verticalDn = verticalDn-refraction;

        horizontalDn = this->edlenRefractionCalculation(refTemperature+horizontalTempGradient,refPressure,refHumidity,wavelength);
        horizontalDn = horizontalDn-refraction;
   }

    r->rPolar.distance = r->rPolar.distance+((refraction-distortedRefraction)*r->rPolar.distance);

    double refractionZenith = (1/(2*refraction))*verticalDn*r->rPolar.distance;
    double refractionAzimuth = (1/(2*refraction))*horizontalDn*r->rPolar.distance;

    r->rPolar.azimuth = r->rPolar.azimuth+refractionAzimuth;
    r->rPolar.zenith = r->rPolar.zenith+refractionZenith;

    return true;
}

bool SimplePolarMeasurement::distortionByHuman(Reading *r)
{
    double deltaAzimuth = distortComponent(givenUncertainties.humanUncertainties.value("delta_azimuth"))/1000;
    deltaAzimuth = deltaAzimuth*M_PI/180;
    double deltaZenith = distortComponent(givenUncertainties.humanUncertainties.value("delta_zenith"))/1000;
    deltaZenith = deltaZenith*M_PI/180;
    double deltaDistance = distortComponent(givenUncertainties.humanUncertainties.value("delta_distance"))/1000;

    r->rPolar.azimuth = r->rPolar.azimuth + deltaAzimuth;
    r->rPolar.zenith = r->rPolar.zenith + deltaZenith;
    r->rPolar.distance = r->rPolar.distance + deltaDistance;

    return true;
}

bool SimplePolarMeasurement::distortionByObject(Reading *r, OiMat objectRelation)
{
    if(objectRelation.getRowCount() !=4 && objectRelation.getColCount() != 4){
        return false;
    }

    if(newIteration){
        ref_coefficientOfExpansion = givenUncertainties.objectUncertainties.value("coefficientOfExpansion").value;
        coefficientOfExpansion = distortComponent(givenUncertainties.objectUncertainties.value("coefficientOfExpansion"));

        ref_materialTemperature = givenUncertainties.objectUncertainties.value("materialTemperature").value;
        materialTemperature = distortComponent(givenUncertainties.objectUncertainties.value("materialTemperature"));
    }

    double scale = 1+((materialTemperature-ref_materialTemperature)*(coefficientOfExpansion-ref_coefficientOfExpansion)/1000000);

    OiVec xyz = Reading::toCartesian(r->rPolar.azimuth,r->rPolar.zenith,r->rPolar.distance);
    xyz = objectRelation * xyz;
    xyz = xyz * scale;
    OiVec polar = Reading::toPolar(xyz.getAt(0),xyz.getAt(1),xyz.getAt(2));

    r->rPolar.azimuth = polar.getAt(0);
    r->rPolar.zenith = polar.getAt(1);
    r->rPolar.distance = polar.getAt(2);

    return true;
}

void SimplePolarMeasurement::checkDistribution(UncertaintyData &d)
{
    double chi95 = 16.919;
    double chi2Normal = 0.0;
    double chi2Uniform = 0.0;

    QList<double> tmpList = d.values;
    double n = tmpList.size();
    qSort(tmpList);


    double classification = (d.maxValue-d.minValue)/10;
    double startClass = tmpList.first();
    double endClass = tmpList.first()+classification;
    double hi = 0.0;


    for(int i=0;i<n;i++){

        if(tmpList.at(i)>=endClass || i == n-1){

            double startNormal = distributionNormal(startClass,d.expectation,d.uncertainty,d.minValue,d.maxValue);
            double endNormal =distributionNormal(endClass,d.expectation,d.uncertainty,d.minValue,d.maxValue);
            double startUniform= distributionUniform(startClass,d.expectation,d.uncertainty,d.minValue,d.maxValue);
            double endUniform = distributionUniform(endClass,d.expectation,d.uncertainty,d.minValue,d.maxValue);

            chi2Normal += ((hi-n*(endNormal-startNormal))*(hi-n*(endNormal-startNormal)))/(n*(endNormal-startNormal));
            chi2Uniform+= ((hi-n*(endUniform-startUniform))*(hi-n*(endUniform-startUniform)))/(n*(endUniform-startUniform));

            startClass = tmpList.at(i);
            endClass = tmpList.at(i)+classification;
            hi = 1.0;

        }else{
            hi += 1.0;
        }

    }

    if(chi95>chi2Normal){
        d.distribution = "normal";
    }else if(chi95>chi2Uniform){
        d.distribution = "uniform";
    }else{
        d.distribution = "triangular";
    }
}

void SimplePolarMeasurement::calcUncertainty(UncertaintyData &d)
{

        double sumVV = 0.0;


        foreach(double v, d.values){

            sumVV += (d.expectation-v)*(d.expectation-v);

        }

        d.uncertainty = sqrt(sumVV/(d.values.size()-1.0));

}

double SimplePolarMeasurement::edlenRefractionCalculation(double temperature, double pressure, double humidity, double wavelength)
{
    double A1 = -13.928169;
    double A2 = 34.7078238;

    double T = temperature+273.15;
    double Phi = T/273.16;
    double Y = A1*(1- pow(Phi,-15))+A2*(1-pow(Phi,-1.25));

    double Psv = 611.657*exp(Y);
    double pv = humidity/100 * Psv;

    double A = 8342.54;
    double B = 2406147;
    double C = 15998;
    double D = 96095.43;
    double E = 0.601;
    double F = 0.00972;
    double G = 0.003661;

    double S = 1/(wavelength*wavelength);

    double Ns = 1+pow(10,-8)*(A+B/(130-S)+C/(38.9-S));
    double X = (1+pow(10,-8)*(E-F*temperature))/(1+G*temperature);

    double Ntp = 1+pressure*(Ns-1.0)*X/D;

    return Ntp-pow(-10,-10)*((292.75)/(temperature+273.15))*(3.7345-0.0401*S)*pv;
}



