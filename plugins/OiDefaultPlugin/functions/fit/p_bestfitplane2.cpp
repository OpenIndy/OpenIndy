#include "p_bestfitplane2.h"

/*!
 * \brief BestFitPlane2::getMetaData
 * \return
 */
PluginMetaData* BestFitPlane2::getMetaData(){
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "BestFitPlane2";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "br";
    metaData->description = QString("%1 %2")
            .arg("This function calculates an adjusted plane.")
            .arg("You can input as many observations as you want which are then used to find the best fit plane.");
    metaData->iid = "de.openIndy.Plugin.Function.FitFunction.v001";
    return metaData;
}

QList<InputParams> BestFitPlane2::getNeededElements(){
    QList<InputParams> result;
    InputParams param;
    param.index = 0;
    param.description = "Select at least three observations to calculate the best fit plane.";
    param.infinite = true;
    param.typeOfElement = Configuration::eObservationElement;
    result.append(param);
    return result;
}

QList<Configuration::FeatureTypes> BestFitPlane2::applicableFor(){
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::ePlaneFeature);
    return result;
}

bool BestFitPlane2::exec(Plane &p){
    bool check = false;
    int obsCount = this->getObservationCount();

    //if enough observations available
    if(this->isValid() && obsCount >= 3){

        //fill x,y,z arrays
        double *x = new double[obsCount];
        double *y = new double[obsCount];
        double *z = new double[obsCount];
        int k = 0;
        foreach(Observation *obs, this->observations){
            if(obs->isValid){
                x[k] = obs->myXyz.getAt(0);
                y[k] = obs->myXyz.getAt(1);
                z[k] = obs->myXyz.getAt(2);
                k++;
                this->setUseState(obs->getId(), true);
            }else{
                this->setUseState(obs->getId(), false);
            }
        }

        for(int i = 0; i < 3; i++){
            this->writeToConsole(QString("x: %1").arg(x[i]));
            this->writeToConsole(QString("y: %1").arg(y[i]));
            this->writeToConsole(QString("z: %1").arg(z[i]));
        }

        //adjust
        double d;
        double n[3];
        double qxx[4*4];
        if( fitting_plane(x, y, z, obsCount , n, &d, qxx) ){
            this->setUpResult(p, x, y, z, obsCount, n, d, qxx);
            check = true;
        }else{
            this->writeToConsole("Unknown error while fitting plane");
        }

        //free space
        delete[] x;
        delete[] y;
        delete[] z;
    }else{
        this->writeToConsole("Not enough observations available for calculation");
    }

    return check;
}

/*!
 * \brief BestFitPlane2::setUpResult
 * \param p
 * \param x
 * \param y
 * \param z
 * \param count
 * \param n
 * \param d
 * \param qxx
 */
void BestFitPlane2::setUpResult(Plane &p, double *x, double *y, double *z, int count, double *n, double d, double *qxx){
    //normalize n vector
    double n_l = qSqrt( n[0]*n[0] + n[1]*n[1] + n[2]*n[2] );
    n[0] = n[0]/n_l;
    n[1] = n[1]/n_l;
    n[2] = n[2]/n_l;
    OiVec n0(4);
    n0.setAt(0, n[0]);
    n0.setAt(1, n[1]);
    n0.setAt(2, n[2]);
    n0.setAt(3, 1.0);
    //normal vector (check that line always points in the same direction)
    OiVec n_check(3);
    n_check.setAt(0, n0.getAt(0));
    n_check.setAt(1, n0.getAt(1));
    n_check.setAt(2, n0.getAt(2));
    if(this->referenceDirection.getSize() == 0){ //if this function is executed the first time
        this->referenceDirection = n_check;
    }
    double absR = qSqrt(n_check.getAt(0) * n_check.getAt(0) + n_check.getAt(1) * n_check.getAt(1) + n_check.getAt(2) * n_check.getAt(2));
    double absRef = qSqrt(this->referenceDirection.getAt(0) * this->referenceDirection.getAt(0)
                          + this->referenceDirection.getAt(1) * this->referenceDirection.getAt(1)
                          + this->referenceDirection.getAt(2) * this->referenceDirection.getAt(2));
    double refH;
    OiVec::dot(refH, n_check, this->referenceDirection);
    double angle = qAcos( refH / (absR * absRef) );
    if(angle > PI || angle < -PI){ //switch direction
        n0 = n0 * -1.0;
        n0.setAt(3, 1.0);
    }
    //calc stdev
    double vv = 0.0;
    for(int i = 0; i < count; i++){
        vv += (x[i]*n0.getAt(0) + y[i]*n0.getAt(1) + z[i]*n0.getAt(2) - d)
                   * (x[i]*n0.getAt(0) + y[i]*n0.getAt(1) + z[i]*n0.getAt(2) - d);
    }
    //set result
    p.xyz = d * n0;
    p.ijk = n0;
    Statistic myStats = p.getStatistic();
    myStats.isValid = true;
    myStats.stdev = vv / (count - 1);
    p.setStatistic(myStats);
}

/*!
 * \brief BestFitPlane2::getObservationCount
 * Get number of valid observations
 * \return
 */
int BestFitPlane2::getObservationCount(){
    int count = 0;
    foreach(Observation *obs, this->observations){
        if(obs->isValid){
            count++;
        }
    }
    return count;
}
