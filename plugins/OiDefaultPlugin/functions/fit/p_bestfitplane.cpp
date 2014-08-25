#include "p_bestfitplane.h"

/*!
 * \brief BestFitPlane::getMetaData
 * \return
 */
PluginMetaData* BestFitPlane::getMetaData(){
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
QList<InputParams> BestFitPlane::getNeededElements(){
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
QList<Configuration::FeatureTypes> BestFitPlane::applicableFor(){
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
            if(obs->isValid){
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
        //normal vector (check that plane always points in the same direction)
        n = n.normalize();
        if(this->referenceDirection.getSize() == 0){ //if this function is executed the first time
            this->referenceDirection = n;
        }
        double absR = qSqrt(n.getAt(0) * n.getAt(0) + n.getAt(1) * n.getAt(1) + n.getAt(2) * n.getAt(2));
        double absRef = qSqrt(this->referenceDirection.getAt(0) * this->referenceDirection.getAt(0)
                              + this->referenceDirection.getAt(1) * this->referenceDirection.getAt(1)
                              + this->referenceDirection.getAt(2) * this->referenceDirection.getAt(2));
        double refH;
        OiVec::dot(refH, n, this->referenceDirection);
        double angle = qAcos( refH / (absR * absRef) );
        if(angle > PI || angle < -PI){ //switch direction
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
        if(obs->isValid){
            centroid = centroid + obs->myXyz;
            n++;
        }
    }
    centroid = centroid * (double)(1.0/n);
    vector<OiVec> crCoord;
    foreach(Observation *obs, this->observations){
        if(obs->isValid){
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
        if(obs->isValid){
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
