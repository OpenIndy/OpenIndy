#include "p_bestfitsphere.h"

/*!
 * \brief BestFitSphere::getMetaData
 * \return
 */
PluginMetaData* BestFitSphere::getMetaData(){
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "BestFitSphere";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "br";
    metaData->description = QString("%1 %2")
            .arg("This function calculates an adjusted sphere.")
            .arg("You can input as many observations as you want which are then used to find the best fit sphere.");
    metaData->iid = "de.openIndy.Plugin.Function.FitFunction.v001";
    return metaData;
}

/*!
 * \brief BestFitSphere::getNeededElements
 * \return
 */
QList<InputParams> BestFitSphere::getNeededElements(){
    QList<InputParams> result;
    InputParams param;
    param.index = 0;
    param.description = "Select at least four observations to calculate the best fit sphere.";
    param.infinite = true;
    param.typeOfElement = Configuration::eObservationElement;
    result.append(param);
    return result;
}

/*!
 * \brief BestFitSphere::applicableFor
 * \return
 */
QList<Configuration::FeatureTypes> BestFitSphere::applicableFor(){
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::eSphereFeature);
    return result;
}

/*!
 * \brief BestFitSphere::exec
 * \param s
 * \return
 */
bool BestFitSphere::exec(Sphere &s){
    bool check = false;
    int obsCount = this->getObservationCount();

    //if enough observations available
    if(this->isValid() && obsCount >= 4){

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

        //adjust
        double r;
        double xm[3];
        double qxx[4*4];
        if( fitting_sphere(x, y, z, obsCount , xm, &r, qxx) ){
            this->setUpResult(s, x, y, z, obsCount, xm, r, qxx);
            check = true;
        }else{
            this->writeToConsole("Unknown error while fitting sphere");
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
 * \brief BestFitSphere::setUpResult
 * \param s
 * \param x
 * \param y
 * \param z
 * \param count
 * \param xm
 * \param r
 * \param qxx
 */
void BestFitSphere::setUpResult(Sphere &s, double *x, double *y, double *z, int count, double *xm, double r, double *qxx){
    OiVec mp(4);
    mp.setAt(0, xm[0]);
    mp.setAt(1, xm[1]);
    mp.setAt(2, xm[2]);
    mp.setAt(3, 1.0);
    s.xyz = mp;
    s.radius = r;
}

/*!
 * \brief BestFitSphere::checkObservationCount
 * Get number of valid observations
 * \return
 */
int BestFitSphere::getObservationCount(){
    int count = 0;
    foreach(Observation *obs, this->observations){
        if(obs->isValid){
            count++;
        }
    }
    return count;
}
