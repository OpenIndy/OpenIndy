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
            .arg("This function calculates an adjusted plane.")
            .arg("You can input as many observations as you want which are then used to find the best fit plane.");
    metaData->iid = "de.openIndy.Plugin.Function.FitFunction.v001";
    //...
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
    qDebug() << "valid: " << this->isValid();
    if(this->isValid() && this->checkObservationCount()){
        return this->setUpResult( p );
    }else{
        Console::addLine("Not enough observations available for calculation");
        return false;
    }
}

/*!
 * \brief BestFitPlane::setUpResult
 * Set up result and statistic for type plane
 * \param p
 */
bool BestFitPlane::setUpResult(Plane &plane){
    qDebug() << "vor a mat";
    OiMat a = this->preCalc();
    qDebug() << "vor ata";
    OiMat ata = a.t() * a;
    qDebug() << "nach a mat";
    OiMat u(3,3);
    OiVec d(3);
    OiMat v(3,3);
    qDebug() << "vor svd";
    ata.svd(u, d, v);
    qDebug() << "nach svd";
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
        qDebug() << "eigenindex";
        OiVec n = u.getCol(eigenIndex);
        double sumXN = 0.0;
        double sumYN = 0.0;
        double sumZN = 0.0;
        int count = 0;
        foreach(Observation *obs, this->observations){
            if(obs->isValid){
                sumXN += obs->myXyz.getAt(0) * n.getAt(0);
                sumXN += obs->myXyz.getAt(1) * n.getAt(1);
                sumXN += obs->myXyz.getAt(2) * n.getAt(2);
                count++;
            }
        }
        qDebug() << "vor d";
        double d = (sumXN + sumYN + sumZN) / (double)count;
        qDebug() << "nach d";
        //normal vector (check that plane always points in the same direction)
        n = n.normalize();
        if(this->referenceDirection.getSize() == 0){ //if this function is executed the first time
            this->referenceDirection = n;
        }
        double absR = qSqrt(n.getAt(0) * n.getAt(0) + n.getAt(1) * n.getAt(1) + n.getAt(2) * n.getAt(2));
        double absRef = qSqrt(this->referenceDirection.getAt(0) * this->referenceDirection.getAt(0)
                              + this->referenceDirection.getAt(1) * this->referenceDirection.getAt(1)
                              + this->referenceDirection.getAt(2) * this->referenceDirection.getAt(2));
        double angle = qAcos( OiVec::dot(n, this->referenceDirection) / (absR * absRef) );
        if(angle > PI || angle < -PI){ //switch direction
            n = n * -1.0;
        }
        n.add(1.0);
        plane.ijk = n;
        plane.xyz = d * n;
        qDebug() << "vor stats";
        plane.myStatistic.stdev = qSqrt( eVal / (count - 3) );
        plane.myStatistic.isValid = true;
        this->myStatistic = plane.myStatistic;
        qDebug() << "fertig";
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
    centroid = centroid * (1/n);
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
            this->setUseState(obs->id, true);
            count++;
        }else{
            this->setUseState(obs->id, false);
        }
    }
    if(count >= 3){
        return true;
    }else{
        return false;
    }
}
