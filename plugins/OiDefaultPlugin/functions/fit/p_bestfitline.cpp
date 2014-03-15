#include "p_bestfitline.h"

/*!
 * \brief BestFitLine::getMetaData
 * \return
 */
PluginMetaData* BestFitLine::getMetaData(){
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "BestFitLine";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "br";
    metaData->description = QString("%1 %2")
            .arg("This function calculates an adjusted line (Drixler).")
            .arg("You can input as many observations as you want which are then used to find the best fit 3D line.");
    metaData->iid = "de.openIndy.Plugin.Function.FitFunction.v001";
    return metaData;
}

/*!
 * \brief BestFitLine::getNeededElements
 * \return
 */
QList<InputParams> BestFitLine::getNeededElements(){
    QList<InputParams> result;
    InputParams param;
    param.index = 0;
    param.description = "Select at least two observations to calculate the best fit line.";
    param.infinite = true;
    param.typeOfElement = Configuration::eObservationElement;
    result.append(param);
    return result;
}

/*!
 * \brief BestFitLine::applicableFor
 * \return
 */
QList<Configuration::FeatureTypes> BestFitLine::applicableFor(){
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::eLineFeature);
    return result;
}

/*!
 * \brief BestFitLine::exec
 * \param p
 * \return
 */
bool BestFitLine::exec(Line &l){
    if(this->isValid() && this->checkObservationCount()){
        return this->setUpResult( l );
    }else{
        this->writeToConsole("Not enough observations available for calculation");
        return false;
    }
}

/*!
 * \brief BestFitLine::setUpResult
 * Set up result and statistic for type plane
 * \param p
 */
bool BestFitLine::setUpResult(Line &line){
    //centroid
    OiVec centroid(4);
    int n = 0;
    foreach(Observation *obs, this->observations){
        if(obs->isValid){
            centroid = centroid + obs->myXyz;
            n++;
        }
    }
    centroid = centroid * (double)(1.0/n);
    //fill a matrix
    OiMat a = this->preCalc(centroid);
    OiMat ata = a.t() * a;
    //perform svd
    OiMat u(3,3);
    OiVec d(3);
    OiMat v(3,3);
    ata.svd(u, d, v);
    //get largest eigenvector which is r vector and v^T * v as sum of the 2 smaller eigenvectors
    int eigenIndex = -1;
    double eVal = 0.0;
    double vtv = 0.0;
    for(int i = 0; i < d.getSize(); i++){
        if(d.getAt(i) > eVal || i == 0){
            eVal = d.getAt(i);
            eigenIndex = i;
        }else{
            vtv += d.getAt(i);
        }
    }
    if(eigenIndex > -1){
        //vector of direction (check that line always points in the same direction)
        OiVec r = u.getCol(eigenIndex);
        if(this->referenceDirection.getSize() == 0){ //if this function is executed the first time
            this->referenceDirection = r;
        }
        double absR = qSqrt(r.getAt(0) * r.getAt(0) + r.getAt(1) * r.getAt(1) + r.getAt(2) * r.getAt(2));
        double absRef = qSqrt(this->referenceDirection.getAt(0) * this->referenceDirection.getAt(0)
                              + this->referenceDirection.getAt(1) * this->referenceDirection.getAt(1)
                              + this->referenceDirection.getAt(2) * this->referenceDirection.getAt(2));
        double angle = qAcos( OiVec::dot(r, this->referenceDirection) / (absR * absRef) );
        if(angle > PI || angle < -PI){ //switch direction
            r = r * -1.0;
        }
        r.add(1.0);
        centroid.setAt(3, 1.0);
        //calculate qxx and sll matrix
        OiMat qxx = (1 / d.getAt(eigenIndex)) * r.t().t() * r.t();
        OiMat sll = (vtv / (n - 3)) * (vtv / (n - 3)) * qxx;
        //set result
        line.ijk = r;
        line.xyz = centroid;
        line.myStatistic.stdev = qSqrt( vtv / (n - 3) );
        line.myStatistic.s0_apriori = 1.0;
        line.myStatistic.s0_aposteriori = line.myStatistic.stdev;
        line.myStatistic.qxx = qxx;
        line.myStatistic.isValid = true;
        this->myStatistic = line.myStatistic;
        return true;
    }
    return false;
}

/*!
 * \brief BestFitLine::preCalc
 * Calculates the A Matrix
 * \param plane
 */
OiMat BestFitLine::preCalc(OiVec centroid){
    //calc centroid reduce coordinates
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
 * \brief BestFitLine::checkObservationCount
 * Check wether there are enough valid observations for calculation
 * \return
 */
bool BestFitLine::checkObservationCount(){
    int count = 0;
    foreach(Observation *obs, this->observations){
        if(obs->isValid){
            this->setUseState(obs->id, true);
            count++;
        }else{
            this->setUseState(obs->id, false);
        }
    }
    if(count >= 2){
        return true;
    }else{
        return false;
    }
}
