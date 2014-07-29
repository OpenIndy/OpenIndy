#include "ps_spheresegment.h"

//CombinationWatcher Sphere::myCombinations;
OiMat PS_SphereSegment::min_A(4,4);
OiMat PS_SphereSegment::min_AReplaced(4,4);
OiVec PS_SphereSegment::min_C(4);
OiVec PS_SphereSegment::min_X(4);

OiMat PS_SphereSegment::fitting_N(4,4);
OiVec PS_SphereSegment::fitting_n(4);
OiMat PS_SphereSegment::fitting_Q(4,4);
OiVec PS_SphereSegment::fitting_a(4);
OiVec PS_SphereSegment::fitting_x(3);

OiMat PS_SphereSegment::verify_u(3,3);
OiMat PS_SphereSegment::verify_v(3,3);
OiVec PS_SphereSegment::verify_d(3);
OiVec PS_SphereSegment::verify_xyz(3);

PS_SphereSegment::PS_SphereSegment() : PS_ShapeSegment(){

    //create sphere states and make sure that mySphereState points to the same object as myState
    this->mySphereState = new SphereState();
    this->myOldState = new SphereState();
    this->myState = this->mySphereState;

}

PS_SphereSegment::~PS_SphereSegment()
{
}

/*!
 * \brief PS_SphereSegment::detectSphere
 * Tries to detect a single sphere in the given point material that contains maximum points that satisfy the distance criterion
 * \param points
 * \param param
 * \return
 */
PS_SphereSegment *PS_SphereSegment::detectSphere(QList<Point_PC *> points, PS_InputParameter param){

    PS_SphereSegment *result = new PS_SphereSegment();

    //qDebug() << "sample size: " << points.size();

    int numTrials = 0; //number of necessary trials
    int k = 4; //number of points in a required minimal set to define a sphere
    float p = 1.0f - param.outlierPercentage; //probability of drawing a sphere-point from the set of points
    float s = 0.99f; //required probability of detecting the sphere in numTrials trials

    float phk = p;
    for(int i = 0; i < k-1; i++){
        phk = phk * p;
    }

    //calculate the number of necessary trials
    numTrials = qLn(1.0f - s) / qLn(1.0f - phk);

    //qDebug() << "Anzahl Durchgänge: " << numTrials;

    //get numTrials random samples
    QMap<int, QList<Point_PC*> > randomSamples;
    randomSamples = PS_GeneralMath::getRandomSubsets(numTrials, k, points);

    for(int i = 0; i < numTrials; i++){

        //qDebug() << "sample number " << i;

        //create sphere out of the four points A, B, C and D
        PS_SphereSegment *possibleSolution = new PS_SphereSegment();
        possibleSolution->minimumSolution(randomSamples.value(i));

        //if the radius is too large/low or there is no solution then break and consider the next random sample
        if(!possibleSolution->getIsValid() || possibleSolution->getRadius() > param.sphereParams.maxRadius
                || possibleSolution->getRadius() < param.sphereParams.minRadius){
            continue;
        }

        //if at least k+1 points are within the sphere...
        if(PS_SphereSegment::checkPointsInSphere(possibleSolution, points, param, 3) >= k+1){

            //...and the solution is better then the best one found before
            if(possibleSolution->getPoints().size() > result->getPoints().size()){

                //delete the old "best" sphere
                if(result != NULL){
                    delete result;
                }

                //set the new sphere as result
                result = possibleSolution;

                //if the sphere contains all points of the voxel then break the search
                if(result->myState->myPoints.size() == points.size()){
                    break;
                }
            }

        }

    }

    return result;

}

/*!
 * \brief PS_SphereSegment::checkPointsInSphere
 * Returns the number of points that were added to the sphere
 * toleranceFactor is a factor that defines wether a point is in a sphere
 * \param mySphere
 * \param myPoints
 * \param param
 * \param toleranceFactor
 * \return
 */
int PS_SphereSegment::checkPointsInSphere(PS_SphereSegment *mySphere, QList<Point_PC *> myPoints, PS_InputParameter param, int toleranceFactor){

    int result = 0; //number of points that were added to the sphere

    if(mySphere->getIsValid()){

        //iterate through all points to check wether they lie in a small band around the sphere surface
        for(int i = 0; i < myPoints.size(); i++){

            Point_PC *p = myPoints.at(i);

            //if the point is not used for another shape
            if(!p->isUsed){

                float diffVec[3]; //difference vector between point and sphere center
                diffVec[0] = p->xyz[0] - mySphere->getXYZ()[0];
                diffVec[1] = p->xyz[1] - mySphere->getXYZ()[1];
                diffVec[2] = p->xyz[2] - mySphere->getXYZ()[2];

                float radiusActual = 0.0f; //length of the difference vector

                radiusActual = qSqrt( diffVec[0]*diffVec[0] + diffVec[1]*diffVec[1] + diffVec[2]*diffVec[2] );

                float distance = 0.0f;

                distance = radiusActual - mySphere->getRadius(); //distance error

                //add the point to the sphere if the distance is lower than a threshold
                float absDist = qAbs(distance);
                if(absDist < (((float)toleranceFactor) * param.sphereParams.maxDistance)){
                    mySphere->addPoint(p);
                    result++;
                }

            }

        }

    }

    return result;

}

/*!
 * \brief PS_SphereSegment::fit
 * Fit the sphere using all points
 */
void PS_SphereSegment::fit(){

    if(this->myState->myPoints.size() < 5){
        this->myState->isValid = false;
        return;
    }

    int numPoints = this->getPoints().size();

    double centroid[3]; //centroids of all points
    centroid[0] = 0.0;
    centroid[1] = 0.0;
    centroid[2] = 0.0;

    double x = 0.0, y = 0.0, z = 0.0;
    double r = 0.0, xm = 0.0, ym = 0.0, zm = 0.0;

    int numIterations = 0;

    //calc centroid of all sphere points
    for(int i = 0; i < this->getPoints().size(); i++){
        centroid[0] += this->myState->myPoints.at(i)->xyz[0];
        centroid[1] += this->myState->myPoints.at(i)->xyz[1];
        centroid[2] += this->myState->myPoints.at(i)->xyz[2];
    }
    centroid[0] = centroid[0] / (double)numPoints;
    centroid[1] = centroid[1] / (double)numPoints;
    centroid[2] = centroid[2] / (double)numPoints;

    //set approximated values
    xm = this->getXYZ()[0];
    ym = this->getXYZ()[1];
    zm = this->getXYZ()[2];
    r = this->getRadius();

    OiVec xd(4);

    OiVec verb(numPoints*3);
    OiMat A(numPoints, 4);
    OiMat B(numPoints, numPoints*3);
    OiVec w(numPoints+4);
    OiMat N(numPoints+4, numPoints+4);

    OiMat Q(numPoints+4, numPoints+4);
    OiMat BBT;
    OiMat AT;
    OiVec res(numPoints+4);
    OiVec k(numPoints);
    OiVec vd;

    do{

        for(int i = 0; i < numPoints; i++){

            x = this->getPoints().at(i)->xyz[0];
            y = this->getPoints().at(i)->xyz[1];
            z = this->getPoints().at(i)->xyz[2];

            double r0 = qSqrt( (x + verb.getAt(i*3) - xm) * (x + verb.getAt(i*3) - xm)
                               + (y + verb.getAt(i*3+1) - ym) * (y + verb.getAt(i*3+1) - ym)
                               + (z + verb.getAt(i*3+2) - zm) * (z + verb.getAt(i*3+2) - zm) );

            A.setAt(i, 0, -1.0 * (x + verb.getAt(i*3) - xm) / r0);
            A.setAt(i, 1, -1.0 * (y + verb.getAt(i*3+1) - ym) / r0);
            A.setAt(i, 2, -1.0 * (z + verb.getAt(i*3+2) - zm) / r0);
            A.setAt(i, 3, -1.0);

            B.setAt(i, i*3, (x + verb.getAt(i*3) - xm) / r0);
            B.setAt(i, i*3+1, (y + verb.getAt(i*3+1) - ym) / r0);
            B.setAt(i, i*3+2, (z + verb.getAt(i*3+2) - zm) / r0);

            w.setAt(i, r0 - r);

        }

        BBT = B * B.t();
        AT = A.t();

        for(unsigned int i = 0; i < BBT.getRowCount(); i++){
            for(unsigned int j = 0; j < BBT.getColCount(); j++){
                N.setAt(i,j, BBT.getAt(i,j));
            }
        }
        for(int i = 0; i < numPoints; i++){
            for(int j = 0; j < 4; j++){
                N.setAt(i, j+numPoints, A.getAt(i,j));
            }
        }
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < numPoints; j++){
                N.setAt(i+numPoints, j, AT.getAt(i,j));
            }
        }

        try{
            if(!OiMat::solve(res, N, -1.0*w)){
                this->myState->isValid = false;
                return;
            }
            //Q = N.inv();
            //res = -1.0 * Q * w;
        }catch(exception &e){
            qDebug() << e.what();
            this->myState->isValid = false;
            return;
        }

        for(int i = 0; i < numPoints; i++){
            k.setAt(i, res.getAt(i));
        }
        for(int i = 0; i < 4; i++){
            xd.setAt(i, res.getAt(i+numPoints));
        }

        vd = B.t() * k;

        //Unbekannte und Beobachtungen verbessern
        verb = verb + vd;
        xm += xd.getAt(0);
        ym += xd.getAt(1);
        zm += xd.getAt(2);
        r += xd.getAt(3);

        numIterations++;

    }while( (OiVec::dot(xd,xd) > 0.000001) && (numIterations < 100) );

    if(numIterations >= 100){
        this->myState->isValid = false;
        return;
    }

    double sumVV = 0.0;
    for(int i = 0; i < this->myState->myPoints.size(); i++){

        x = this->myState->myPoints.at(i)->xyz[0];
        y = this->myState->myPoints.at(i)->xyz[1];
        z = this->myState->myPoints.at(i)->xyz[2];

        sumVV += (r - qSqrt( (xm-x)*(xm-x) + (ym-y)*(ym-y) + (zm-z)*(zm-z) ))
                * (r - qSqrt( (xm-x)*(xm-x) + (ym-y)*(ym-y) + (zm-z)*(zm-z) ));

    }

    this->mySphereState->radius = r;
    this->mySphereState->xyz[0] = xm;
    this->mySphereState->xyz[1] = ym;
    this->mySphereState->xyz[2] = zm;
    this->myState->mainFocus[0] = centroid[0];
    this->myState->mainFocus[1] = centroid[1];
    this->myState->mainFocus[2] = centroid[2];
    this->myState->sigma = qSqrt( sumVV / ((double)numPoints - 4.0) );
    this->myState->isValid = true;

}

/*!
 * \brief PS_SphereSegment::fitBySample
 * Fit the sphere using a random subset of points to increase the performance
 * \param numPoints
 */
void PS_SphereSegment::fitBySample(unsigned int numPoints){

    if(this->myState->myPoints.size() < 5 || numPoints < 5){
        this->myState->isValid = false;
        return;
    }

    //get random subset of numPoints points
    if(numPoints > this->getPoints().size()){
        numPoints = this->getPoints().size();
    }
    QMap<int, QList<Point_PC*> > randomSampleMap = PS_GeneralMath::getRandomSubsets(1, numPoints, this->myState->myPoints);
    QList<Point_PC*> randomSample = randomSampleMap.value(0);

    double centroid[3], centroidAll[3]; //centroids of numPoints and all points
    centroid[0] = 0.0;
    centroid[1] = 0.0;
    centroid[2] = 0.0;
    centroidAll[0] = 0.0;
    centroidAll[1] = 0.0;
    centroidAll[2] = 0.0;

    double x = 0.0, y = 0.0, z = 0.0;
    double r = 0.0, xm = 0.0, ym = 0.0, zm = 0.0;

    //calc centroid of all sphere points
    for(int i = 0; i < this->getPoints().size(); i++){
        centroidAll[0] += this->myState->myPoints.at(i)->xyz[0];
        centroidAll[1] += this->myState->myPoints.at(i)->xyz[1];
        centroidAll[2] += this->myState->myPoints.at(i)->xyz[2];
    }
    centroidAll[0] = centroidAll[0] / (double)this->getPoints().size();
    centroidAll[1] = centroidAll[1] / (double)this->getPoints().size();
    centroidAll[2] = centroidAll[2] / (double)this->getPoints().size();

    //calc centroid of numPoints points
    for(int i = 0; i < numPoints; i++){
        centroid[0] += randomSample.at(i)->xyz[0];
        centroid[1] += randomSample.at(i)->xyz[1];
        centroid[2] += randomSample.at(i)->xyz[2];
    }
    centroid[0] = centroid[0] / (double)numPoints;
    centroid[1] = centroid[1] / (double)numPoints;
    centroid[2] = centroid[2] / (double)numPoints;

    //fit the sphere with Drixler
    for(int i = 0; i < 4; i++){
        PS_SphereSegment::fitting_n.setAt(i, 0.0);
        for(int j = 0; j < 4; j++){
            PS_SphereSegment::fitting_N.setAt(i, j, 0.0);
        }
    }
    for(unsigned int i = 0; i < numPoints; i++){

        x = randomSample.at(i)->xyz[0] - centroid[0];
        y = randomSample.at(i)->xyz[1] - centroid[1];
        z = randomSample.at(i)->xyz[2] - centroid[2];

        double xx = x*x;
        double yy = y*y;
        double zz = z*z;
        double xy = x*y;
        double xz = x*z;
        double yz = y*z;
        double xyz2 = x*x + y*y + z*z;

        PS_SphereSegment::fitting_N.setAt(0,0, PS_SphereSegment::fitting_N.getAt(0,0) + xx);
        PS_SphereSegment::fitting_N.setAt(0,1, PS_SphereSegment::fitting_N.getAt(0,1) + xy);
        PS_SphereSegment::fitting_N.setAt(0,2, PS_SphereSegment::fitting_N.getAt(0,2) + xz);
        PS_SphereSegment::fitting_N.setAt(0,3, PS_SphereSegment::fitting_N.getAt(0,3) + x);
        PS_SphereSegment::fitting_N.setAt(1,0, PS_SphereSegment::fitting_N.getAt(1,0) + xy);
        PS_SphereSegment::fitting_N.setAt(1,1, PS_SphereSegment::fitting_N.getAt(1,1) + yy);
        PS_SphereSegment::fitting_N.setAt(1,2, PS_SphereSegment::fitting_N.getAt(1,2) + yz);
        PS_SphereSegment::fitting_N.setAt(1,3, PS_SphereSegment::fitting_N.getAt(1,3) + y);
        PS_SphereSegment::fitting_N.setAt(2,0, PS_SphereSegment::fitting_N.getAt(2,0) + xz);
        PS_SphereSegment::fitting_N.setAt(2,1, PS_SphereSegment::fitting_N.getAt(2,1) + yz);
        PS_SphereSegment::fitting_N.setAt(2,2, PS_SphereSegment::fitting_N.getAt(2,2) + zz);
        PS_SphereSegment::fitting_N.setAt(2,3, PS_SphereSegment::fitting_N.getAt(2,3) + z);
        PS_SphereSegment::fitting_N.setAt(3,0, PS_SphereSegment::fitting_N.getAt(3,0) + x);
        PS_SphereSegment::fitting_N.setAt(3,1, PS_SphereSegment::fitting_N.getAt(3,1) + y);
        PS_SphereSegment::fitting_N.setAt(3,2, PS_SphereSegment::fitting_N.getAt(3,2) + z);
        PS_SphereSegment::fitting_N.setAt(3,3, PS_SphereSegment::fitting_N.getAt(3,3) + 1.0);

        PS_SphereSegment::fitting_n.setAt(0, PS_SphereSegment::fitting_n.getAt(0) + x*xyz2);
        PS_SphereSegment::fitting_n.setAt(1, PS_SphereSegment::fitting_n.getAt(1) + y*xyz2);
        PS_SphereSegment::fitting_n.setAt(2, PS_SphereSegment::fitting_n.getAt(2) + z*xyz2);
        PS_SphereSegment::fitting_n.setAt(3, PS_SphereSegment::fitting_n.getAt(3) + xyz2);

    }

    OiVec a(4);
    try{
        if(!OiMat::solve(a, PS_SphereSegment::fitting_N, -1.0*PS_SphereSegment::fitting_n)){
            this->myState->isValid = false;
            return;
        }
        //PS_SphereSegment::fitting_Q = PS_SphereSegment::fitting_N.inv();
        //a = -1.0 * PS_SphereSegment::fitting_Q * PS_SphereSegment::fitting_n;
    }catch(exception &e){
        qDebug() << e.what();
        this->myState->isValid = false;
        return;
    }

    r = qSqrt( qAbs( 0.25 * (a.getAt(0)*a.getAt(0) + a.getAt(1)*a.getAt(1) + a.getAt(2)*a.getAt(2)) - a.getAt(3) ) );
    xm = -0.5 * a.getAt(0) + centroid[0];
    ym = -0.5 * a.getAt(1) + centroid[1];
    zm = -0.5 * a.getAt(2) + centroid[2];

    double sumVV = 0.0;
    for(int i = 0; i < this->myState->myPoints.size(); i++){

        x = this->myState->myPoints.at(i)->xyz[0];
        y = this->myState->myPoints.at(i)->xyz[1];
        z = this->myState->myPoints.at(i)->xyz[2];

        sumVV += (r - qSqrt( (xm-x)*(xm-x) + (ym-y)*(ym-y) + (zm-z)*(zm-z) ))
                * (r - qSqrt( (xm-x)*(xm-x) + (ym-y)*(ym-y) + (zm-z)*(zm-z) ));

    }

    //set results
    this->mySphereState->radius = r;
    this->mySphereState->xyz[0] = xm;
    this->mySphereState->xyz[1] = ym;
    this->mySphereState->xyz[2] = zm;
    this->myState->mainFocus[0] = centroidAll[0];
    this->myState->mainFocus[1] = centroidAll[1];
    this->myState->mainFocus[2] = centroidAll[2];
    this->myState->sigma = qSqrt( sumVV / ((double)this->myState->myPoints.size() - 4.0) );
    this->myState->isValid = true;

}

/*!
 * \brief PS_SphereSegment::minimumSolution
 * Calculate sphere from 4 points
 * \param points
 */
void PS_SphereSegment::minimumSolution(QList<Point_PC *> points){

    if(points.size() != 4){
        this->myState->isValid = false;
        return;
    }

    for(int i = 0; i < 4; i++){
        PS_SphereSegment::min_A.setAt(i, 0, -2.0 * points.at(i)->xyz[0]);
        PS_SphereSegment::min_A.setAt(i, 1, -2.0 * points.at(i)->xyz[1]);
        PS_SphereSegment::min_A.setAt(i, 2, -2.0 * points.at(i)->xyz[2]);
        PS_SphereSegment::min_A.setAt(i, 3, 1.0);

        PS_SphereSegment::min_C.setAt(i, -1.0*points.at(i)->xyz[0]*points.at(i)->xyz[0]
                - points.at(i)->xyz[1]*points.at(i)->xyz[1]
                - points.at(i)->xyz[2]*points.at(i)->xyz[2]);
    }

    double detA = PS_SphereSegment::min_A.det();
    for(int i = 0; i < 4; i++){
        PS_SphereSegment::min_AReplaced = PS_SphereSegment::min_A;
        PS_SphereSegment::min_AReplaced.setCol(i, PS_SphereSegment::min_C);
        double detAr = PS_SphereSegment::min_AReplaced.det();
        PS_SphereSegment::min_X.setAt(i, detAr / detA);
    }

    //set the sphere's attributes to the calculated values
    this->mySphereState->radius = qSqrt( PS_SphereSegment::min_X.getAt(0)*PS_SphereSegment::min_X.getAt(0)
                                         + PS_SphereSegment::min_X.getAt(1)*PS_SphereSegment::min_X.getAt(1)
                                         + PS_SphereSegment::min_X.getAt(2)*PS_SphereSegment::min_X.getAt(2)
                                         - PS_SphereSegment::min_X.getAt(3) );
    this->mySphereState->xyz[0] = PS_SphereSegment::min_X.getAt(0);
    this->mySphereState->xyz[1] = PS_SphereSegment::min_X.getAt(1);
    this->mySphereState->xyz[2] = PS_SphereSegment::min_X.getAt(2);
    this->myState->isValid = true;

}

/*!
 * \brief PS_SphereSegment::sortOut
 * Only include points in the sphere that are within a distance around the sphere surface (specified by the user)
 * \param mySphere
 * \param param
 */
void PS_SphereSegment::sortOut(PS_SphereSegment *mySphere, PS_InputParameter param){

    QList<Point_PC *> myPoints = mySphere->getPoints();
    mySphere->removeAllPoints();
    PS_SphereSegment::checkPointsInSphere(mySphere, myPoints, param, 1);

}

/*!
 * \brief PS_SphereSegment::mergeSpheres
 * Merge detected spheres which seem to be the same one
 * \param detectedSpheres
 * \param mergedSpheres
 * \param param
 */
void PS_SphereSegment::mergeSpheres(const QList<PS_SphereSegment *> &detectedSpheres, QList<PS_SphereSegment *> &mergedSpheres, PS_InputParameter param){

    //distance of main focus of the points on one sphere to the centroid of the other sphere
    float dCentroid2FocusS1 = 0.0, dCentroid2FocusS2 = 0.0;

    if(mergedSpheres.size() != 0){
        mergedSpheres.clear();
    }

    //for each sphere save its merged state
    QMap<int, bool> mergeMap;
    for(int i = 0; i < detectedSpheres.size(); i++){
        mergeMap.insert(i, false);
    }

    qDebug() << "numSpheres: " << detectedSpheres.size();

    //run through all detected spheres
    for(int i = 0; i < detectedSpheres.size(); i++){

        qDebug() << i;

        //if the sphere at position i was not merged before
        if(mergeMap.value(i) == false){

            mergeMap.insert(i, true); //set as merged

            PS_SphereSegment *s1 = detectedSpheres.at(i); //get the first sphere

            //compare all sphere's params and merge them if they fit together
            for(int j = i+1; j < detectedSpheres.size(); j++){

                //if the sphere at position j was not merged before
                if(mergeMap.value(j) == false){

                    PS_SphereSegment *s2 = detectedSpheres.at(j); //get the second sphere

                    //TODO getXYZ etc. inline machen

                    dCentroid2FocusS1 = qSqrt( (s2->getXYZ()[0] - s1->getMainFocus()[0]) * (s2->getXYZ()[0] - s1->getMainFocus()[0])
                            + (s2->getXYZ()[1] - s1->getMainFocus()[1]) * (s2->getXYZ()[1] - s1->getMainFocus()[1])
                            + (s2->getXYZ()[2] - s1->getMainFocus()[2]) * (s2->getXYZ()[2] - s1->getMainFocus()[2]) );
                    dCentroid2FocusS2 = qSqrt( (s1->getXYZ()[0] - s2->getMainFocus()[0]) * (s1->getXYZ()[0] - s2->getMainFocus()[0])
                            + (s1->getXYZ()[1] - s2->getMainFocus()[1]) * (s1->getXYZ()[1] - s2->getMainFocus()[1])
                            + (s1->getXYZ()[2] - s2->getMainFocus()[2]) * (s1->getXYZ()[2] - s2->getMainFocus()[2]) );

                    //if the main focus of the points on one sphere is inside the other sphere
                    if(dCentroid2FocusS1 <= 2.0*s2->getRadius() || dCentroid2FocusS2 <= 2.0*s1->getRadius()){

                        PS_SphereSegment *mergedSphere = new PS_SphereSegment();
                        mergedSphere->setIsValid(true);
                        foreach(Point_PC *myPoint, s1->getPoints()){
                            myPoint->isUsed = false;
                            mergedSphere->addPoint(myPoint);
                        }
                        foreach(Point_PC *myPoint, s2->getPoints()){
                            myPoint->isUsed = false;
                            mergedSphere->addPoint(myPoint);
                        }

                        if(dCentroid2FocusS1 < s2->getRadius()){
                            mergedSphere->setApproximation(s2->getRadius(), s2->getXYZ()[0], s2->getXYZ()[1], s2->getXYZ()[2]);
                        }else{
                            mergedSphere->setApproximation(s1->getRadius(), s1->getXYZ()[0], s1->getXYZ()[1], s1->getXYZ()[2]);
                        }

                        //mergedSphere->fit();
                        mergedSphere->fitBySample(param.fitSampleSize);

                        //if the merged sphere's standard deviation is ok
                        if(mergedSphere->getIsValid() && mergedSphere->getSigma() <= param.sphereParams.maxDistance
                                && mergedSphere->getRadius() <= param.sphereParams.maxRadius
                                && mergedSphere->getRadius() >= param.sphereParams.minRadius){

                            //sort out points that do not satisfy the distance criterion and refit
                            PS_SphereSegment::sortOut(mergedSphere, param);
                            mergedSphere->fitBySample(param.fitSampleSize);
                            //mergedSphere->fit();

                            if(mergedSphere->getIsValid() && mergedSphere->getPoints().size() >= param.sphereParams.minPoints){

                                //delete single spheres
                                delete s1;
                                delete s2;

                                //set the sphere to be merged
                                mergeMap.insert(j, true);

                                s1 = mergedSphere;

                            }else{
                                delete mergedSphere;
                            }

                        }else{
                            delete mergedSphere;
                        }

                    }

                }

            }

            //add the sphere to result list
            mergedSpheres.append(s1);

        }

    }

}

/*!
 * \brief PS_SphereSegment::verifySpheres
 * Verify the detected spheres (eg. sort out spheres whose points almost lie in a plane)
 * \param detectedSpheres
 * \param mergedSpheres
 * \param param
 */
void PS_SphereSegment::verifySpheres(const QList<PS_SphereSegment *> &detectedSpheres, QList<PS_SphereSegment *> &verifiedSpheres, PS_InputParameter param){

    //initialize variables
    double centroid[3]; //main focus of sphere points
    vector<OiVec> crCoord; //centroid reduced coordinates
    int numPoints;
    double eVal; //smallest eigen-value
    double planeSigma; //sigma of a fitted plane

    foreach(PS_SphereSegment *sphere, detectedSpheres){

        crCoord.clear();

        numPoints = sphere->getPoints().size();

        OiMat a(numPoints, 3); //a matrix
        OiMat ata;

        //calc centroid of sphere points
        centroid[0] = 0.0;
        centroid[1] = 0.0;
        centroid[2] = 0.0;
        for(unsigned int i = 0; i < numPoints; i++){
            Point_PC *p = sphere->getPoints().at(i);
            centroid[0] += p->xyz[0];
            centroid[1] += p->xyz[1];
            centroid[2] += p->xyz[2];
        }
        centroid[0] = centroid[0] / (double)numPoints;
        centroid[1] = centroid[1] / (double)numPoints;
        centroid[2] = centroid[2] / (double)numPoints;

        //calc centroid reduced coordinates
        for(unsigned int i = 0; i < numPoints; i++){
            Point_PC *p = sphere->getPoints().at(i);
            PS_SphereSegment::verify_xyz.setAt(0, p->xyz[0] - centroid[0]);
            PS_SphereSegment::verify_xyz.setAt(1, p->xyz[1] - centroid[1]);
            PS_SphereSegment::verify_xyz.setAt(2, p->xyz[2] - centroid[2]);
            crCoord.push_back( PS_SphereSegment::verify_xyz );
        }

        //set up A matrix
        for(int i = 0; i < numPoints; i++){
            a.setAt(i, 0, crCoord.at(i).getAt(0));
            a.setAt(i, 1, crCoord.at(i).getAt(1));
            a.setAt(i, 2, crCoord.at(i).getAt(2));
        }

        //principal component analysis
        ata = a.t() * a;
        ata.svd(PS_SphereSegment::verify_u, PS_SphereSegment::verify_d, PS_SphereSegment::verify_v);

        eVal = numeric_limits<double>::max();
        for(int i = 0; i < 3; i++){
            if(PS_SphereSegment::verify_d.getAt(i) < eVal){
                eVal = PS_SphereSegment::verify_d.getAt(i);
            }
        }

        planeSigma = qSqrt(eVal / (double)(numPoints - 3.0));

        if( planeSigma > 3.0*param.sphereParams.maxDistance ){

            verifiedSpheres.append(sphere);

        }else{
            delete sphere;
        }

    }

}

/*!
 * \brief SphereSegment::getRadius
 * \return
 */
float PS_SphereSegment::getRadius(){
    return this->mySphereState->radius;
}

/*!
 * \brief SphereSegment::getXYZ
 * \return
 */
float *PS_SphereSegment::getXYZ(){
    return this->mySphereState->xyz;
}

/*!
 * \brief SphereSegment::setApproximation
 * Set approximated sphere attributes manually to be able to fit the sphere
 * \param radius
 * \param x
 * \param y
 * \param z
 */
void PS_SphereSegment::setApproximation(float radius, float x, float y, float z){
    this->mySphereState->radius = radius;
    this->mySphereState->xyz[0] = x;
    this->mySphereState->xyz[1] = y;
    this->mySphereState->xyz[2] = z;
}
