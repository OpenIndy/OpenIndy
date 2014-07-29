#include "ps_cylindersegment.h"

OiMat PS_CylinderSegment::Ralpha = OiMat(3,3);
OiMat PS_CylinderSegment::Rbeta = OiMat(3,3);
OiMat PS_CylinderSegment::Rall = OiMat(3,3);
OiVec PS_CylinderSegment::x_m_n = OiVec(3);
OiVec PS_CylinderSegment::n0 = OiVec(3);
OiVec PS_CylinderSegment::a = OiVec(5);
OiVec PS_CylinderSegment::b = OiVec(5);
OiVec PS_CylinderSegment::X0 = OiVec(5);
OiVec PS_CylinderSegment::x = OiVec(5);
OiMat PS_CylinderSegment::H = OiMat(3,3);
OiMat PS_CylinderSegment::u = OiMat(3,3);
OiVec PS_CylinderSegment::d = OiVec(3);
OiMat PS_CylinderSegment::v = OiMat(3,3);

PS_CylinderSegment::PS_CylinderSegment(){

    //create cylinder states and make sure that myCylinderState points to the same object as myState
    this->myCylinderState = new CylinderState();
    this->myOldState = new CylinderState();
    this->myState = this->myCylinderState;

}

PS_CylinderSegment::~PS_CylinderSegment()
{
}

/*!
 * \brief PS_CylinderSegment::detectCylinder
 * Tries to detect a single cylinder in the given point material that contains maximum points that satisfy the distance criterion
 * \param points
 * \param param
 * \return
 */
PS_CylinderSegment *PS_CylinderSegment::detectCylinder(QList<Point_PC *> points, PS_InputParameter param){

    PS_CylinderSegment *result = new PS_CylinderSegment();

    //qDebug() << "sample size: " << points.size();

    int numTrials = 0; //number of necessary trials
    int k = 9; //number of points in a required minimal set to define a cylinder (here 9 points)
    float p = 1.0 - param.outlierPercentage; //probability of drawing a cylinder-point from the set of points
    float s = 0.99; //required probability of detecting the cylinder in numTrials trials

    float phk = p;
    for(int i = 0; i < k-1; i++){
        phk = phk * p;
    }

    //calculate the number of necessary trials
    numTrials = qLn(1.0 - s) / qLn(1.0 - phk);

    //get numTrials random samples
    QMap<int, QList<Point_PC*> > randomSamples;
    randomSamples = PS_GeneralMath::getRandomSubsets(numTrials, 9, points);

    for(int i = 0; i < numTrials; i++){

        //create cylinder from k points
        PS_CylinderSegment *possibleSolution = new PS_CylinderSegment();
        possibleSolution->minimumSolution(randomSamples.value(i));

        //if the radius is too large/low or there is no solution then break and consider the next random sample
        if(!possibleSolution->getIsValid() || possibleSolution->getRadius() > param.cylinderParams.maxRadius
                || possibleSolution->getRadius() < param.cylinderParams.minRadius){
            continue;
        }

        //if at least k+1 points are within the cylinder...
        if(PS_CylinderSegment::checkPointsInCylinder(possibleSolution, points, param, 3) >= 6){

            //...and the solution is better then the best one found before
            if(possibleSolution->getPoints().size() > result->getPoints().size()){

                //delete the old "best" cylinder
                if(result != NULL){
                    delete result;
                }

                //set the new cylinder as result
                result = possibleSolution;

                //if the cylinder contains all points of the voxel then break the search
                if(result->getPoints().size() == points.size()){
                    break;
                }
            }

        }

    }

    return result;

}

/*!
 * \brief PS_CylinderSegment::checkPointsInCylinder
 * Returns the number of points that were added to the cylinder
 * toleranceFactor is a factor that defines wether a point is in a cylinder
 * \param myCylinder
 * \param myPoints
 * \param param
 * \param toleranceFactor
 * \return
 */
int PS_CylinderSegment::checkPointsInCylinder(PS_CylinderSegment *myCylinder, QList<Point_PC *> myPoints, PS_InputParameter param, int toleranceFactor){

    int result = 0; //number of points that were added to the cylinder

    if(myCylinder->getIsValid()){

        //iterate through all points to check wether they lie in a small band around the cylinder surface
        for(unsigned int i = 0; i < myPoints.size(); i++){

            Point_PC *p = myPoints.at(i);

            //if the point is not used for another shape
            if(!p->isUsed){

                PS_CylinderSegment::Ralpha.setAt(0, 0, 1.0);
                PS_CylinderSegment::Ralpha.setAt(1, 1, qCos(myCylinder->getAlpha()));
                PS_CylinderSegment::Ralpha.setAt(1, 2, -qSin(myCylinder->getAlpha()));
                PS_CylinderSegment::Ralpha.setAt(2, 1, qSin(myCylinder->getAlpha()));
                PS_CylinderSegment::Ralpha.setAt(2, 2, qCos(myCylinder->getAlpha()));
                PS_CylinderSegment::Rbeta.setAt(0, 0, qCos(myCylinder->getBeta()));
                PS_CylinderSegment::Rbeta.setAt(0, 2, qSin(myCylinder->getBeta()));
                PS_CylinderSegment::Rbeta.setAt(1, 1, 1.0);
                PS_CylinderSegment::Rbeta.setAt(2, 0, -qSin(myCylinder->getBeta()));
                PS_CylinderSegment::Rbeta.setAt(2, 2, qCos(myCylinder->getBeta()));

                PS_CylinderSegment::Rall = PS_CylinderSegment::Rbeta * PS_CylinderSegment::Ralpha;

                PS_CylinderSegment::x_m_n.setAt(0, myCylinder->getXYZ()[0]);
                PS_CylinderSegment::x_m_n.setAt(1, myCylinder->getXYZ()[1]);
                PS_CylinderSegment::x_m_n.setAt(2, 0.0);
                OiMat::solve(PS_CylinderSegment::x_m_n, PS_CylinderSegment::Rall, (-1.0) * PS_CylinderSegment::x_m_n);
                //PS_CylinderSegment::x_m_n = (PS_CylinderSegment::Rbeta * PS_CylinderSegment::Ralpha).inv() * ((-1.0) * PS_CylinderSegment::x_m_n);

                PS_CylinderSegment::n0.setAt(0, 0.0);
                PS_CylinderSegment::n0.setAt(1, 0.0);
                PS_CylinderSegment::n0.setAt(2, 1.0);
                OiMat::solve(PS_CylinderSegment::n0, PS_CylinderSegment::Rall, PS_CylinderSegment::n0);
                //PS_CylinderSegment::n0 = (PS_CylinderSegment::Rbeta * PS_CylinderSegment::Ralpha).inv() * PS_CylinderSegment::n0;

                float b[3]; //vector between point on cylinder axis and point p which is probably on cylinder
                b[0] = p->xyz[0] - PS_CylinderSegment::x_m_n.getAt(0);
                b[1] = p->xyz[1] - PS_CylinderSegment::x_m_n.getAt(1);
                b[2] = p->xyz[2] - PS_CylinderSegment::x_m_n.getAt(2);

                float n0CrossB[3]; //cross product of cylinder axis (length 1) and b
                n0CrossB[0] = PS_CylinderSegment::n0.getAt(1) * b[2] - PS_CylinderSegment::n0.getAt(2) * b[1];
                n0CrossB[1] = PS_CylinderSegment::n0.getAt(2) * b[0] - PS_CylinderSegment::n0.getAt(0) * b[2];
                n0CrossB[2] = PS_CylinderSegment::n0.getAt(0) * b[1] - PS_CylinderSegment::n0.getAt(1) * b[0];

                float radiusActual = 0.0f; //smallest distance of point p to cylinder axis

                radiusActual = qSqrt( n0CrossB[0]*n0CrossB[0] + n0CrossB[1]*n0CrossB[1] + n0CrossB[2]*n0CrossB[2] );

                float distance = 0.0f;

                distance = radiusActual - myCylinder->getRadius(); //distance error

                //add the point to the cylinder if the distance is lower than a threshold
                float absDist = qAbs(distance);
                if(absDist < (((float)toleranceFactor) * param.cylinderParams.maxDistance)){
                    myCylinder->addPoint(p);
                    result++;
                }

            }

        }

    }

    return result;

}

/*!
 * \brief PS_CylinderSegment::fit
 * Fit the cylinder using all points
 */
void PS_CylinderSegment::fit(){

    if(this->myState->myPoints.size() < 6){
        this->myState->isValid = false;
        return;
    }

    int numPoints = this->getPoints().size();

    //centroids of all points
    double centroid[3];
    centroid[0] = 0.0;
    centroid[1] = 0.0;
    centroid[2] = 0.0;

    //initialize variables
    OiVec L(numPoints*3); //observations
    OiVec v(numPoints*3); //approximation of corrections
    OiVec L0(numPoints*3); //L + v
    OiMat B(numPoints, 3*numPoints); //derivations with respect to (L+v0)
    OiMat A(numPoints, 5); //derivations with respect to unknowns
    OiVec c(numPoints+5); //right side
    OiMat BBT(numPoints, numPoints);
    OiMat AT(5, numPoints);
    OiMat N(numPoints+5, numPoints+5); //normal equation matrix
    OiVec res(numPoints+5); //adjustment result
    OiVec k(numPoints);
    double _r = 0.0, _X0 = 0.0, _Y0 = 0.0, _alpha = 0.0, _beta = 0.0;
    double _x = 0.0, _y = 0.0, _z = 0.0;
    double a1 = 0.0, a2 = 0.0;
    double diff = 0.0, _xr = 0.0, _yr = 0.0;
    double sigma = 2.0;
    OiVec ua;

    //set approximations of unknowns
    PS_CylinderSegment::X0.setAt(0, this->getRadius());
    PS_CylinderSegment::X0.setAt(1, this->getXYZ()[0]);
    PS_CylinderSegment::X0.setAt(2, this->getXYZ()[1]);
    PS_CylinderSegment::X0.setAt(3, this->getAlpha());
    PS_CylinderSegment::X0.setAt(4, this->getBeta());
    for(int i = 0; i < 5; i++){
        PS_CylinderSegment::x.setAt(i, 0.0);
    }

    //fill L vector
    for(int i = 0; i < numPoints; i++){
        L.setAt(i*3, this->myState->myPoints.at(i)->xyz[0]);
        L.setAt(i*3+1, this->myState->myPoints.at(i)->xyz[1]);
        L.setAt(i*3+2, this->myState->myPoints.at(i)->xyz[2]);

        centroid[0] += this->myState->myPoints.at(i)->xyz[0];
        centroid[1] += this->myState->myPoints.at(i)->xyz[1];
        centroid[2] += this->myState->myPoints.at(i)->xyz[2];
    }
    L0 = L;
    centroid[0] = centroid[0] / (double)numPoints;
    centroid[1] = centroid[1] / (double)numPoints;
    centroid[2] = centroid[2] / (double)numPoints;

    int numIterations = 0;

    do{

        //improve unknowns
        PS_CylinderSegment::X0 = PS_CylinderSegment::X0 + PS_CylinderSegment::x;

        //improve observations
        L0 = L0 + v;

        //build rotations matrices
        PS_CylinderSegment::Ralpha.setAt(0, 0, 1.0);
        PS_CylinderSegment::Ralpha.setAt(1, 1, qCos(PS_CylinderSegment::X0.getAt(3)));
        PS_CylinderSegment::Ralpha.setAt(1, 2, -qSin(PS_CylinderSegment::X0.getAt(3)));
        PS_CylinderSegment::Ralpha.setAt(2, 1, qSin(PS_CylinderSegment::X0.getAt(3)));
        PS_CylinderSegment::Ralpha.setAt(2, 2, qCos(PS_CylinderSegment::X0.getAt(3)));
        PS_CylinderSegment::Rbeta.setAt(0, 0, qCos(PS_CylinderSegment::X0.getAt(4)));
        PS_CylinderSegment::Rbeta.setAt(0, 2, qSin(PS_CylinderSegment::X0.getAt(4)));
        PS_CylinderSegment::Rbeta.setAt(1, 1, 1.0);
        PS_CylinderSegment::Rbeta.setAt(2, 0, -qSin(PS_CylinderSegment::X0.getAt(4)));
        PS_CylinderSegment::Rbeta.setAt(2, 2, qCos(PS_CylinderSegment::X0.getAt(4)));

        PS_CylinderSegment::Rall = PS_CylinderSegment::Rbeta * PS_CylinderSegment::Ralpha;

        //fill A and B matrix + w vector + rechte Seite
        _r = PS_CylinderSegment::X0.getAt(0);
        _X0 = PS_CylinderSegment::X0.getAt(1);
        _Y0 = PS_CylinderSegment::X0.getAt(2);
        _alpha = PS_CylinderSegment::X0.getAt(3);
        _beta = PS_CylinderSegment::X0.getAt(4);
        for(int i = 0; i < numPoints; i++){
            _x = L0.getAt(i*3);
            _y = L0.getAt(i*3+1);
            _z = L0.getAt(i*3+2);

            a1 = _X0 + _x * qCos(_beta) + _y * qSin(_alpha) * qSin(_beta) + _z * qCos(_alpha) * qSin(_beta);
            a2 = _Y0 + _y * qCos(_alpha) - _z * qSin(_alpha);

            B.setAt(i, i*3, -1.0 * qCos(_beta) * a1 / qSqrt(a1*a1 + a2*a2)); // x
            B.setAt(i, i*3+1, -1.0 * (qSin(_alpha) * qSin(_beta) * a1 + qCos(_alpha) * a2) / qSqrt(a1*a1 + a2*a2)); // y
            B.setAt(i, i*3+2, -1.0 * (qCos(_alpha) * qSin(_beta) * a1 - qSin(_alpha) * a2) / qSqrt(a1*a1 + a2*a2)); // z

            A.setAt(i, 0, 1.0); //r
            A.setAt(i, 1, -1.0 * a1 / qSqrt(a1*a1 + a2*a2)); // X0
            A.setAt(i, 2, -1.0 * a2 / qSqrt(a1*a1 + a2*a2)); // Y0
            A.setAt(i, 3, -1.0 * ((_y * qSin(_beta) * qCos(_alpha) - _z * qSin(_beta) * qSin(_alpha)) * a1 - (_y * qSin(_alpha) + _z * qCos(_alpha)) * a2) / qSqrt(a1*a1 + a2*a2)); // alpha
            A.setAt(i, 4, -1.0 * (_y * qSin(_alpha) * qCos(_beta) - _x * qSin(_beta) + _z * qCos(_alpha) * qCos(_beta)) * a1 / qSqrt(a1*a1 + a2*a2)); // beta

            _xr = PS_CylinderSegment::Rall.getAt(0,0) * _x + PS_CylinderSegment::Rall.getAt(0,1) * _y + PS_CylinderSegment::Rall.getAt(0,2) * _z + X0.getAt(1);
            _yr = PS_CylinderSegment::Rall.getAt(1,0) * _x + PS_CylinderSegment::Rall.getAt(1,1) * _y + PS_CylinderSegment::Rall.getAt(1,2) * _z + X0.getAt(2);

            //genäherter Radius des Zylinders minus Abstand Punkt i zur z-Achse ist der Widerspruch
            diff = _r - qSqrt(_xr*_xr + _yr*_yr);

            c.setAt(i, diff);
        }

        //build normal equation
        BBT = B * B.t();
        AT = A.t();
        for(int i = 0; i < (numPoints+5); i++){
            for(int j = 0; j < (numPoints+5); j++){
                if(i < numPoints && j < numPoints){
                    N.setAt(i, j, BBT.getAt(i, j));
                }else if(i < numPoints && j >= numPoints){
                    N.setAt(i, j, A.getAt(i, (j-numPoints)));
                }else if(i >= numPoints && j < numPoints){
                    N.setAt(i, j, AT.getAt((i-numPoints), j));
                }else if(i >= numPoints && j >= numPoints){
                    N.setAt(i, j, 0.0);
                }
            }
        }

        try{
            if(!OiMat::solve(res, N, -1.0*c)){
                this->myState->isValid = false;
                return;
            }
            //res = -1.0 * N.inv() * c;
        }catch(exception &e){
            this->myState->isValid = false;
            return;
        }

        //get results
        for(int i = 0; i < numPoints; i++){
            k.setAt(i, res.getAt(i));
        }
        for(int i = numPoints; i < numPoints+5; i++){
            PS_CylinderSegment::x.setAt(i-numPoints, res.getAt(i));
        }
        v = B.t() * k;

        //s0 a posteriori
        //double sigma = 0.0;
        //sigma = qSqrt((v.t() * v).getAt(0) / 13.0);

        //qDebug() << "loop e: " << 1000.0 * ((clock() - test)/(double)CLOCKS_PER_SEC) << " msec." << endl;

        //Armijo Regel
        do{

            sigma = sigma / 2.0;

            ua = PS_CylinderSegment::X0 + sigma * PS_CylinderSegment::x;
            for(int i = 0; i < 5; i++){
                _x = L0.getAt(i*3);
                _y = L0.getAt(i*3+1);
                _z = L0.getAt(i*3+2);

                PS_CylinderSegment::a.setAt(i, ua.getAt(0) - qSqrt( (ua.getAt(1) + _x*qCos(ua.getAt(4)) + _y*qSin(ua.getAt(3))*qSin(ua.getAt(4)) + _z*qCos(ua.getAt(3))*qSin(ua.getAt(4)))*(ua.getAt(1) + _x*qCos(ua.getAt(4)) + _y*qSin(ua.getAt(3))*qSin(ua.getAt(4)) + _z*qCos(ua.getAt(3))*qSin(ua.getAt(4)))
                                       + (ua.getAt(2) + _y*qCos(ua.getAt(3)) - _z*qSin(ua.getAt(3)))*(ua.getAt(2) + _y*qCos(ua.getAt(3)) - _z*qSin(ua.getAt(3))) ));

                PS_CylinderSegment::b.setAt(i, _r - qSqrt( (_X0 + _x*qCos(_beta) + _y*qSin(_alpha)*qSin(_beta) + _z*qCos(_alpha)*qSin(_beta))*(_X0 + _x*qCos(_beta) + _y*qSin(_alpha)*qSin(_beta) + _z*qCos(_alpha)*qSin(_beta))
                                               + (_Y0 + _y*qCos(_alpha) - _z*qSin(_alpha))*(_Y0 + _y*qCos(_alpha) - _z*qSin(_alpha)) ));
            }


        //}while( OiVec::dot(a,a) > 0.5 * OiVec::dot(b,b) );
        }while( OiVec::dot(PS_CylinderSegment::a,PS_CylinderSegment::a) > ( OiVec::dot(b,b) - 2.0 * 0.001 * sigma * OiVec::dot(PS_CylinderSegment::b,PS_CylinderSegment::b) ) );

        PS_CylinderSegment::x = sigma * PS_CylinderSegment::x;

        //neue Verbesserungen
        //v = B.t() * sigma * k;

        numIterations++;

    }while( (OiVec::dot(PS_CylinderSegment::x,PS_CylinderSegment::x) > 0.00001) && numIterations < 16 );

    if(numIterations >= 16){
        this->myState->isValid = false;
        return;
    }

    PS_CylinderSegment::X0 = PS_CylinderSegment::X0 + PS_CylinderSegment::x;

    //calc sigma
    PS_CylinderSegment::Ralpha.setAt(0, 0, 1.0);
    PS_CylinderSegment::Ralpha.setAt(1, 1, qCos(PS_CylinderSegment::X0.getAt(3)));
    PS_CylinderSegment::Ralpha.setAt(1, 2, -qSin(PS_CylinderSegment::X0.getAt(3)));
    PS_CylinderSegment::Ralpha.setAt(2, 1, qSin(PS_CylinderSegment::X0.getAt(3)));
    PS_CylinderSegment::Ralpha.setAt(2, 2, qCos(PS_CylinderSegment::X0.getAt(3)));
    PS_CylinderSegment::Rbeta.setAt(0, 0, qCos(PS_CylinderSegment::X0.getAt(4)));
    PS_CylinderSegment::Rbeta.setAt(0, 2, qSin(PS_CylinderSegment::X0.getAt(4)));
    PS_CylinderSegment::Rbeta.setAt(1, 1, 1.0);
    PS_CylinderSegment::Rbeta.setAt(2, 0, -qSin(PS_CylinderSegment::X0.getAt(4)));
    PS_CylinderSegment::Rbeta.setAt(2, 2, qCos(PS_CylinderSegment::X0.getAt(4)));

    PS_CylinderSegment::Rall = PS_CylinderSegment::Rbeta * PS_CylinderSegment::Ralpha;

    PS_CylinderSegment::x_m_n.setAt(0, this->getXYZ()[0]);
    PS_CylinderSegment::x_m_n.setAt(1, this->getXYZ()[1]);
    PS_CylinderSegment::x_m_n.setAt(2, 0.0);
    OiMat::solve(PS_CylinderSegment::x_m_n, PS_CylinderSegment::Rall, (-1.0) * PS_CylinderSegment::x_m_n);
    //PS_CylinderSegment::x_m_n = (PS_CylinderSegment::Rbeta * PS_CylinderSegment::Ralpha).inv() * ((-1.0) * PS_CylinderSegment::x_m_n);

    PS_CylinderSegment::n0.setAt(0, 0.0);
    PS_CylinderSegment::n0.setAt(1, 0.0);
    PS_CylinderSegment::n0.setAt(2, 1.0);
    OiMat::solve(PS_CylinderSegment::n0, PS_CylinderSegment::Rall, PS_CylinderSegment::n0);
    //PS_CylinderSegment::n0 = (PS_CylinderSegment::Rbeta * PS_CylinderSegment::Ralpha).inv() * PS_CylinderSegment::n0;

    double sumVV = 0.0;
    for(int i = 0; i < this->myState->myPoints.size(); i++){

        _x = this->myState->myPoints.at(i)->xyz[0];
        _y = this->myState->myPoints.at(i)->xyz[1];
        _z = this->myState->myPoints.at(i)->xyz[2];

        float b[3]; //vector between point on cylinder axis and point p which is probably on cylinder
        b[0] = _x - PS_CylinderSegment::x_m_n.getAt(0);
        b[1] = _y - PS_CylinderSegment::x_m_n.getAt(1);
        b[2] = _z - PS_CylinderSegment::x_m_n.getAt(2);

        float n0CrossB[3]; //cross product of cylinder axis (length 1) and b
        n0CrossB[0] = PS_CylinderSegment::n0.getAt(1) * b[2] - PS_CylinderSegment::n0.getAt(2) * b[1];
        n0CrossB[1] = PS_CylinderSegment::n0.getAt(2) * b[0] - PS_CylinderSegment::n0.getAt(0) * b[2];
        n0CrossB[2] = PS_CylinderSegment::n0.getAt(0) * b[1] - PS_CylinderSegment::n0.getAt(1) * b[0];

        float radiusActual = 0.0f; //smallest distance of point p to cylinder axis

        radiusActual = qSqrt( n0CrossB[0]*n0CrossB[0] + n0CrossB[1]*n0CrossB[1] + n0CrossB[2]*n0CrossB[2] );

        float distance = 0.0f;

        distance = radiusActual - PS_CylinderSegment::X0.getAt(0); //distance error

        sumVV += distance * distance;

    }

    this->myCylinderState->radius = PS_CylinderSegment::X0.getAt(0);
    this->myCylinderState->xyz[0] = PS_CylinderSegment::X0.getAt(1);
    this->myCylinderState->xyz[1] = PS_CylinderSegment::X0.getAt(2);
    this->myCylinderState->alpha = PS_CylinderSegment::X0.getAt(3);
    this->myCylinderState->beta = PS_CylinderSegment::X0.getAt(4);
    this->myState->mainFocus[0] = centroid[0];
    this->myState->mainFocus[1] = centroid[1];
    this->myState->mainFocus[2] = centroid[2];
    this->myState->sigma = qSqrt( sumVV / ((double)numPoints - 5.0) );
    this->myState->isValid = true;

}

/*!
 * \brief PS_CylinderSegment::fitBySample
 * Fit the cylinder using a random subset of points to increase the performance
 * \param numPoints
 */
void PS_CylinderSegment::fitBySample(unsigned int numPoints){

    if(this->myState->myPoints.size() < 6 || numPoints < 6){
        this->myState->isValid = false;
        return;
    }

    //get random subset of numPoints points
    if(numPoints > this->getPoints().size()){
        numPoints = this->getPoints().size();
    }
    QMap<int, QList<Point_PC*> > randomSampleMap = PS_GeneralMath::getRandomSubsets(1, numPoints, this->getPoints());
    QList<Point_PC*> randomSample = randomSampleMap.value(0);

    //centroids of all points
    double centroid[3];
    centroid[0] = 0.0;
    centroid[1] = 0.0;
    centroid[2] = 0.0;
    for(int i = 0; i < this->myState->myPoints.size(); i++){
        centroid[0] += this->myState->myPoints.at(i)->xyz[0];
        centroid[1] += this->myState->myPoints.at(i)->xyz[1];
        centroid[2] += this->myState->myPoints.at(i)->xyz[2];
    }
    centroid[0] = centroid[0] / (double)numPoints;
    centroid[1] = centroid[1] / (double)numPoints;
    centroid[2] = centroid[2] / (double)numPoints;

    //initialize variables
    OiVec L(numPoints*3); //observations
    OiVec v(numPoints*3); //approximation of corrections
    OiVec L0(numPoints*3); //L + v
    OiMat B(numPoints, 3*numPoints); //derivations with respect to (L+v0)
    OiMat A(numPoints, 5); //derivations with respect to unknowns
    OiVec c(numPoints+5); //right side
    OiMat BBT(numPoints, numPoints);
    OiMat AT(5, numPoints);
    OiMat N(numPoints+5, numPoints+5); //normal equation matrix
    OiVec res(numPoints+5); //adjustment result
    OiVec k(numPoints);
    double _r = 0.0, _X0 = 0.0, _Y0 = 0.0, _alpha = 0.0, _beta = 0.0;
    double _x = 0.0, _y = 0.0, _z = 0.0;
    double a1 = 0.0, a2 = 0.0;
    double diff = 0.0, _xr = 0.0, _yr = 0.0;
    double sigma = 2.0;
    OiVec ua;

    //set approximations of unknowns
    PS_CylinderSegment::X0.setAt(0, this->getRadius());
    PS_CylinderSegment::X0.setAt(1, this->getXYZ()[0]);
    PS_CylinderSegment::X0.setAt(2, this->getXYZ()[1]);
    PS_CylinderSegment::X0.setAt(3, this->getAlpha());
    PS_CylinderSegment::X0.setAt(4, this->getBeta());
    for(int i = 0; i < 5; i++){
        PS_CylinderSegment::x.setAt(i, 0.0);
    }

    //fill L vector
    for(int i = 0; i < numPoints; i++){
        L.setAt(i*3, randomSample.at(i)->xyz[0]);
        L.setAt(i*3+1, randomSample.at(i)->xyz[1]);
        L.setAt(i*3+2, randomSample.at(i)->xyz[2]);
    }
    L0 = L;

    int numIterations = 0;

    do{

        //improve unknowns
        PS_CylinderSegment::X0 = PS_CylinderSegment::X0 + PS_CylinderSegment::x;

        //improve observations
        L0 = L0 + v;

        //build rotations matrices
        PS_CylinderSegment::Ralpha.setAt(0, 0, 1.0);
        PS_CylinderSegment::Ralpha.setAt(1, 1, qCos(PS_CylinderSegment::X0.getAt(3)));
        PS_CylinderSegment::Ralpha.setAt(1, 2, -qSin(PS_CylinderSegment::X0.getAt(3)));
        PS_CylinderSegment::Ralpha.setAt(2, 1, qSin(PS_CylinderSegment::X0.getAt(3)));
        PS_CylinderSegment::Ralpha.setAt(2, 2, qCos(PS_CylinderSegment::X0.getAt(3)));
        PS_CylinderSegment::Rbeta.setAt(0, 0, qCos(PS_CylinderSegment::X0.getAt(4)));
        PS_CylinderSegment::Rbeta.setAt(0, 2, qSin(PS_CylinderSegment::X0.getAt(4)));
        PS_CylinderSegment::Rbeta.setAt(1, 1, 1.0);
        PS_CylinderSegment::Rbeta.setAt(2, 0, -qSin(PS_CylinderSegment::X0.getAt(4)));
        PS_CylinderSegment::Rbeta.setAt(2, 2, qCos(PS_CylinderSegment::X0.getAt(4)));

        PS_CylinderSegment::Rall = PS_CylinderSegment::Rbeta * PS_CylinderSegment::Ralpha;

        //fill A and B matrix + w vector + rechte Seite
        _r = PS_CylinderSegment::X0.getAt(0);
        _X0 = PS_CylinderSegment::X0.getAt(1);
        _Y0 = PS_CylinderSegment::X0.getAt(2);
        _alpha = PS_CylinderSegment::X0.getAt(3);
        _beta = PS_CylinderSegment::X0.getAt(4);
        for(int i = 0; i < numPoints; i++){
            _x = L0.getAt(i*3);
            _y = L0.getAt(i*3+1);
            _z = L0.getAt(i*3+2);

            a1 = _X0 + _x * qCos(_beta) + _y * qSin(_alpha) * qSin(_beta) + _z * qCos(_alpha) * qSin(_beta);
            a2 = _Y0 + _y * qCos(_alpha) - _z * qSin(_alpha);

            B.setAt(i, i*3, -1.0 * qCos(_beta) * a1 / qSqrt(a1*a1 + a2*a2)); // x
            B.setAt(i, i*3+1, -1.0 * (qSin(_alpha) * qSin(_beta) * a1 + qCos(_alpha) * a2) / qSqrt(a1*a1 + a2*a2)); // y
            B.setAt(i, i*3+2, -1.0 * (qCos(_alpha) * qSin(_beta) * a1 - qSin(_alpha) * a2) / qSqrt(a1*a1 + a2*a2)); // z

            A.setAt(i, 0, 1.0); //r
            A.setAt(i, 1, -1.0 * a1 / qSqrt(a1*a1 + a2*a2)); // X0
            A.setAt(i, 2, -1.0 * a2 / qSqrt(a1*a1 + a2*a2)); // Y0
            A.setAt(i, 3, -1.0 * ((_y * qSin(_beta) * qCos(_alpha) - _z * qSin(_beta) * qSin(_alpha)) * a1 - (_y * qSin(_alpha) + _z * qCos(_alpha)) * a2) / qSqrt(a1*a1 + a2*a2)); // alpha
            A.setAt(i, 4, -1.0 * (_y * qSin(_alpha) * qCos(_beta) - _x * qSin(_beta) + _z * qCos(_alpha) * qCos(_beta)) * a1 / qSqrt(a1*a1 + a2*a2)); // beta

            _xr = PS_CylinderSegment::Rall.getAt(0,0) * _x + PS_CylinderSegment::Rall.getAt(0,1) * _y + PS_CylinderSegment::Rall.getAt(0,2) * _z + X0.getAt(1);
            _yr = PS_CylinderSegment::Rall.getAt(1,0) * _x + PS_CylinderSegment::Rall.getAt(1,1) * _y + PS_CylinderSegment::Rall.getAt(1,2) * _z + X0.getAt(2);

            //genäherter Radius des Zylinders minus Abstand Punkt i zur z-Achse ist der Widerspruch
            diff = _r - qSqrt(_xr*_xr + _yr*_yr);

            c.setAt(i, diff);
        }

        //build normal equation
        BBT = B * B.t();
        AT = A.t();
        for(int i = 0; i < (numPoints+5); i++){
            for(int j = 0; j < (numPoints+5); j++){
                if(i < numPoints && j < numPoints){
                    N.setAt(i, j, BBT.getAt(i, j));
                }else if(i < numPoints && j >= numPoints){
                    N.setAt(i, j, A.getAt(i, (j-numPoints)));
                }else if(i >= numPoints && j < numPoints){
                    N.setAt(i, j, AT.getAt((i-numPoints), j));
                }else if(i >= numPoints && j >= numPoints){
                    N.setAt(i, j, 0.0);
                }
            }
        }

        try{
            if(!OiMat::solve(res, N, -1.0*c)){
                this->myState->isValid = false;
                return;
            }
            //res = -1.0 * N.inv() * c;
        }catch(exception &e){
            this->myState->isValid = false;
            return;
        }

        //get results
        for(int i = 0; i < numPoints; i++){
            k.setAt(i, res.getAt(i));
        }
        for(int i = numPoints; i < numPoints+5; i++){
            PS_CylinderSegment::x.setAt(i-numPoints, res.getAt(i));
        }
        v = B.t() * k;

        //s0 a posteriori
        //double sigma = 0.0;
        //sigma = qSqrt((v.t() * v).getAt(0) / 13.0);

        //qDebug() << "loop e: " << 1000.0 * ((clock() - test)/(double)CLOCKS_PER_SEC) << " msec." << endl;

        //Armijo Regel
        do{

            sigma = sigma / 2.0;

            ua = PS_CylinderSegment::X0 + sigma * PS_CylinderSegment::x;
            for(int i = 0; i < 5; i++){
                _x = L0.getAt(i*3);
                _y = L0.getAt(i*3+1);
                _z = L0.getAt(i*3+2);

                PS_CylinderSegment::a.setAt(i, ua.getAt(0) - qSqrt( (ua.getAt(1) + _x*qCos(ua.getAt(4)) + _y*qSin(ua.getAt(3))*qSin(ua.getAt(4)) + _z*qCos(ua.getAt(3))*qSin(ua.getAt(4)))*(ua.getAt(1) + _x*qCos(ua.getAt(4)) + _y*qSin(ua.getAt(3))*qSin(ua.getAt(4)) + _z*qCos(ua.getAt(3))*qSin(ua.getAt(4)))
                                       + (ua.getAt(2) + _y*qCos(ua.getAt(3)) - _z*qSin(ua.getAt(3)))*(ua.getAt(2) + _y*qCos(ua.getAt(3)) - _z*qSin(ua.getAt(3))) ));

                PS_CylinderSegment::b.setAt(i, _r - qSqrt( (_X0 + _x*qCos(_beta) + _y*qSin(_alpha)*qSin(_beta) + _z*qCos(_alpha)*qSin(_beta))*(_X0 + _x*qCos(_beta) + _y*qSin(_alpha)*qSin(_beta) + _z*qCos(_alpha)*qSin(_beta))
                                               + (_Y0 + _y*qCos(_alpha) - _z*qSin(_alpha))*(_Y0 + _y*qCos(_alpha) - _z*qSin(_alpha)) ));
            }


        //}while( OiVec::dot(a,a) > 0.5 * OiVec::dot(b,b) );
        }while( OiVec::dot(PS_CylinderSegment::a,PS_CylinderSegment::a) > ( OiVec::dot(b,b) - 2.0 * 0.001 * sigma * OiVec::dot(PS_CylinderSegment::b,PS_CylinderSegment::b) ) );

        PS_CylinderSegment::x = sigma * PS_CylinderSegment::x;

        //neue Verbesserungen
        //v = B.t() * sigma * k;

        numIterations++;

    }while( (OiVec::dot(PS_CylinderSegment::x,PS_CylinderSegment::x) > 0.00001) && numIterations < 16 );

    if(numIterations >= 16){
        this->myState->isValid = false;
        return;
    }

    PS_CylinderSegment::X0 = PS_CylinderSegment::X0 + PS_CylinderSegment::x;

    //calc sigma
    PS_CylinderSegment::Ralpha.setAt(0, 0, 1.0);
    PS_CylinderSegment::Ralpha.setAt(1, 1, qCos(PS_CylinderSegment::X0.getAt(3)));
    PS_CylinderSegment::Ralpha.setAt(1, 2, -qSin(PS_CylinderSegment::X0.getAt(3)));
    PS_CylinderSegment::Ralpha.setAt(2, 1, qSin(PS_CylinderSegment::X0.getAt(3)));
    PS_CylinderSegment::Ralpha.setAt(2, 2, qCos(PS_CylinderSegment::X0.getAt(3)));
    PS_CylinderSegment::Rbeta.setAt(0, 0, qCos(PS_CylinderSegment::X0.getAt(4)));
    PS_CylinderSegment::Rbeta.setAt(0, 2, qSin(PS_CylinderSegment::X0.getAt(4)));
    PS_CylinderSegment::Rbeta.setAt(1, 1, 1.0);
    PS_CylinderSegment::Rbeta.setAt(2, 0, -qSin(PS_CylinderSegment::X0.getAt(4)));
    PS_CylinderSegment::Rbeta.setAt(2, 2, qCos(PS_CylinderSegment::X0.getAt(4)));

    PS_CylinderSegment::Rall = PS_CylinderSegment::Rbeta * PS_CylinderSegment::Ralpha;

    PS_CylinderSegment::x_m_n.setAt(0, this->getXYZ()[0]);
    PS_CylinderSegment::x_m_n.setAt(1, this->getXYZ()[1]);
    PS_CylinderSegment::x_m_n.setAt(2, 0.0);
    OiMat::solve(PS_CylinderSegment::x_m_n, PS_CylinderSegment::Rall, (-1.0) * PS_CylinderSegment::x_m_n);
    //PS_CylinderSegment::x_m_n = (PS_CylinderSegment::Rbeta * PS_CylinderSegment::Ralpha).inv() * ((-1.0) * PS_CylinderSegment::x_m_n);

    PS_CylinderSegment::n0.setAt(0, 0.0);
    PS_CylinderSegment::n0.setAt(1, 0.0);
    PS_CylinderSegment::n0.setAt(2, 1.0);
    OiMat::solve(PS_CylinderSegment::n0, PS_CylinderSegment::Rall, PS_CylinderSegment::n0);
    //PS_CylinderSegment::n0 = (PS_CylinderSegment::Rbeta * PS_CylinderSegment::Ralpha).inv() * PS_CylinderSegment::n0;

    double sumVV = 0.0;
    for(int i = 0; i < this->myState->myPoints.size(); i++){

        _x = this->myState->myPoints.at(i)->xyz[0];
        _y = this->myState->myPoints.at(i)->xyz[1];
        _z = this->myState->myPoints.at(i)->xyz[2];

        float b[3]; //vector between point on cylinder axis and point p which is probably on cylinder
        b[0] = _x - PS_CylinderSegment::x_m_n.getAt(0);
        b[1] = _y - PS_CylinderSegment::x_m_n.getAt(1);
        b[2] = _z - PS_CylinderSegment::x_m_n.getAt(2);

        float n0CrossB[3]; //cross product of cylinder axis (length 1) and b
        n0CrossB[0] = PS_CylinderSegment::n0.getAt(1) * b[2] - PS_CylinderSegment::n0.getAt(2) * b[1];
        n0CrossB[1] = PS_CylinderSegment::n0.getAt(2) * b[0] - PS_CylinderSegment::n0.getAt(0) * b[2];
        n0CrossB[2] = PS_CylinderSegment::n0.getAt(0) * b[1] - PS_CylinderSegment::n0.getAt(1) * b[0];

        float radiusActual = 0.0f; //smallest distance of point p to cylinder axis

        radiusActual = qSqrt( n0CrossB[0]*n0CrossB[0] + n0CrossB[1]*n0CrossB[1] + n0CrossB[2]*n0CrossB[2] );

        float distance = 0.0f;

        distance = radiusActual - PS_CylinderSegment::X0.getAt(0); //distance error

        sumVV += distance * distance;

    }

    this->myCylinderState->radius = PS_CylinderSegment::X0.getAt(0);
    this->myCylinderState->xyz[0] = PS_CylinderSegment::X0.getAt(1);
    this->myCylinderState->xyz[1] = PS_CylinderSegment::X0.getAt(2);
    this->myCylinderState->alpha = PS_CylinderSegment::X0.getAt(3);
    this->myCylinderState->beta = PS_CylinderSegment::X0.getAt(4);
    this->myState->mainFocus[0] = centroid[0];
    this->myState->mainFocus[1] = centroid[1];
    this->myState->mainFocus[2] = centroid[2];
    this->myState->sigma = qSqrt( sumVV / ((double)this->myState->myPoints.size() - 5.0) );
    this->myState->isValid = true;

}

/*!
 * \brief PS_CylinderSegment::minimumSolution
 * Calculate cylinder from 9 or more points
 * \param points
 */
void PS_CylinderSegment::minimumSolution(QList<Point_PC *> points){

    if(points.size() < 9){
        this->myState->isValid = false;
        return;
    }

    int numPoints = points.size();

    //calc centroid
    double centroid[3];
    centroid[0] = 0.0;
    centroid[1] = 0.0;
    centroid[2] = 0.0;
    for(int i = 0; i < numPoints; i++){
        Point_PC *p = points.at(i);
        centroid[0] += p->xyz[0];
        centroid[1] += p->xyz[1];
        centroid[2] += p->xyz[2];
    }
    centroid[0] = centroid[0] / (float)numPoints;
    centroid[1] = centroid[1] / (float)numPoints;
    centroid[2] = centroid[2] / (float)numPoints;

    //centroid of transformed 2D (x,y) coordinates
    double centroid2D[2];

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            PS_CylinderSegment::H.setAt(i, j, 0.0);
        }
    }

    for (int k = 0; k < numPoints; k++) {
        Point_PC *p = points.at(k);
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                double a = 0.0, b = 0.0;
                if (i == 0)
                    a = p->xyz[0] - centroid[0];
                else if (i == 1)
                    a = p->xyz[1] - centroid[1];
                else
                    a = p->xyz[2] - centroid[2];
                if (j == 0)
                    b = p->xyz[0] - centroid[0];
                else if (j == 1)
                    b = p->xyz[1] - centroid[1];
                else
                    b = p->xyz[2] - centroid[2];
                PS_CylinderSegment::H.setAt(i,j, PS_CylinderSegment::H.getAt(i,j) + a * b);
            }
        }
    }


    try{
        PS_CylinderSegment::H.svd(PS_CylinderSegment::u, PS_CylinderSegment::d, PS_CylinderSegment::v);
    }catch(exception &e){
        qDebug() << "SVD error cylinder minimum solution: " << e.what();
        this->myState->isValid = false;
        return;
    }

    //final solutions of cylinder approximation
    double alpha_n = 0.0, beta_n = 0.0, radius_n = 0.0, x_m_n = 0.0, y_m_n = 0.0;

    double vv_ref = numeric_limits<double>::max();//1000.0;

    OiVec pn; //possible normal vector
    double a = 0.0, b = 0.0; //sin + cos of rotation angles
    double a_alpha = 0.0, b_alpha = 0.0, a_beta = 0.0, b_beta = 0.0; //possible rotation angles (check acos + asin)
    double alpha = 0.0, beta = 0.0; //rotation angles
    double _y = 0.0, _z = 0.0;
    double tx = 0.0, ty = 0.0; //transformed 2D coordinates
    double x_m = 0.0, y_m = 0.0, radius = 0.0, sum_vv = 0.0; //result parameters in circle fit
    OiVec v;

    //a matrices for circle adjustment
    OiVec A1(numPoints);
    OiMat A2(numPoints,3);

    //result vector for circle fit
    OiVec s(3);

    //rotated 2D points
    QList<float> points2D_x;
    QList<float> points2D_y;

    //rotated centroid reduced 2D points
    QList<float> centroidreduced2D_x;
    QList<float> centroidreduced2D_y;

    //one of the eigen-vectors is the approximate cylinder axis
    for(int i = 0; i < 3; i++){

        pn = PS_CylinderSegment::u.getCol(i); //Eigenvektor

        //calculate rotations angles
        a = qSqrt(1.0 / (1.0 + (pn.getAt(2)/pn.getAt(1))*(pn.getAt(2)/pn.getAt(1))));
        b = a * pn.getAt(2) / pn.getAt(1);

        b_alpha = qAcos(b);
        a_alpha = qAsin(a);

        alpha = 0.0;

        //cos und sin sind mehrdeutig, deswegen hier den richtigen Winkel checken
        if(this->compareAngles(b_alpha, a_alpha)){
            alpha = a_alpha;
        }else if(this->compareAngles(this->getCorrespondingCos(b_alpha), a_alpha)){
            alpha = a_alpha;
        }else if(this->compareAngles(b_alpha, this->getCorrespondingSin(a_alpha))){
            alpha = b_alpha;
        }else{
            alpha = this->getCorrespondingCos(b_alpha);
        }

        //Eigenvektor transformieren --> y-Komponente sollte jetzt 0 sein
        _y = pn.getAt(1);
        _z = pn.getAt(2);
        pn.setAt(1, _y * qCos(alpha) - _z * qSin(alpha));
        pn.setAt(2, _y * qSin(alpha) + _z * qCos(alpha));

        a = 0.0, b = 0.0;
        a = qSqrt(1.0 / (1.0 + (-1.0 * pn.getAt(2) / pn.getAt(0))*(-1.0 * pn.getAt(2) / pn.getAt(0))));
        b = -1.0 * pn.getAt(2) * a / pn.getAt(0);

        b_beta = qAcos(b);
        a_beta = qAsin(a);

        beta = 0.0;

        //cos und sin sind mehrdeutig, deswegen hier den richtigen Winkel checken
        if(this->compareAngles(b_beta, a_beta)){
            beta = b_beta;
        }else if(this->compareAngles(this->getCorrespondingCos(b_beta), a_beta)){
            beta = a_beta;
        }else if(this->compareAngles(b_beta, this->getCorrespondingSin(a_beta))){
            beta = b_beta;
        }else{
            beta = this->getCorrespondingCos(b_beta);
        }

        //set up rotations matrices
        PS_CylinderSegment::Ralpha.setAt(0, 0, 1.0);
        PS_CylinderSegment::Ralpha.setAt(1, 1, qCos(alpha));
        PS_CylinderSegment::Ralpha.setAt(1, 2, -qSin(alpha));
        PS_CylinderSegment::Ralpha.setAt(2, 1, qSin(alpha));
        PS_CylinderSegment::Ralpha.setAt(2, 2, qCos(alpha));
        PS_CylinderSegment::Rbeta.setAt(0, 0, qCos(beta));
        PS_CylinderSegment::Rbeta.setAt(0, 2, qSin(beta));
        PS_CylinderSegment::Rbeta.setAt(1, 1, 1.0);
        PS_CylinderSegment::Rbeta.setAt(2, 0, -qSin(beta));
        PS_CylinderSegment::Rbeta.setAt(2, 2, qCos(beta));

        PS_CylinderSegment::Rall = PS_CylinderSegment::Rbeta * PS_CylinderSegment::Ralpha;

        //circle fit to determine midpoint and radius

        //clear transformed point lists
        points2D_x.clear();
        points2D_y.clear();
        centroidreduced2D_x.clear();
        centroidreduced2D_y.clear();

        //reset 2D centroid and recalculate with new rotation
        centroid2D[0] = 0.0;
        centroid2D[1] = 0.0;

        for(int j = 0; j < numPoints; j++){
            Point_PC *p = points.at(j);

            tx = PS_CylinderSegment::Rall.getAt(0,0)*p->xyz[0]
                    + PS_CylinderSegment::Rall.getAt(0,1)*p->xyz[1]
                    + PS_CylinderSegment::Rall.getAt(0,2)*p->xyz[2];
            ty = PS_CylinderSegment::Rall.getAt(1,0)*p->xyz[0]
                    + PS_CylinderSegment::Rall.getAt(1,1)*p->xyz[1]
                    + PS_CylinderSegment::Rall.getAt(1,2)*p->xyz[2];

            points2D_x.append( tx );
            points2D_y.append( ty );

            centroid2D[0] += tx;
            centroid2D[1] += ty;
        }
        centroid2D[0] = centroid2D[0] / (float)numPoints;
        centroid2D[1] = centroid2D[1] / (float)numPoints;

        for(int j = 0; j < numPoints; j++){
            centroidreduced2D_x.append( points2D_x.at(j) - centroid2D[0] );
            centroidreduced2D_y.append( points2D_y.at(j) - centroid2D[1] );
        }

        //fill a matrices
        for(int j = 0; j < numPoints; j++){
            tx = centroidreduced2D_x.at(j);
            ty = centroidreduced2D_y.at(j);

            A1.setAt(j, tx*tx + ty*ty);

            A2.setAt(j,0,tx);
            A2.setAt(j,1,ty);
            A2.setAt(j,2,1.0);
        }

        try{
            if(!OiMat::solve(s, A2.t() * A2, -1.0 * A2.t() * A1)){
                this->myState->isValid = false;
                return;
            }
            //s = -1.0 * (A2.t() * A2).inv() * A2.t() * A1;
        }catch(const exception &e){
            qDebug() << "inv error cylinder minimum solution: " << e.what();
            this->myState->isValid = false;
            return;
        }

        //midpoint + radius
        x_m = (-1.0 * s.getAt(0) / 2.0) + centroid2D[0];
        y_m = (-1.0 * s.getAt(1) / 2.0) + centroid2D[1];
        radius = qSqrt(0.25 * (s.getAt(0) * s.getAt(0) + s.getAt(1) * s.getAt(1)) - s.getAt(2));

        v = -1.0 * A1 - A2 * s;
        sum_vv = OiVec::dot(v,v);
        sum_vv = qSqrt(sum_vv / (numPoints-3.0));

        //wenn diese Lösung besser als die bisher beste Lösung ist
        if(sum_vv < vv_ref){
            alpha_n = alpha;
            beta_n = beta;
            x_m_n = -1.0 * x_m;
            y_m_n = -1.0 * y_m;
            radius_n = radius;
            vv_ref = sum_vv;
        }

    }

    //set the cylinder's attributes to the calculated values
    this->myCylinderState->radius = radius_n;
    this->myCylinderState->xyz[0] = x_m_n;
    this->myCylinderState->xyz[1] = y_m_n;
    this->myCylinderState->alpha = alpha_n;
    this->myCylinderState->beta = beta_n;
    this->myState->isValid = true;

}

/*!
 * \brief PS_CylinderSegment::sortOut
 * Only include points in the cylinder that are within a distance around the cylinder surface (specified by the user)
 * \param myCylinder
 * \param param
 */
void PS_CylinderSegment::sortOut(PS_CylinderSegment *myCylinder, PS_InputParameter param){

    QList<Point_PC *> myPoints = myCylinder->getPoints();
    myCylinder->removeAllPoints();
    PS_CylinderSegment::checkPointsInCylinder(myCylinder, myPoints, param, 1);

}

/*!
 * \brief PS_CylinderSegment::mergeCylinders
 * \param detectedCylinders
 * \param mergedCylinders
 * \param param
 */
void PS_CylinderSegment::mergeCylinders(const QList<PS_CylinderSegment *> &detectedCylinders, QList<PS_CylinderSegment *> &mergedCylinders, PS_InputParameter param){

    float diffAlpha = 0.0, diffBeta = 0.0; //rotation angle differences
    float diffXYZ = 0.0; //distance of the 2D centroid of one cylinder to the other cylinder
    float diffRadius = 0.0; //radius difference

    if(mergedCylinders.size() != 0){
        mergedCylinders.clear();
    }

    //for each cylinder save its merged state
    QMap<int, bool> mergeMap;
    for(int i = 0; i < detectedCylinders.size(); i++){
        mergeMap.insert(i, false);
    }

    for(int i = 0; i < detectedCylinders.size(); i++){

        qDebug() << i;

        //if the cylinder at position i was not merged before
        if(mergeMap.value(i) == false){

            mergeMap.insert(i, true);

            PS_CylinderSegment *c1 = detectedCylinders.at(i);

            //compare all cylinder's params and merge them if they fit together
            for(int j = i+1; j < detectedCylinders.size(); j++){

                //if the cylinder at position j was not merged before
                if(mergeMap.value(j) == false){

                    PS_CylinderSegment *c2 = detectedCylinders.at(j);

                    diffAlpha = qAbs(c2->getAlpha() - c1->getAlpha());
                    diffBeta = qAbs(c2->getBeta() - c1->getBeta());
                    diffXYZ = qSqrt( (c2->getXYZ()[0]-c1->getXYZ()[0])*(c2->getXYZ()[0]-c1->getXYZ()[0])
                            + (c2->getXYZ()[1]-c1->getXYZ()[1])*(c2->getXYZ()[1]-c1->getXYZ()[1]) );
                    diffRadius = qAbs(c2->getRadius() - c1->getRadius());

                    //if the parameter-differences are under a threshold
                    if(diffXYZ < 10.0 && diffRadius < 10.0*param.cylinderParams.maxDistance
                            && diffAlpha < 0.4 && diffBeta < 0.4){

                        PS_CylinderSegment *mergedCylinder = new PS_CylinderSegment();
                        mergedCylinder->setIsValid(true);
                        foreach(Point_PC *myPoint, c1->getPoints()){
                            myPoint->isUsed = false;
                            mergedCylinder->addPoint(myPoint);
                        }
                        foreach(Point_PC *myPoint, c2->getPoints()){
                            myPoint->isUsed = false;
                            mergedCylinder->addPoint(myPoint);
                        }

                        //set approximate values
                        mergedCylinder->setApproximation(c1->getAlpha(), c1->getBeta(), c1->getRadius(), c1->getXYZ()[0], c1->getXYZ()[1]);

                        mergedCylinder->fitBySample(param.fitSampleSize);

                        if(mergedCylinder->getIsValid() && mergedCylinder->getSigma() <= param.cylinderParams.maxDistance
                                && mergedCylinder->getRadius() >= param.cylinderParams.minRadius
                                && mergedCylinder->getRadius() <= param.cylinderParams.maxRadius){

                            //sort out points that do not satisfy the distance criterion and refit
                            PS_CylinderSegment::sortOut(mergedCylinder, param);
                            mergedCylinder->fitBySample(param.fitSampleSize);

                            if(mergedCylinder->getIsValid() && mergedCylinder->getPoints().size() >= param.cylinderParams.minPoints){

                                //delete single cylinders
                                delete c1;
                                delete c2;

                                //set the planes to be merged
                                mergeMap.insert(j, true);

                                c1 = mergedCylinder;

                            }else{
                                delete mergedCylinder;
                            }

                        }else{
                            delete mergedCylinder;
                        }

                    }

                }

            }

            //add the cylinder to result list
            mergedCylinders.append(c1);

        }

    }

}

/*!
 * \brief PS_CylinderSegment::getRadius
 * \return
 */
float PS_CylinderSegment::getRadius(){
    return this->myCylinderState->radius;
}

/*!
 * \brief PS_CylinderSegment::getXYZ
 * \return
 */
float *PS_CylinderSegment::getXYZ(){
    return this->myCylinderState->xyz;
}

/*!
 * \brief PS_CylinderSegment::getAlpha
 * \return
 */
float PS_CylinderSegment::getAlpha(){
    return this->myCylinderState->alpha;
}

/*!
 * \brief PS_CylinderSegment::getBeta
 * \return
 */
float PS_CylinderSegment::getBeta(){
    return this->myCylinderState->beta;
}

/*!
 * \brief PS_CylinderSegment::setApproximation
 * \param alpha
 * \param beta
 * \param radius
 * \param x
 * \param y
 */
void PS_CylinderSegment::setApproximation(float alpha, float beta, float radius, float x, float y)
{
    this->myCylinderState->alpha = alpha;
    this->myCylinderState->beta = beta;
    this->myCylinderState->xyz[0] = x;
    this->myCylinderState->xyz[1] = y;
    this->myCylinderState->radius = radius;
}

/*!
 * \brief PS_CylinderSegment::compareAngles
 * \param a
 * \param b
 * \return
 */
bool PS_CylinderSegment::compareAngles(double a, double b){
    double t = 2.0*PS_PI;

    //a und b zwischen 0 und 2*PI
    while(a < 0.0){
        a += t;
    }
    while(a > t){
        a -= t;
    }
    while(b < 0.0){
        b += t;
    }
    while(b > t){
        b -= t;
    }

    double diff = a - b;
    if(diff < 0.001 && diff > -0.001){
        return true;
    }
    return false;
}

/*!
 * \brief PS_CylinderSegment::getCorrespondingCos
 * \param a
 * \return
 */
double PS_CylinderSegment::getCorrespondingCos(double a){
    while(a < 0.0){
        a += 2.0*PS_PI;
    }
    while(a > 2.0*PS_PI){
        a -= 2.0*PS_PI;
    }
    return 2.0 * PS_PI - a;
}

/*!
 * \brief PS_CylinderSegment::getCorrespondingSin
 * \param a
 * \return
 */
double PS_CylinderSegment::getCorrespondingSin(double a){
    while(a < 0.0){
        a += 2.0*PS_PI;
    }
    while(a > 2.0*PS_PI){
        a -= 2.0*PS_PI;
    }
    if(a <= PS_PI){
        return PS_PI - a;
    }else{
        return 3.0*PS_PI - a;
    }
}
