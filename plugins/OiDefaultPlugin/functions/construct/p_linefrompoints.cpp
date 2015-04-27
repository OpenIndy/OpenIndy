#include "p_linefrompoints.h"

/*!
 * \brief LineFromPoints::init
 */
void LineFromPoints::init(){

    //set plugin meta data
    this->metaData.name = "LineFromPoints";
    this->metaData.pluginName = "OpenIndy Default Plugin";
    this->metaData.author = "bra";
    this->metaData.description = QString("%1 %2")
            .arg("This function calculates an adjusted line.")
            .arg("You can input as many points as you want which are then used to find the best fit 3D line.");
    this->metaData.iid = "de.openIndy.plugin.function.constructFunction.v001";

    //set needed elements
    NeededElement param1;
    param1.description = "Select at least two points to calculate the best fit line.";
    param1.infinite = true;
    param1.typeOfElement = ePointElement;
    this->neededElements.append(param1);

    //set spplicable for
    this->applicableFor.append(eLineFeature);

}

/*!
 * \brief LineFromPoints::exec
 * \param line
 * \return
 */
bool LineFromPoints::exec(Line &line){
    return this->setUpResult(line);
}

/*!
 * \brief LineFromPoints::setUpResult
 * \param line
 * \return
 */
bool LineFromPoints::setUpResult(Line &line){

    //get and check input points
    if(!this->inputElements.contains(0) || this->inputElements[0].size() < 2){
        emit this->sendMessage(QString("Not enough valid points to fit the line %1").arg(line.getFeatureName()));
        return false;
    }
    QList<QPointer<Point> > inputPoints;
    foreach(const InputElement &element, this->inputElements[0]){
        if(!element.point.isNull() && element.point->getIsSolved()){
            inputPoints.append(element.point);
            this->setUseState(0, element.id, true);
        }
        this->setUseState(0, element.id, false);
    }
    if(inputPoints.size() < 2){
        emit this->sendMessage(QString("Not enough valid points to fit the line %1").arg(line.getFeatureName()));
        return false;
    }

    //centroid
    OiVec centroid(3);
    foreach(const QPointer<Point> &point, inputPoints){
        centroid = centroid + point->getPosition().getVector();
    }
    centroid = centroid * (1.0/inputPoints.size());

    //principle component analysis
    OiMat a(inputPoints.size(), 3);
    for(int i = 0; i < inputPoints.size(); i++){
        a.setAt(i, 0, inputPoints.at(i)->getPosition().getVector().getAt(0) - centroid.getAt(0));
        a.setAt(i, 1, inputPoints.at(i)->getPosition().getVector().getAt(1) - centroid.getAt(1));
        a.setAt(i, 2, inputPoints.at(i)->getPosition().getVector().getAt(2) - centroid.getAt(2));
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

    //check that the orientation of the line is from first to second point
    OiVec pos1 = inputPoints.at(0)->getPosition().getVector();
    OiVec pos2 = inputPoints.at(1)->getPosition().getVector();
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
    this->statistic.setStdev(qSqrt(vtv/(inputPoints.size()-2.0)));
    line.setStatistic(this->statistic);

    return true;

}
