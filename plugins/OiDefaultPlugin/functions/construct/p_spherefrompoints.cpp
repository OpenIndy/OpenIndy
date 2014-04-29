#include "p_spherefrompoints.h"

/*!
 * \brief SphereFromPoints::getMetaData
 * \return
 */
PluginMetaData* SphereFromPoints::getMetaData(){
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "SphereFromPoints";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "br";
    metaData->description = QString("%1 %2")
            .arg("This function caclulates an adjusted sphere.")
            .arg("You can input as many points as you want which are then used to find the best fit sphere.");
    metaData->iid = "de.openIndy.Plugin.Function.ConstructFunction.v001";
    return metaData;
}

/*!
 * \brief SphereFromPoints::getNeededElements
 * \return
 */
QList<InputParams> SphereFromPoints::getNeededElements(){
    QList<InputParams> result;
    InputParams param;
    param.index = 0;
    param.description = "Select at least four points to calculate the best fit sphere.";
    param.infinite = true;
    param.typeOfElement = Configuration::ePointElement;
    result.append(param);
    return result;
}

/*!
 * \brief SphereFromPoints::applicableFor
 * \return
 */
QList<Configuration::FeatureTypes> SphereFromPoints::applicableFor(){
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::eSphereFeature);
    return result;
}

/*!
 * \brief SphereFromPoints::exec
 * \param s
 * \return
 */
bool SphereFromPoints::exec(Sphere &s){
    bool check = false;
    int poiCount = this->getPointCount();

    //if enough points available
    if(this->isValid() && poiCount >= 4){

        //fill x,y,z arrays
        double *x = new double[poiCount];
        double *y = new double[poiCount];
        double *z = new double[poiCount];
        int k = 0;
        foreach(Point *p, this->points){
            if(p->isSolved){
                x[k] = p->xyz.getAt(0);
                y[k] = p->xyz.getAt(1);
                z[k] = p->xyz.getAt(2);
                k++;
            }else{
                this->setUseState(p->id, false);
            }
        }

        //adjust
        double r;
        double xm[3];
        double qxx[4*4];
        if( fitting_sphere(x, y, z, poiCount , xm, &r, qxx) ){
            this->setUpResult(s, x, y, z, poiCount, xm, r, qxx);
            check = true;
        }else{
            this->writeToConsole("Unknown error while fitting sphere");
        }

        //free space
        delete[] x;
        delete[] y;
        delete[] z;
    }else{
        this->writeToConsole("Not enough points available for calculation");
    }

    return check;
}

/*!
 * \brief SphereFromPoints::setUpResult
 * \param s
 * \param x
 * \param y
 * \param z
 * \param count
 * \param xm
 * \param r
 * \param qxx
 */
void SphereFromPoints::setUpResult(Sphere &s, double *x, double *y, double *z, int count, double *xm, double r, double *qxx){
    OiVec mp(4);
    mp.setAt(0, xm[0]);
    mp.setAt(1, xm[1]);
    mp.setAt(2, xm[2]);
    mp.setAt(3, 1.0);
    s.xyz = mp;
    s.radius = r;
}

/*!
 * \brief SphereFromPoints::checkPointCount
 * \return
 */
int SphereFromPoints::getPointCount(){
    int count = 0;
    foreach(Point *p, this->points){
        if(p->isSolved){
            this->setUseState(p->id, true);
            count++;
        }else{
            this->setUseState(p->id, false);
        }
    }
    return count;
}
