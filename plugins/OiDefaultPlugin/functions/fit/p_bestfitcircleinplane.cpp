#include "p_bestfitcircleinplane.h"

/*!
 * \brief BestFitCircleInPlane::init
 */
void BestFitCircleInPlane::init(){

    //set plugin meta data
    this->metaData.name = "BestFitCircleInPlane";
    this->metaData.pluginName = "OpenIndy Default Plugin";
    this->metaData.author = "bra";
    this->metaData.description = QString("%1 %2 %3")
            .arg("This function calculates an adjusted circle.")
            .arg("The observations are registered into a best fit plane first and afterward a 2D circle is approximated inside the plane.")
            .arg("You can input as many observations as you want which are then used to find the best fit circle.");
    this->metaData.iid = "de.openIndy.plugin.function.fitFunction.v001";

    //set needed elements
    this->neededElements.clear();
    NeededElement param1;
    param1.description = "Select at least three observations to calculate the best fit circle.";
    param1.infinite = true;
    param1.typeOfElement = eObservationElement;
    this->neededElements.append(param1);

    //set spplicable for
    this->applicableFor.append(eCircleFeature);

}

/*!
 * \brief BestFitCircleInPlane::exec
 * \param circle
 * \return
 */
bool BestFitCircleInPlane::exec(Circle &circle){
    return this->setUpResult(circle);
}

/*!
 * \brief BestFitCircleInPlane::setUpResult
 * \param circle
 * \return
 */
bool BestFitCircleInPlane::setUpResult(Circle &circle){

    //get and check input observations
    if(!this->inputElements.contains(0) || this->inputElements[0].size() < 3){
        emit this->sendMessage(QString("Not enough valid observations to fit the circle %1").arg(circle.getFeatureName()));
        return false;
    }
    QList<QPointer<Observation> > inputObservations;
    foreach(const InputElement &element, this->inputElements[0]){
        if(!element.observation.isNull() && element.observation->getIsSolved() && element.observation->getIsValid()){
            inputObservations.append(element.observation);
            this->setUseState(0, element.id, true);
            continue;
        }
        this->setUseState(0, element.id, false);
    }
    if(inputObservations.size() < 3){
        emit this->sendMessage(QString("Not enough valid observations to fit the plane %1").arg(circle.getFeatureName()));
        return false;
    }

    //calculate centroid
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
    OiVec ab = inputObservations.at(1)->getXYZ() - inputObservations.at(0)->getXYZ();
    ab.removeLast();
    OiVec ac = inputObservations.at(2)->getXYZ() - inputObservations.at(0)->getXYZ();
    ac.removeLast();
    OiVec direction(3);
    OiVec::cross(direction, ab, ac);
    direction.normalize();
    double angle = 0.0; //angle between n and direction
    OiVec::dot(angle, n, direction);
    angle = qAbs(qAcos(angle));
    if(angle > (PI/2.0)){
        n = n * -1.0;
    }

    //calculate smallest distance of the plane from the origin
    double dOrigin = n.getAt(0) * centroid.getAt(0) + n.getAt(1) * centroid.getAt(1) + n.getAt(2) * centroid.getAt(2);

    //calculate the distances of each observation from the plane
    /*OiVec planeDistances;
    for(int i = 0; i < inputObservations.size(); i++){

        double distance = qAbs(n.getAt(0) * inputObservations.at(i)->getXYZ().getAt(0)
                               + n.getAt(1) * inputObservations.at(i)->getXYZ().getAt(1)
                               + n.getAt(2) * inputObservations.at(i)->getXYZ().getAt(2) - dOrigin);

        planeDistances.add( distance );

    }*/

    //get transformation matrix
    OiMat t = u.inv();

    //transform centroid into 2D space
    OiVec centroid2D = t * centroid;
    centroid2D.removeLast();

    //calculate centroid reduced coordinates in 2D space
    QList<OiVec> centroidReducedCoordinates;
    OiVec xyz(4);
    for(int i = 0; i < inputObservations.size(); i++){
        xyz = inputObservations.at(i)->getXYZ();
        xyz.removeLast(); //remove the homogeneous item
        xyz = t * xyz;
        xyz.removeLast();
        centroidReducedCoordinates.append(xyz - centroid2D);
        xyz.add(1.0); //add two items so xyz is of size 4 again
        xyz.add(1.0);
    }

    //calculate best fit circle in 2D space
    OiVec A1(inputObservations.size());
    OiMat A2(inputObservations.size(), 3);
    for(int i = 0; i < inputObservations.size(); i++){
        A1.setAt(i, centroidReducedCoordinates.at(i).getAt(0)*centroidReducedCoordinates.at(i).getAt(0)
                 + centroidReducedCoordinates.at(i).getAt(1)*centroidReducedCoordinates.at(i).getAt(1));

        A2.setAt(i,0,centroidReducedCoordinates.at(i).getAt(0));
        A2.setAt(i,1,centroidReducedCoordinates.at(i).getAt(1));
        A2.setAt(i,2,1.0);
    }
    OiVec s(3);
    try{
        if(!OiMat::solve(s, A2.t() * A2, -1.0 * A2.t() * A1)){
            return false;
        }
    }catch(const exception &e){
        qDebug() << "inv error cylinder minimum solution: " << e.what();
        return false;
    }

    //calculate center and radius in 2D space
    OiVec xm(3);
    xm.setAt(0, (-1.0 * s.getAt(0) / 2.0) + centroid2D.getAt(0));
    xm.setAt(1, (-1.0 * s.getAt(1) / 2.0) + centroid2D.getAt(1));
    xm.setAt(2, (t * centroid).getAt(2));
    double radius = qSqrt(0.25 * (s.getAt(0) * s.getAt(0) + s.getAt(1) * s.getAt(1)) - s.getAt(2));

    //calculate distance of each observation from the 2D circle
    OiVec circleDistances;
    OiVec diff(2);
    for(int i = 0; i < centroidReducedCoordinates.size(); i++){

        diff.setAt(0, centroidReducedCoordinates.at(i).getAt(0) + centroid2D.getAt(0) - xm.getAt(0));
        diff.setAt(1, centroidReducedCoordinates.at(i).getAt(1) + centroid2D.getAt(1) - xm.getAt(1));

        double distance = 0.0;
        OiVec::dot(distance, diff, diff);
        distance = qSqrt(distance);
        distance = qAbs(distance - radius);

        circleDistances.add(distance);

    }

    //transform center into 3D space
    xm = t.inv() * xm;

    //set result
    Position circlePosition;
    circlePosition.setVector(xm);
    Radius circleRadius;
    circleRadius.setRadius(radius);
    Direction circleNormal;
    circleNormal.setVector(n);
    circle.setCircle(circlePosition, circleNormal, circleRadius);

    //set statistic
    double stdev = 0.0;
    for(int i = 0; i < centroidReducedCoordinates.size(); i++){
        double v_i = circleDistances.getAt(i);
        stdev += v_i*v_i;
    }
    stdev = qSqrt(stdev / (centroidReducedCoordinates.size() - 3.0));
    this->statistic.setIsValid(true);
    this->statistic.setStdev(stdev);
    circle.setStatistic(this->statistic);

    return true;

}
