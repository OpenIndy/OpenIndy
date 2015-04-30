#include "p_bestfitplane.h"

/*!
 * \brief BestFitPlane::getMetaData
 * \return
 */
PluginMetaData* BestFitPlane::getMetaData() const{
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "BestFitPlane";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "br";
    metaData->description = QString("%1 %2")
            .arg("This function calculates an adjusted plane (Drixler).")
            .arg("You can input as many observations as you want which are then used to find the best fit plane.");
    metaData->iid = "de.openIndy.Plugin.Function.FitFunction.v001";
    return metaData;
}

/*!
 * \brief BestFitPlane::getNeededElements
 * \return
 */
QList<InputParams> BestFitPlane::getNeededElements() const{
    QList<InputParams> result;
    InputParams param;
    param.index = 0;
    param.description = "Select at least three observations to calculate the best fit plane.";
    param.infinite = true;
    param.typeOfElement = Configuration::eObservationElement;
    result.append(param);
    return result;
}

/*!
 * \brief BestFitPlane::applicableFor
 * \return
 */
QList<Configuration::FeatureTypes> BestFitPlane::applicableFor() const{
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::ePlaneFeature);
    return result;
}

/*!
 * \brief BestFitPlane::exec
 * \param p
 * \return
 */
bool BestFitPlane::exec(Plane &p){
    if(this->isValid() && this->checkObservationCount()){
        return this->setUpResult( p );
    }else{
        //set statistic to invalid
        Statistic myStats = p.getStatistic();
        myStats.isValid = false;
        p.setStatistic(myStats);
        this->myStatistic = p.getStatistic();
        this->writeToConsole("Not enough observations available for calculation");
        return false;
    }
}

/*!
 * \brief BestFitPlane::setUpResult
 * Set up result and statistic for type plane
 * \param p
 */
bool BestFitPlane::setUpResult(Plane &plane){
    OiMat a = this->preCalc();
    OiMat ata = a.t() * a;
    OiMat u(3,3);
    OiVec d(3);
    OiMat v(3,3);
    ata.svd(u, d, v);
    //get eigenvector which is n vector
    int eigenIndex = -1;
    double eVal = 0.0;
    for(int i = 0; i < d.getSize(); i++){
        if(d.getAt(i) < eVal || i == 0){
            eVal = d.getAt(i);
            eigenIndex = i;
        }
    }
    if(eigenIndex > -1){
        OiVec n(3);
        u.getCol(n, eigenIndex);
        double sumXN = 0.0;
        double sumYN = 0.0;
        double sumZN = 0.0;
        double sumX = 0.0;
        double sumY = 0.0;
        double sumZ = 0.0;
        int count = 0;
        foreach(Observation *obs, this->observations){
            if(obs->getUseState()){
                sumXN += obs->myXyz.getAt(0) * n.getAt(0);
                sumYN += obs->myXyz.getAt(1) * n.getAt(1);
                sumZN += obs->myXyz.getAt(2) * n.getAt(2);

                sumX += obs->myXyz.getAt(0);
                sumY += obs->myXyz.getAt(1);
                sumZ += obs->myXyz.getAt(2);

                count++;
            }
        }
        double d = (sumXN + sumYN + sumZN) / (double)count;
        n = n.normalize();


        //define normal vector of the plane by the first three points
        OiVec ab = this->observations.at(1)->myXyz - this->observations.at(0)->myXyz;
        OiVec ac = this->observations.at(2)->myXyz - this->observations.at(0)->myXyz;
        OiVec ab_3(3);
        ab_3.setAt(0, ab.getAt(0));
        ab_3.setAt(1, ab.getAt(1));
        ab_3.setAt(2, ab.getAt(2));
        OiVec ac_3(3);
        ac_3.setAt(0, ac.getAt(0));
        ac_3.setAt(1, ac.getAt(1));
        ac_3.setAt(2, ac.getAt(2));
        OiVec direction(3);
        OiVec::cross(direction, ab_3, ac_3);
        direction = direction.normalize();
        double angle = 0.0; //angle between n and direction
        OiVec::dot(angle, n, direction);
        angle = qAbs(qAcos(angle));
        if(angle > (PI / 2.0)){
            n = n * -1.0;
        }

        n.add(1.0);
        plane.ijk = n;
        plane.xyz.setAt(0, sumX/(double)count);
        plane.xyz.setAt(1, sumY/(double)count);
        plane.xyz.setAt(2, sumZ/(double)count);
        plane.xyz.setAt(3, 1.0);
        //plane.xyz = d * n;
        Statistic myStats = plane.getStatistic();
        myStats.stdev = qSqrt( eVal / (count - 3) );
        myStats.isValid = true;
        plane.setStatistic(myStats);
        this->myStatistic = plane.getStatistic();
        return true;
    }
    return false;
}

/*!
 * \brief BestFitPlane::preCalc
 * Calculates the A Matrix
 * \param plane
 */
OiMat BestFitPlane::preCalc(){
    //calc centroid reduce coordinates
    OiVec centroid(4);
    int n = 0;
    foreach(Observation *obs, this->observations){
        if(obs->getUseState()){
            centroid = centroid + obs->myXyz;
            n++;
        }
    }
    centroid = centroid * (double)(1.0/n);
    vector<OiVec> crCoord;
    foreach(Observation *obs, this->observations){
        if(obs->getUseState()){
            crCoord.push_back( (obs->myXyz - centroid) );
        }
    }
    //set up A matrix
    OiMat a(crCoord.size(), 3);
    for(int i = 0; i < crCoord.size(); i++){
        a.setAt(i, 0, crCoord.at(i).getAt(0));
        a.setAt(i, 1, crCoord.at(i).getAt(1));
        a.setAt(i, 2, crCoord.at(i).getAt(2));
    }
    return a;
}

/*!
 * \brief BestFitPlane::checkObservationCount
 * Check wether there are enough valid observations for calculation
 * \return
 */
bool BestFitPlane::checkObservationCount(){
    int count = 0;
    foreach(Observation *obs, this->observations){
        if(obs->getUseState()){
            this->setUseState(obs->getId(), true);
            count++;
        }else{
            this->setUseState(obs->getId(), false);
        }
    }
    if(count >= 3){
        return true;
    }else{
        return false;
    }
}
