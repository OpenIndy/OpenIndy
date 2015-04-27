#include "p_bestfitline.h"

void BestFitLine::init(){

    //set plugin meta data
    this->metaData.name = "BestFitLine";
    this->metaData.pluginName = "OpenIndy Default Plugin";
    this->metaData.author = "bra";
    this->metaData.description = QString("%1 %2")
            .arg("This function calculates an adjusted line.")
            .arg("You can input as many observations as you want which are then used to find the best fit 3D line.");
    this->metaData.iid = "de.openIndy.plugin.function.fitFunction.v001";

    //set needed elements
    NeededElement param1;
    param1.description = "Select at least two observations to calculate the best fit line.";
    param1.infinite = true;
    param1.typeOfElement = eObservationElement;
    this->neededElements.append(param1);

    //set spplicable for
    this->applicableFor.append(eLineFeature);

}

/*!
 * \brief BestFitLine::exec
 * \param line
 * \return
 */
bool BestFitLine::exec(Line &line){
    return this->setUpResult(line);
}

/*!
 * \brief BestFitLine::setUpResult
 * Set up result and statistic for type plane
 * \param p
 */
bool BestFitLine::setUpResult(Line &line){

    //get and check input observations
    if(!this->inputElements.contains(0) || this->inputElements[0].size() < 2){
        emit this->sendMessage(QString("Not enough valid observations to fit the line %1").arg(line.getFeatureName()));
        return false;
    }
    QList<QPointer<Observation> > inputObservations;
    foreach(const InputElement &element, this->inputElements[0]){
        if(!element.observation.isNull() && element.observation->getIsSolved() && element.observation->getIsValid()){
            inputObservations.append(element.observation);
            this->setUseState(0, element.id, true);
        }
        this->setUseState(0, element.id, false);
    }
    if(inputObservations.size() < 2){
        emit this->sendMessage(QString("Not enough valid observations to fit the line %1").arg(line.getFeatureName()));
        return false;
    }

    //centroid
    OiVec centroid(4);
    foreach(const QPointer<Observation> &obs, inputObservations){
        centroid = centroid + obs->getXYZ();
    }
    centroid = centroid * (1.0/inputObservations.size());
    centroid.removeLast();

    //principle component analysis
    OiMat a(inputObservations.size(), 3);
    for(int i = 0; i < inputObservations.size(); i++){
        a.setAt(i, 0, inputObservations.at(i)->getXYZ().getAt(0) - centroid.getAt(0));
        a.setAt(i, 1, inputObservations.at(i)->getXYZ().getAt(1) - centroid.getAt(1));
        a.setAt(i, 2, inputObservations.at(i)->getXYZ().getAt(2) - centroid.getAt(2));
    }
    OiMat ata = a.t() * a;
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
    OiVec r(3);
    u.getCol(r, eigenIndex);
    r.normalize();

    //check that the orientation of the line is from first to second observation
    OiVec pos1 = inputObservations.at(0)->getXYZ();
    pos1.removeLast();
    OiVec pos2 = inputObservations.at(1)->getXYZ();
    pos2.removeLast();
    OiVec direction = pos2 - pos1;
    direction.normalize();
    double angle = 0.0; //angle between r and direction
    OiVec::dot(angle, r, direction);
    angle = qAbs(qAcos(angle));
    if(angle > (PI/2.0)){
        r = r * -1.0;
    }

    //set result
    Position linePosition;
    linePosition.setVector(centroid);
    Direction lineDirection;
    lineDirection.setVector(r);
    line.setLine(linePosition, lineDirection);

    //set statistic
    this->statistic.setIsValid(true);
    this->statistic.setStdev(qSqrt(vtv/(inputObservations.size()-2.0)));
    line.setStatistic(this->statistic);

    return true;

}
