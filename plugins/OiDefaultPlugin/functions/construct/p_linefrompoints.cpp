#include "p_linefrompoints.h"

/*!
 * \brief LineFromPoints::getMetaData
 * \return
 */
PluginMetaData* LineFromPoints::getMetaData() const{
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "LineFromPoints";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "br";
    metaData->description = QString("%1 %2")
            .arg("This function calculates an adjusted line.")
            .arg("You can input as many points as you want which are then used to find the best fit 3D line.");
    metaData->iid = "de.openIndy.Plugin.Function.ConstructFunction.v001";
    return metaData;
}

/*!
 * \brief LineFromPoints::getNeededElements
 * \return
 */
QList<InputParams> LineFromPoints::getNeededElements() const{
    QList<InputParams> result;
    InputParams param;
    param.index = 0;
    param.description = "Select at least two points to calculate the best fit line.";
    param.infinite = true;
    param.typeOfElement = Configuration::ePointElement;
    result.append(param);
    return result;
}

/*!
 * \brief LineFromPoints::applicableFor
 * \return
 */
QList<Configuration::FeatureTypes> LineFromPoints::applicableFor() const{
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::eLineFeature);
    return result;
}

/*!
 * \brief LineFromPoints::exec
 * \param l
 * \return
 */
bool LineFromPoints::exec(Line &l){
    if(this->isValid() && this->checkPointCount()){
        return this->setUpResult( l );
    }else{
        this->writeToConsole("Not enough points available for calculation");
        return false;
    }
}

/*!
 * \brief LineFromPoints::setUpResult
 * Set up result and statistic for type plane
 * \param line
 */
bool LineFromPoints::setUpResult(Line &line){
    //centroid
    OiVec centroid(4);
    int n = 0;
    foreach(Point *p, this->points){
        if(p->getIsSolved()){
            centroid = centroid + p->xyz;
            n++;
        }
    }
    centroid = centroid * (1.0/n);
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
        OiVec r(3);
        u.getCol(r, eigenIndex);
        if(this->referenceDirection.getSize() == 0){ //if this function is executed the first time
            this->referenceDirection = r;
        }
        double absR = qSqrt(r.getAt(0) * r.getAt(0) + r.getAt(1) * r.getAt(1) + r.getAt(2) * r.getAt(2));
        double absRef = qSqrt(this->referenceDirection.getAt(0) * this->referenceDirection.getAt(0)
                              + this->referenceDirection.getAt(1) * this->referenceDirection.getAt(1)
                              + this->referenceDirection.getAt(2) * this->referenceDirection.getAt(2));
        double refH;
        OiVec::dot(refH, r, this->referenceDirection);
        double angle = qAcos( refH / (absR * absRef) );
        if(angle > PI || angle < -PI){ //switch direction
            r = r * -1.0;
        }
        r.add(1.0);
        centroid.setAt(3, 1.0);
        //set result
        line.ijk = r;
        line.xyz = centroid;
        Statistic myStats;
        myStats.isValid = true;
        myStats.stdev = qSqrt( vtv / (n - 3) );
        line.setStatistic(myStats);
        return true;
    }
    return false;
}

/*!
 * \brief LineFromPoints::preCalc
 * \param centroid
 * \return
 */
OiMat LineFromPoints::preCalc(OiVec centroid){
    //calc centroid reduced coordinates
    vector<OiVec> crCoord;
    foreach(Point *p, this->points){
        if(p->getIsSolved()){
            crCoord.push_back( (p->xyz - centroid) );
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
 * \brief LineFromPoints::checkPointCount
 * Check wether there are enough valid points for calculation
 * \return
 */
bool LineFromPoints::checkPointCount(){
    int count = 0;
    foreach(Point *p, this->points){
        if(p->getIsSolved()){
            this->setUseState(p->getId(), true);
            count++;
        }else{
            this->setUseState(p->getId(), false);
        }
    }
    if(count >= 2){
        return true;
    }else{
        return false;
    }
}
