#include "p_planefrompoints.h"

/*!
 * \brief PlaneFromPoints::init
 */
void PlaneFromPoints::init(){

    //set plugin meta data
    this->metaData.name = "PlaneFromPoints";
    this->metaData.pluginName = "OpenIndy Default Plugin";
    this->metaData.author = "bra";
    this->metaData.description = QString("%1 %2")
            .arg("This function calculates an adjusted plane.")
            .arg("You can input as many points as you want which are then used to find the best fit plane.");
    this->metaData.iid = "de.openIndy.plugin.function.constructFunction.v001";

    //set needed elements
    NeededElement param1;
    param1.description = "Select at least three points to calculate the best fit plane.";
    param1.infinite = true;
    param1.typeOfElement = ePointElement;
    this->neededElements.append(param1);

    //set spplicable for
    this->applicableFor.append(ePlaneFeature);

}

/*!
 * \brief PlaneFromPoints::exec
 * \param plane
 * \return
 */
bool PlaneFromPoints::exec(Plane &plane){
    return this->setUpResult(plane);
}

/*!
 * \brief PlaneFromPoints::setUpResult
 * \param plane
 * \return
 */
bool PlaneFromPoints::setUpResult(Plane &plane){

    //get and check input points
    if(!this->inputElements.contains(0) || this->inputElements[0].size() < 3){
        emit this->sendMessage(QString("Not enough valid points to fit the plane %1").arg(plane.getFeatureName()));
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
    if(inputPoints.size() < 3){
        emit this->sendMessage(QString("Not enough valid points to fit the plane %1").arg(plane.getFeatureName()));
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

    //get smallest eigenvector which is n vector
    int eigenIndex = -1;
    double eVal = 0.0;
    for(int i = 0; i < d.getSize(); i++){
        if(d.getAt(i) < eVal || i == 0){
            eVal = d.getAt(i);
            eigenIndex = i;
        }
    }
    OiVec n(3);
    u.getCol(n, eigenIndex);
    n.normalize();

    //check that the normal vector of the plane is defined by the first three points A, B and C (cross product)
    OiVec ab = inputPoints.at(1)->getPosition().getVector() - inputPoints.at(0)->getPosition().getVector();
    OiVec ac = inputPoints.at(2)->getPosition().getVector() - inputPoints.at(0)->getPosition().getVector();
    OiVec direction(3);
    direction.normalize();
    OiVec::cross(direction, ab, ac);
    double angle = 0.0; //angle between n and direction
    OiVec::dot(angle, n, direction);
    angle = qAbs(qAcos(angle));
    if(angle > (PI/2.0)){
        n = n * -1.0;
    }

    //set result
    Position planePosition;
    planePosition.setVector(centroid);
    Direction planeDirection;
    planeDirection.setVector(n);
    plane.setPlane(planePosition, planeDirection);

    //set statistic
    this->statistic.setIsValid(true);
    this->statistic.setStdev(qSqrt(eVal/(inputPoints.size()-3.0)));
    plane.setStatistic(this->statistic);

    return true;

}
