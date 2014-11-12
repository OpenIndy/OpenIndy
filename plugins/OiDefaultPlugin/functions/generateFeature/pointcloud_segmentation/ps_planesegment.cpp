#include "ps_planesegment.h"

OiMat PS_PlaneSegment::u = OiMat(3,3);
OiMat PS_PlaneSegment::v = OiMat(3,3);
OiVec PS_PlaneSegment::d = OiVec(3);
OiVec PS_PlaneSegment::n0;
OiMat PS_PlaneSegment::ata(3,3);
//OiVec PS_PlaneSegment::xyz = OiVec(3);

PS_PlaneSegment::PS_PlaneSegment(){

    //create plane states and make sure that myPlaneState points to the same object as myState
    this->myPlaneState = new PlaneState();
    this->myOldState = new PlaneState();
    this->myState = this->myPlaneState;

}

PS_PlaneSegment::~PS_PlaneSegment()
{
}

/*!
 * \brief PS_PlaneSegment::writeToX3D
 * Write an x3d - file with the calculated plane
 * \param filePath
 * \return
 */
bool PS_PlaneSegment::writeToX3D(const QString &filePath){

    return false;

}

/*!
 * \brief PS_PlaneSegment::detectPlane
 * Tries to detect a single plane in the given point material that contains maximum points that satisfy the distance criterion
 * \param points
 * \param param
 * \return
 */
PS_PlaneSegment *PS_PlaneSegment::detectPlane(const QList<PS_Point_PC*> &points, const PS_InputParameter &param){

    PS_PlaneSegment *result = new PS_PlaneSegment();

    int numTrials = 0; //number of necessary trials
    int k = 3; //number of points in a required minimal set to define a plane
    float p = 1.0f - param.outlierPercentage; //probability of drawing a plane-point from the set of points
    float s = 0.99f; //required probability of detecting the plane in numTrials trials

    float phk = p;
    for(int i = 0; i < k-1; i++){
        phk = phk * p;
    }

    //calculate the number of necessary trials
    numTrials = (int)ceil(qLn(1.0f - s) / qLn(1.0f - phk));

    //get numTrials random samples of k points
    QMap<int, QList<PS_Point_PC*> > randomSamples;
    PS_GeneralMath::getRandomSubsets(randomSamples, numTrials, k, points);

    for(int i = 0; i < numTrials; ++i){

        //create plane from k points
        PS_PlaneSegment *possibleSolution = new PS_PlaneSegment();
        possibleSolution->minimumSolution(randomSamples.value(i));

        if(!possibleSolution->getIsValid()){
            continue;
        }

        //if at least 4 points are within the plane...
        if(PS_PlaneSegment::checkPointsInPlane(possibleSolution, points, param, 1) >= k+1){

            //...and the solution is better then the best one found before
            if(possibleSolution->getPoints().size() > result->getPoints().size()){

                //delete the old "best" plane
                if(result != NULL){
                    delete result;
                }

                //set the new plane as result
                result = possibleSolution;

                //if the plane contains mostly all points of the voxel then break the search because no better plane can be found
                if(result->getPoints().size() >= points.size()-3){
                    break;
                }
            }

        }

    }

    return result;

}

/*!
 * \brief PS_PlaneSegment::checkPointsInPlane
 * Check each given point if it lies in a small band around the plane
 * \param myPlane
 * \param myPoints
 * \param param
 * \param toleranceFactor
 * \return
 */
int PS_PlaneSegment::checkPointsInPlane(PS_PlaneSegment *myPlane, const QList<PS_Point_PC*> &myPoints, const PS_InputParameter &param, const int &toleranceFactor){

    int result = 0; //number of points that were added to the plane (that lie in a small band around the plane)

    if(myPlane->getIsValid()){

        float distanceThreshold = (float)toleranceFactor * param.planeParams.maxDistance;

        //iterate through all points to check wether they lie in a small band around the plane
        for(int i = 0; i < myPoints.size(); ++i){

            PS_Point_PC *p = myPoints.at(i);

            //if the point is not used for another shape
            if(!p->isUsed){

                //distance of the point p from the plane
                float distance = p->xyz[0]*myPlane->getIJK()[0] + p->xyz[1]*myPlane->getIJK()[1] + p->xyz[2]*myPlane->getIJK()[2] - myPlane->getDistance();

                //add the point to the plane if the distance is lower than a threshold
                if(qAbs(distance) < distanceThreshold){
                    myPlane->addPoint(p);
                    result++;
                }

            }

        }

    }

    return result;

}

/*!
 * \brief PS_PlaneSegment::fit
 * Fit the plane using all points
 */
void PS_PlaneSegment::fit(){

    if(this->myState->myPoints.size() < 4){
        this->myState->isValid = false;
        return;
    }

    int numPoints = this->getPoints().size();

    //calc centroid of plane sample points
    double centroid[3];
    centroid[0] = 0.0;
    centroid[1] = 0.0;
    centroid[2] = 0.0;
    for(int i = 0; i < numPoints; ++i){
        PS_Point_PC *p = this->getPoints().at(i);
        centroid[0] += p->xyz[0];
        centroid[1] += p->xyz[1];
        centroid[2] += p->xyz[2];
    }
    centroid[0] = centroid[0] / (double)numPoints;
    centroid[1] = centroid[1] / (double)numPoints;
    centroid[2] = centroid[2] / (double)numPoints;

    //centroid reduced coordinates
    /*vector<OiVec> crCoord;
    for(int i = 0; i < numPoints; ++i){
        PS_Point_PC *p = this->getPoints().at(i);
        PS_PlaneSegment::xyz.setAt(0, p->xyz[0] - centroid[0]);
        PS_PlaneSegment::xyz.setAt(1, p->xyz[1] - centroid[1]);
        PS_PlaneSegment::xyz.setAt(2, p->xyz[2] - centroid[2]);
        crCoord.push_back( PS_PlaneSegment::xyz );
    }*/

    //set up A matrix
    /*OiMat a(crCoord.size(), 3);
    for(int i = 0; i < crCoord.size(); i++){
        a.setAt(i, 0, crCoord.at(i).getAt(0));
        a.setAt(i, 1, crCoord.at(i).getAt(1));
        a.setAt(i, 2, crCoord.at(i).getAt(2));
    }

    //principal component analysis
    OiMat ata = a.t() * a;*/

    double value = 0.0;
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            value = 0.0;
            for(int k = 0; k < numPoints; ++k){
                PS_Point_PC *p = this->getPoints().at(k);
                value += (p->xyz[i] - centroid[i]) * (p->xyz[j] - centroid[j]);
            }
            PS_PlaneSegment::ata.setAt(i, j, value);
        }
    }

    PS_PlaneSegment::ata.svd(PS_PlaneSegment::u, PS_PlaneSegment::d, PS_PlaneSegment::v);

    //get eigenvector which is n vector
    int eigenIndex = -1;
    double eVal = 0.0;
    for(int i = 0; i < PS_PlaneSegment::d.getSize(); ++i){
        if(PS_PlaneSegment::d.getAt(i) < eVal || i == 0){
            eVal = PS_PlaneSegment::d.getAt(i);
            eigenIndex = i;
        }
    }
    if(eigenIndex > -1){

        PS_PlaneSegment::u.getCol(PS_PlaneSegment::n0, eigenIndex);
        double sumXN = 0.0;
        double sumYN = 0.0;
        double sumZN = 0.0;
        for(int i = 0; i < numPoints; ++i){
            PS_Point_PC *p = this->getPoints().at(i);
            sumXN += p->xyz[0] * PS_PlaneSegment::n0.getAt(0);
            sumXN += p->xyz[1] * PS_PlaneSegment::n0.getAt(1);
            sumXN += p->xyz[2] * PS_PlaneSegment::n0.getAt(2);
        }
        double d = (sumXN + sumYN + sumZN) / (double)numPoints;
        n0 = n0.normalize();

        //set result
        this->myPlaneState->n0[0] = PS_PlaneSegment::n0.getAt(0);
        this->myPlaneState->n0[1] = PS_PlaneSegment::n0.getAt(1);
        this->myPlaneState->n0[2] = PS_PlaneSegment::n0.getAt(2);
        this->myPlaneState->d = d;
        this->myState->sigma = qSqrt(eVal / ((double)numPoints - 3.0));
        this->myState->mainFocus[0] = centroid[0];
        this->myState->mainFocus[1] = centroid[1];
        this->myState->mainFocus[2] = centroid[2];
        this->myState->isValid = true;

    }else{
        this->myState->isValid = false;
        return;
    }

}

/*!
 * \brief PS_PlaneSegment::fitBySample
 * Fit the plane using a random subset of points to increase performance
 * \param param
 */
void PS_PlaneSegment::fitBySample(int numPoints){

    //number of points on the plane
    const int planePoints = this->myState->myPoints.size();

    if(planePoints < 4 || numPoints < 4){
        this->myState->isValid = false;
        return;
    }

    if(numPoints > planePoints){
        numPoints = planePoints;
    }
    QList<int> mySet;
    for(int i = 0; i < numPoints; ++i){
        mySet.append(i);
    }
    QMap<int, QList<int> > randomSampleMap;
    PS_GeneralMath::getRandomSubsets(randomSampleMap, 1, numPoints, mySet);
    QList<int> &randomSample = randomSampleMap.first();

    if(randomSample.size() == 0){
        this->myState->isValid = false;
        return;
    }

    //calc centroid of plane sample points
    double centroid[3];
    centroid[0] = 0.0;
    centroid[1] = 0.0;
    centroid[2] = 0.0;
    for(int i = 0; i < numPoints; ++i){
        PS_Point_PC *p = this->myState->myPoints.at(randomSample.at(i));
        centroid[0] += p->xyz[0];
        centroid[1] += p->xyz[1];
        centroid[2] += p->xyz[2];
    }
    centroid[0] = centroid[0] / (double)numPoints;
    centroid[1] = centroid[1] / (double)numPoints;
    centroid[2] = centroid[2] / (double)numPoints;

    //centroid reduced coordinates
    /*QList<OiVec> crCoord;
    for(int i = 0; i < numPoints; ++i){
        PS_Point_PC *p = this->myState->myPoints.at(randomSample.at(i));
        PS_PlaneSegment::xyz.setAt(0, p->xyz[0] - centroid[0]);
        PS_PlaneSegment::xyz.setAt(1, p->xyz[1] - centroid[1]);
        PS_PlaneSegment::xyz.setAt(2, p->xyz[2] - centroid[2]);
        crCoord.append( PS_PlaneSegment::xyz );
    }*/

    //set up A matrix
    /*OiMat a(numPoints, 3);
    for(int i = 0; i < crCoord.size(); ++i){
        a.setAt(i, 0, crCoord.at(i).getAt(0));
        a.setAt(i, 1, crCoord.at(i).getAt(1));
        a.setAt(i, 2, crCoord.at(i).getAt(2));
    }

    //principal component analysis
    OiMat ata = a.t() * a;*/

    /*double value = 0.0;
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            value = 0.0;
            for(unsigned int k = 0; k < numPoints; ++k){
                value += crCoord.at(k).getAt(i) * crCoord.at(k).getAt(j);
            }
            PS_PlaneSegment::ata.setAt(i, j, value);
        }
    }*/

    double value = 0.0;
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            value = 0.0;
            for(int k = 0; k < numPoints; ++k){
                PS_Point_PC *p = this->getPoints().at(k);
                value += (p->xyz[i] - centroid[i]) * (p->xyz[j] - centroid[j]);
            }
            PS_PlaneSegment::ata.setAt(i, j, value);
        }
    }

    PS_PlaneSegment::ata.svd(PS_PlaneSegment::u, PS_PlaneSegment::d, PS_PlaneSegment::v);

    //get eigenvector which is n vector
    int eigenIndex = -1;
    float eVal = 0.0;
    for(int i = 0; i < PS_PlaneSegment::d.getSize(); i++){
        if(PS_PlaneSegment::d.getAt(i) < eVal || i == 0){
            eVal = PS_PlaneSegment::d.getAt(i);
            eigenIndex = i;
        }
    }
    if(eigenIndex > -1){

        PS_PlaneSegment::u.getCol(PS_PlaneSegment::n0, eigenIndex);
        double sumXN = 0.0, sumYN = 0.0, sumZN = 0.0;
        for(int i = 0; i < numPoints; ++i){
            PS_Point_PC *p = this->getPoints().at(randomSample.at(i));
            sumXN += p->xyz[0] * PS_PlaneSegment::n0.getAt(0);
            sumXN += p->xyz[1] * PS_PlaneSegment::n0.getAt(1);
            sumXN += p->xyz[2] * PS_PlaneSegment::n0.getAt(2);
        }
        double d = (sumXN + sumYN + sumZN) / (double)numPoints;
        //n0 = n0.normalize();

        double sumVV = 0.0;
        double x = 0.0, y = 0.0, z = 0.0;
        for(int i = 0; i < planePoints; ++i){

            x = this->myState->myPoints.at(i)->xyz[0];
            y = this->myState->myPoints.at(i)->xyz[1];
            z = this->myState->myPoints.at(i)->xyz[2];

            sumVV += (x*PS_PlaneSegment::n0.getAt(0) + y*PS_PlaneSegment::n0.getAt(1) + z*PS_PlaneSegment::n0.getAt(2) - d)
                    * (x*PS_PlaneSegment::n0.getAt(0) + y*PS_PlaneSegment::n0.getAt(1) + z*PS_PlaneSegment::n0.getAt(2) - d);

        }

        //set result
        this->myPlaneState->n0[0] = PS_PlaneSegment::n0.getAt(0);
        this->myPlaneState->n0[1] = PS_PlaneSegment::n0.getAt(1);
        this->myPlaneState->n0[2] = PS_PlaneSegment::n0.getAt(2);
        this->myPlaneState->d = d;
        //this->myState->sigma = qSqrt(sumVV / ((double)planePoints - 3.0));
        this->myState->sigma = qSqrt(eVal / ((double)numPoints - 3.0));
        this->myState->mainFocus[0] = centroid[0];
        this->myState->mainFocus[1] = centroid[1];
        this->myState->mainFocus[2] = centroid[2];
        this->myState->isValid = true;

    }else{
        this->myState->isValid = false;
        return;
    }

}

/*!
 * \brief PS_PlaneSegment::minimumSolution
 * Calculate plane parameters from 3 points A, B and C
 * \param points
 */
void PS_PlaneSegment::minimumSolution(const QList<PS_Point_PC *> &points){

    if(points.size() != 3){
        this->myState->isValid = false;
        return;
    }

    //calculate vectors AB and AC
    float ab[3];
    float ac[3];
    for(int j = 0; j < 3; j++){
        ab[j] = points.at(0)->xyz[j] - points.at(1)->xyz[j];
        ac[j] = points.at(0)->xyz[j] - points.at(2)->xyz[j];
    }

    //by cross product calculate normal vector of the plane
    float n[3];
    n[0] = ab[1]*ac[2] - ab[2]*ac[1];
    n[1] = ab[2]*ac[0] - ab[0]*ac[2];
    n[2] = ab[0]*ac[1] - ab[1]*ac[0];

    //dividing by the length of the normal vector results in the unit normal vector
    float laenge_n = qSqrt( n[0]*n[0] + n[1]*n[1] + n[2]*n[2] );
    n[0] = n[0] / laenge_n;
    n[1] = n[1] / laenge_n;
    n[2] = n[2] / laenge_n;

    //smallest distance d of the plane from the origin
    float d = points.at(0)->xyz[0] * n[0]
            + points.at(0)->xyz[1] * n[1]
            + points.at(0)->xyz[2] * n[2];

    //set the plane's attributes to the calculated values
    this->myPlaneState->d = d;
    this->myPlaneState->n0[0] = n[0];
    this->myPlaneState->n0[1] = n[1];
    this->myPlaneState->n0[2] = n[2];
    this->myState->isValid = true;

}

/*!
 * \brief PS_PlaneSegment::sortOut
 * Only include points in the plane that are within a distance around the plane (specified by the user)
 * \param myPlane
 * \param param
 * \param toleranceFactor
 */
void PS_PlaneSegment::sortOut(PS_PlaneSegment *myPlane, const PS_InputParameter &param, const int &toleranceFactor){

    QList<PS_Point_PC *> myPoints = myPlane->getPoints();
    myPlane->removeAllPoints();
    PS_PlaneSegment::checkPointsInPlane(myPlane, myPoints, param, toleranceFactor);

}

/*!
 * \brief PS_PlaneSegment::mergePlanes
 * Merge detected planes which seem to be the same one
 * \param detectedPlanes
 * \param mergedPlanes
 * \param param
 */
void PS_PlaneSegment::mergePlanes(const QList<PS_PlaneSegment *> &detectedPlanes, QList<PS_PlaneSegment *> &mergedPlanes, const PS_InputParameter &param){

    float nAngle = 0.0f; //angle between the normal vectors of the two planes
    float dCP1P2 = 0.0f; //distance of centroid of plane 1 to plane 2
    float dCP2P1 = 0.0f; //distance of centroid of plane 2 to plane 1

    if(mergedPlanes.size() != 0){
        mergedPlanes.clear();
    }

    //for each plane save its merged state
    QMap<int, bool> mergeMap;
    for(int i = 0; i < detectedPlanes.size(); ++i){
        mergeMap.insert(i, false);
    }

    //run through all detected planes
    for(int i = 0; i < detectedPlanes.size(); ++i){

        //if the plane at position i was merged before
        if(mergeMap.value(i) == true){
            continue;
        }
        mergeMap.insert(i, true); //set as merged

        PS_PlaneSegment *p1 = detectedPlanes.at(i); //get the first plane

        //compare all plane's params and merge them if they fit together
        for(int j = i+1; j < detectedPlanes.size(); ++j){

            //if the plane at position j was merged before
            if(mergeMap.value(j) == true){
                continue;
            }

            PS_PlaneSegment *p2 = detectedPlanes.at(j); //get the second plane

            //calculate the angle between the normal vectors of plane 1 and 2
            nAngle = qAcos( p1->getIJK()[0] * p2->getIJK()[0]
                    + p1->getIJK()[1] * p2->getIJK()[1]
                    + p1->getIJK()[2] * p2->getIJK()[2] );
            nAngle = qAbs( fmod(nAngle, PS_PI) );
            if(qAbs(nAngle - PS_PI) < nAngle){ nAngle = qAbs(nAngle - PS_PI); }

            dCP2P1 = p2->getMainFocus()[0]*p1->getIJK()[0] + p2->getMainFocus()[1]*p1->getIJK()[1] + p2->getMainFocus()[2]*p1->getIJK()[2] - p1->getDistance();
            dCP1P2 = p1->getMainFocus()[0]*p2->getIJK()[0] + p1->getMainFocus()[1]*p2->getIJK()[1] + p1->getMainFocus()[2]*p2->getIJK()[2] - p2->getDistance();

            //if the parameter-differences are under a threshold
            if(dCP2P1 < 10.0*param.planeParams.maxDistance && dCP1P2 < 10.0*param.planeParams.maxDistance){
                    //&& nAngle < 0.0873){

                PS_PlaneSegment *mergedPlane = new PS_PlaneSegment();
                mergedPlane->setIsValid(true);
                foreach(PS_Point_PC *myPoint, p1->getPoints()){
                    myPoint->isUsed = false;
                    mergedPlane->addPoint(myPoint);
                }
                foreach(PS_Point_PC *myPoint, p2->getPoints()){
                    myPoint->isUsed = false;
                    mergedPlane->addPoint(myPoint);
                }

                //fit plane and sort out points that do not satisfy the distance criterion
                //mergedPlane->fitBySample(param.fitSampleSize * 10);
                //mergedPlane->fitBySample(param.fitSampleSize * 10);
                mergedPlane->fit();
                PS_PlaneSegment::sortOut(mergedPlane, param, 3);
                //mergedPlane->fitBySample(param.fitSampleSize);

                //qDebug() << "p1 " << p1->getPointCount();
                //qDebug()<< "p2" << p2->getPointCount();
                //qDebug() << "merged " << mergedPlane->getPointCount();

                //if the merged plane's standard deviation is ok
                if(mergedPlane->getIsValid() //&& mergedPlane->getSigma() <= param.planeParams.maxDistance
                        && mergedPlane->getPointCount() > p1->getPointCount()
                        && mergedPlane->getPointCount() > p2->getPointCount()){

                    //qDebug() << "deleted: " << p1->getPointCount()+p2->getPointCount()-mergedPlane->getPointCount();

                    //delete single planes
                    delete p1;
                    delete p2;

                    //set the planes to be merged
                    mergeMap.insert(j, true);

                    p1 = mergedPlane;

                }else{
                    //qDebug() << "reject";
                    /*qDebug() << mergedPlane->getIsValid();
                    qDebug() << mergedPlane->getSigma();
                    qDebug() << mergedPlane->getPointCount();*/
                    delete mergedPlane;
                }

            }

            //qDebug() << "angle: " << nAngle;

        }

        //add the plane to result list
        mergedPlanes.append(p1);

    }

}

/*!
 * \brief PS_PlaneSegment::reviewNodes
 * Review all points of used nodes and try to add them to the plane
 * \param detectedPlanes
 */
void PS_PlaneSegment::reviewNodes(const QList<PS_PlaneSegment *> &detectedPlanes, const PS_InputParameter &param){

    bool pointsAdded = false;
    int numAdded = 0; //number of added points

    //list to save all unmerged points of each node
    QList<PS_Point_PC *> unmergedPoints;

    foreach(PS_PlaneSegment *p, detectedPlanes){

        //consider each node from which points were used for the plane
        foreach(PS_Node *n, p->getUsedNodes()){

            //try to add points that were not added yet
            n->getUnmergedPoints(unmergedPoints);
            numAdded = PS_PlaneSegment::checkPointsInPlane(p, unmergedPoints, param, 3);
            QList<PS_Point_PC *> planePoints = p->getPoints();
            for(int i = 0; i < numAdded; ++i){
                planePoints.at(planePoints.size()-1-i)->isUsed = true;
            }

            if(numAdded > 0){
                pointsAdded = true;
            }

            //set node as merged
            n->setWasConsideredInMerge(true);

        }

        if(pointsAdded){
            //refit the plane
            //p->fitBySample(param.fitSampleSize);
        }

        //set all nodes as unconsidered in merge
        foreach(PS_Node *n, p->getUsedNodes()){
            n->setWasConsideredInMerge(false);
        }

    }

}
