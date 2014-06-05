#include "p_planefrompoints.h"

/*!
 * \brief PlaneFromPoints::getMetaData
 * \return
 */
PluginMetaData* PlaneFromPoints::getMetaData(){
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "PlaneFromPoints";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "br";
    metaData->description = QString("%1 %2")
            .arg("This function calculates an adjusted plane.")
            .arg("You can input as many points as you want which are then used to find the best fit plane.");
    metaData->iid = "de.openIndy.Plugin.Function.ConstructFunction.v001";
    return metaData;
}

/*!
 * \brief PlaneFromPoints::getNeededElements
 * \return
 */
QList<InputParams> PlaneFromPoints::getNeededElements(){
    QList<InputParams> result;
    InputParams param;
    param.index = 0;
    param.description = "Select at least three points to calculate the best fit plane.";
    param.infinite = true;
    param.typeOfElement = Configuration::ePointElement;
    result.append(param);
    return result;
}

/*!
 * \brief PlaneFromPoints::applicableFor
 * \return
 */
QList<Configuration::FeatureTypes> PlaneFromPoints::applicableFor(){
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::ePlaneFeature);
    return result;
}

/*!
 * \brief PlaneFromPoints::exec
 * \param p
 * \return
 */
bool PlaneFromPoints::exec(Plane &p){
    if(this->isValid() && this->checkPointCount()){
        return this->setUpResult( p );
    }else{
        this->writeToConsole("Not enough points available for calculation");
        return false;
    }
}

/*!
 * \brief PlaneFromPoints::setUpResult
 * Set up result and statistic for type plane
 * \param plane
 */
bool PlaneFromPoints::setUpResult(Plane &plane){
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
        OiVec n = u.getCol(eigenIndex);
        double sumXN = 0.0;
        double sumYN = 0.0;
        double sumZN = 0.0;
        int count = 0;
        foreach(Point *p, this->points){
            if(p->getIsSolved()){
                sumXN += p->xyz.getAt(0) * n.getAt(0);
                sumXN += p->xyz.getAt(1) * n.getAt(1);
                sumXN += p->xyz.getAt(2) * n.getAt(2);
                count++;
            }
        }
        double d = (sumXN + sumYN + sumZN) / (double)count;
        //normal vector (check that line always points in the same direction)
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
        Statistic myStats;
        myStats.isValid = true;
        myStats.stdev = qSqrt( eVal / (count - 3) );
        plane.setStatistic(myStats);
        return true;
    }
    return false;
}

/*!
 * \brief PlaneFromPoints::preCalc
 * Calculates the A Matrix
 * \param plane
 */
OiMat PlaneFromPoints::preCalc(){
    //calc centroid reduce coordinates
    OiVec centroid(4);
    int n = 0;
    foreach(Point *p, this->points){
        if(p->getIsSolved()){
            centroid = centroid + p->xyz;
            n++;
        }
    }
    centroid = centroid * (1/n);
    vector<OiVec> crCoord;
    foreach(Point *p, this->points){
        if(p->getIsSolved()){
            crCoord.push_back( (p->xyz - centroid) );
        }
    }
    //set up A matrix
    OiMat a(crCoord.size(), 3);
    for(int i = 0; i < n; i++){
        a.setAt(i, 0, crCoord.at(i).getAt(0));
        a.setAt(i, 1, crCoord.at(i).getAt(1));
        a.setAt(i, 2, crCoord.at(i).getAt(2));
    }
    return a;
}

/*!
 * \brief PlaneFromPoints::checkPointCount
 * Check wether there are enough valid points for calculation
 * \return
 */
bool PlaneFromPoints::checkPointCount(){
    int count = 0;
    foreach(Point *p, this->points){
        if(p->getIsSolved()){
            this->setUseState(p->getId(), true);
            count++;
        }else{
            this->setUseState(p->getId(), false);
        }
    }
    if(count >= 3){
        return true;
    }else{
        return false;
    }
}
