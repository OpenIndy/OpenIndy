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

OiMat PS_CylinderSegment::verify_u(3,3);
OiMat PS_CylinderSegment::verify_v(3,3);
OiVec PS_CylinderSegment::verify_d(3);

PS_CylinderSegment::PS_CylinderSegment(){

    //create cylinder states and make sure that myCylinderState points to the same object as myState
    this->myCylinderState = new CylinderState();
    this->myOldState = new CylinderState();
    this->myState = this->myCylinderState;

}

PS_CylinderSegment::~PS_CylinderSegment(){
}

/*!
 * \brief PS_CylinderSegment::writeToX3D
 * Write an x3d - file with the calculated cylinder
 * \param filePath
 * \return
 */
bool PS_CylinderSegment::writeToX3D(const QString &filePath){

    return false;

}

/*!
 * \brief PS_CylinderSegment::detectCylinder
 * Tries to detect a single cylinder in the given point material that contains maximum points that satisfy the distance criterion
 * \param points
 * \param param
 * \return
 */
PS_CylinderSegment *PS_CylinderSegment::detectCylinder(const QList<PS_Point_PC*> &points, const PS_InputParameter &param){

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
    numTrials = (int)ceil(qLn(1.0f - s) / qLn(1.0f - phk));

    //get numTrials random samples
    QMap<int, QList<PS_Point_PC*> > randomSamples;
    PS_GeneralMath::getRandomSubsets(randomSamples, numTrials, 9, points);

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
        if(PS_CylinderSegment::checkPointsInCylinder(possibleSolution, points, param, 1) >= 6){

            //...and the solution is better then the best one found before
            if(possibleSolution->getPoints().size() > result->getPoints().size()){

                //delete the old "best" cylinder
                if(result != NULL){
                    delete result;
                }

                //set the new cylinder as result
                result = possibleSolution;

                //if the cylinder contains mostly all points of the voxel then break the search
                if(result->getPoints().size() >= points.size()-5){
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
int PS_CylinderSegment::checkPointsInCylinder(PS_CylinderSegment *myCylinder, const QList<PS_Point_PC*> &myPoints, const PS_InputParameter &param, const int &toleranceFactor){

    int result = 0; //number of points that were added to the cylinder

    if(myCylinder->getIsValid()){

        //get normal vector and 3D point on the cylinder axis
        /*float ijk[3], x0[3];
        myCylinder->getX0(x0);
        myCylinder->getIJK(ijk);*/

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

        PS_CylinderSegment::n0.setAt(0, 0.0);
        PS_CylinderSegment::n0.setAt(1, 0.0);
        PS_CylinderSegment::n0.setAt(2, 1.0);
        OiMat::solve(PS_CylinderSegment::n0, PS_CylinderSegment::Rall, PS_CylinderSegment::n0);

        //iterate through all points to check wether they lie in a small band around the cylinder surface
        for(unsigned int i = 0; i < myPoints.size(); i++){

            PS_Point_PC *p = myPoints.at(i);

            //if the point is not used for another shape
            if(!p->isUsed){

                float b[3]; //vector between point on cylinder axis and point p which is probably on cylinder
                b[0] = p->xyz[0] - PS_CylinderSegment::x_m_n.getAt(0);
                b[1] = p->xyz[1] - PS_CylinderSegment::x_m_n.getAt(1);
                b[2] = p->xyz[2] - PS_CylinderSegment::x_m_n.getAt(2);

                float n0CrossB[3]; //cross product of cylinder axis (length 1) and b
                n0CrossB[0] = PS_CylinderSegment::n0.getAt(1) * b[2] - PS_CylinderSegment::n0.getAt(2) * b[1];
                n0CrossB[1] = PS_CylinderSegment::n0.getAt(2) * b[0] - PS_CylinderSegment::n0.getAt(0) * b[2];
                n0CrossB[2] = PS_CylinderSegment::n0.getAt(0) * b[1] - PS_CylinderSegment::n0.getAt(1) * b[0];

                /*float b[3]; //vector between point on cylinder axis and point p which is probably on cylinder
                b[0] = p->xyz[0] - x0[0];
                b[1] = p->xyz[1] - x0[1];
                b[2] = p->xyz[2] - x0[2];

                float n0CrossB[3]; //cross product of cylinder axis (length 1) and b
                n0CrossB[0] = ijk[1] * b[2] - ijk[2] * b[1];
                n0CrossB[1] = ijk[2] * b[0] - ijk[0] * b[2];
                n0CrossB[2] = ijk[0] * b[1] - ijk[1] * b[0];*/

                /*PS_CylinderSegment::Ralpha.setAt(0, 0, 1.0);
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

                PS_CylinderSegment::n0.setAt(0, 0.0);
                PS_CylinderSegment::n0.setAt(1, 0.0);
                PS_CylinderSegment::n0.setAt(2, 1.0);
                OiMat::solve(PS_CylinderSegment::n0, PS_CylinderSegment::Rall, PS_CylinderSegment::n0);

                float b[3]; //vector between point on cylinder axis and point p which is probably on cylinder
                b[0] = p->xyz[0] - PS_CylinderSegment::x_m_n.getAt(0);
                b[1] = p->xyz[1] - PS_CylinderSegment::x_m_n.getAt(1);
                b[2] = p->xyz[2] - PS_CylinderSegment::x_m_n.getAt(2);

                float n0CrossB[3]; //cross product of cylinder axis (length 1) and b
                n0CrossB[0] = PS_CylinderSegment::n0.getAt(1) * b[2] - PS_CylinderSegment::n0.getAt(2) * b[1];
                n0CrossB[1] = PS_CylinderSegment::n0.getAt(2) * b[0] - PS_CylinderSegment::n0.getAt(0) * b[2];
                n0CrossB[2] = PS_CylinderSegment::n0.getAt(0) * b[1] - PS_CylinderSegment::n0.getAt(1) * b[0];*/

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

    //qDebug() << "fit";

    //clock_t test = clock();

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
    //OiVec L(numPoints*3); //observations
    OiVec v(numPoints*3); //approximation of corrections
    OiVec L0(numPoints*3); //L + v
    //OiMat B(numPoints, 3*numPoints); //derivations with respect to (L+v0)
    //OiMat A(numPoints, 5); //derivations with respect to unknowns
    OiVec c(numPoints+5); //right side
    //OiMat BBT(numPoints, numPoints);
    //OiMat AT(5, numPoints);
    OiMat N(numPoints+5, numPoints+5); //normal equation matrix
    OiVec res(numPoints+5); //adjustment result
    //OiVec k(numPoints);
    double _r = 0.0, _X0 = 0.0, _Y0 = 0.0, _alpha = 0.0, _beta = 0.0;
    double _r_armijo = 0.0, _X0_armijo = 0.0, _Y0_armijo = 0.0, _alpha_armijo = 0.0, _beta_armijo = 0.0;
    double _x = 0.0, _y = 0.0, _z = 0.0;
    double a1 = 0.0, a2 = 0.0;
    double diff = 0.0, _xr = 0.0, _yr = 0.0;
    double sigma = 2.0;

    //set approximations of unknowns
    _r = this->getRadius();
    _X0 = this->getXYZ()[0];
    _Y0 = this->getXYZ()[1];
    _alpha = this->getAlpha();
    _beta = this->getBeta();
    for(int i = 0; i < 5; i++){
        PS_CylinderSegment::x.setAt(i, 0.0);
    }

    //fill L vector
    for(int i = 0; i < numPoints; i++){
        L0.setAt(i*3, this->myState->myPoints.at(i)->xyz[0]);
        L0.setAt(i*3+1, this->myState->myPoints.at(i)->xyz[1]);
        L0.setAt(i*3+2, this->myState->myPoints.at(i)->xyz[2]);

        centroid[0] += this->myState->myPoints.at(i)->xyz[0];
        centroid[1] += this->myState->myPoints.at(i)->xyz[1];
        centroid[2] += this->myState->myPoints.at(i)->xyz[2];
    }
    //L0 = L;
    centroid[0] = centroid[0] / (double)numPoints;
    centroid[1] = centroid[1] / (double)numPoints;
    centroid[2] = centroid[2] / (double)numPoints;

    int numIterations = 0;

    //qDebug() << "vor it " << (clock() - test)/(double)CLOCKS_PER_SEC;

    double stopAA = 0.0, stopBB = 0.0, stopXX = 0.0;
    do{

        //qDebug() << "it i " << (clock() - test)/(double)CLOCKS_PER_SEC;

        //improve unknowns
        _r += PS_CylinderSegment::x.getAt(0);
        _X0 += PS_CylinderSegment::x.getAt(1);
        _Y0 += PS_CylinderSegment::x.getAt(2);
        _alpha += PS_CylinderSegment::x.getAt(3);
        _beta += PS_CylinderSegment::x.getAt(4);

        //improve observations
        L0 = L0 + v;

        //build rotations matrices
        PS_CylinderSegment::Ralpha.setAt(0, 0, 1.0);
        PS_CylinderSegment::Ralpha.setAt(1, 1, qCos(_alpha));
        PS_CylinderSegment::Ralpha.setAt(1, 2, -qSin(_alpha));
        PS_CylinderSegment::Ralpha.setAt(2, 1, qSin(_alpha));
        PS_CylinderSegment::Ralpha.setAt(2, 2, qCos(_alpha));
        PS_CylinderSegment::Rbeta.setAt(0, 0, qCos(_beta));
        PS_CylinderSegment::Rbeta.setAt(0, 2, qSin(_beta));
        PS_CylinderSegment::Rbeta.setAt(1, 1, 1.0);
        PS_CylinderSegment::Rbeta.setAt(2, 0, -qSin(_beta));
        PS_CylinderSegment::Rbeta.setAt(2, 2, qCos(_beta));

        PS_CylinderSegment::Rall = PS_CylinderSegment::Rbeta * PS_CylinderSegment::Ralpha;

        //qDebug() << "a und b " << (clock() - test)/(double)CLOCKS_PER_SEC;

        //fill A and B matrix + w vector + rechte Seite
        for(int i = 0; i < numPoints; i++){

            _x = L0.getAt(i*3);
            _y = L0.getAt(i*3+1);
            _z = L0.getAt(i*3+2);

            a1 = _X0 + _x * qCos(_beta) + _y * qSin(_alpha) * qSin(_beta) + _z * qCos(_alpha) * qSin(_beta);
            a2 = _Y0 + _y * qCos(_alpha) - _z * qSin(_alpha);

            //B.setAt(i, i*3, -1.0 * qCos(_beta) * a1 / qSqrt(a1*a1 + a2*a2)); // x
            //B.setAt(i, i*3+1, -1.0 * (qSin(_alpha) * qSin(_beta) * a1 + qCos(_alpha) * a2) / qSqrt(a1*a1 + a2*a2)); // y
            //B.setAt(i, i*3+2, -1.0 * (qCos(_alpha) * qSin(_beta) * a1 - qSin(_alpha) * a2) / qSqrt(a1*a1 + a2*a2)); // z

            //A
            N.setAt(i, numPoints, 1.0);
            N.setAt(i, numPoints+1, -1.0 * a1 / qSqrt(a1*a1 + a2*a2));
            N.setAt(i, numPoints+2, -1.0 * a2 / qSqrt(a1*a1 + a2*a2));
            N.setAt(i, numPoints+3, -1.0 * ((_y * qSin(_beta) * qCos(_alpha) - _z * qSin(_beta) * qSin(_alpha)) * a1 - (_y * qSin(_alpha) + _z * qCos(_alpha)) * a2) / qSqrt(a1*a1 + a2*a2));
            N.setAt(i, numPoints+4, -1.0 * (_y * qSin(_alpha) * qCos(_beta) - _x * qSin(_beta) + _z * qCos(_alpha) * qCos(_beta)) * a1 / qSqrt(a1*a1 + a2*a2));

            //AT
            N.setAt(numPoints, i, 1.0);
            N.setAt(numPoints+1, i, -1.0 * a1 / qSqrt(a1*a1 + a2*a2));
            N.setAt(numPoints+2, i, -1.0 * a2 / qSqrt(a1*a1 + a2*a2));
            N.setAt(numPoints+3, i, -1.0 * ((_y * qSin(_beta) * qCos(_alpha) - _z * qSin(_beta) * qSin(_alpha)) * a1 - (_y * qSin(_alpha) + _z * qCos(_alpha)) * a2) / qSqrt(a1*a1 + a2*a2));
            N.setAt(numPoints+4, i, -1.0 * (_y * qSin(_alpha) * qCos(_beta) - _x * qSin(_beta) + _z * qCos(_alpha) * qCos(_beta)) * a1 / qSqrt(a1*a1 + a2*a2));

            //BBT
            N.setAt(i, i, (-1.0 * qCos(_beta) * a1 / qSqrt(a1*a1 + a2*a2))*(-1.0 * qCos(_beta) * a1 / qSqrt(a1*a1 + a2*a2))
                        + (-1.0 * (qSin(_alpha) * qSin(_beta) * a1 + qCos(_alpha) * a2) / qSqrt(a1*a1 + a2*a2))*(-1.0 * (qSin(_alpha) * qSin(_beta) * a1 + qCos(_alpha) * a2) / qSqrt(a1*a1 + a2*a2))
                        + (-1.0 * (qCos(_alpha) * qSin(_beta) * a1 - qSin(_alpha) * a2) / qSqrt(a1*a1 + a2*a2))*(-1.0 * (qCos(_alpha) * qSin(_beta) * a1 - qSin(_alpha) * a2) / qSqrt(a1*a1 + a2*a2)));


            //A.setAt(i, 0, 1.0); //r
            //A.setAt(i, 1, -1.0 * a1 / qSqrt(a1*a1 + a2*a2)); // X0
            //A.setAt(i, 2, -1.0 * a2 / qSqrt(a1*a1 + a2*a2)); // Y0
            //A.setAt(i, 3, -1.0 * ((_y * qSin(_beta) * qCos(_alpha) - _z * qSin(_beta) * qSin(_alpha)) * a1 - (_y * qSin(_alpha) + _z * qCos(_alpha)) * a2) / qSqrt(a1*a1 + a2*a2)); // alpha
            //A.setAt(i, 4, -1.0 * (_y * qSin(_alpha) * qCos(_beta) - _x * qSin(_beta) + _z * qCos(_alpha) * qCos(_beta)) * a1 / qSqrt(a1*a1 + a2*a2)); // beta

            _xr = PS_CylinderSegment::Rall.getAt(0,0) * _x + PS_CylinderSegment::Rall.getAt(0,1) * _y + PS_CylinderSegment::Rall.getAt(0,2) * _z + X0.getAt(1);
            _yr = PS_CylinderSegment::Rall.getAt(1,0) * _x + PS_CylinderSegment::Rall.getAt(1,1) * _y + PS_CylinderSegment::Rall.getAt(1,2) * _z + X0.getAt(2);

            //genäherter Radius des Zylinders minus Abstand Punkt i zur z-Achse ist der Widerspruch
            diff = _r - qSqrt(_xr*_xr + _yr*_yr);

            c.setAt(i, diff);

        }

        //qDebug() << "vor bbt " << (clock() - test)/(double)CLOCKS_PER_SEC;



        //build normal equation
        //BBT = B * B.t();
        //AT = A.t();
        /*for(int i = 0; i < (numPoints+5); i++){
            for(int j = 0; j < (numPoints+5); j++){
                if(i < numPoints && j < numPoints){
                    //N.setAt(i, j, BBT.getAt(i, j));
                }else if(i < numPoints && j >= numPoints){
                    //N.setAt(i, j, A.getAt(i, (j-numPoints)));
                }else if(i >= numPoints && j < numPoints){
                    //N.setAt(i, j, AT.getAt((i-numPoints), j));
                }else if(i >= numPoints && j >= numPoints){
                    N.setAt(i, j, 0.0);
                }
            }
        }*/

        //qDebug() << "vor solve " << (clock() - test)/(double)CLOCKS_PER_SEC;

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

        //qDebug() << "nach solve " << (clock() - test)/(double)CLOCKS_PER_SEC;

        //get results
        /*for(int i = 0; i < numPoints; i++){
            k.setAt(i, res.getAt(i));
        }*/
        for(int i = numPoints; i < numPoints+5; i++){
            PS_CylinderSegment::x.setAt(i-numPoints, res.getAt(i));
        }
        //v = B.t() * k;

        for(int i = 0; i < numPoints; ++i){

            _x = this->getPoints().at(i)->xyz[0];
            _y = this->getPoints().at(i)->xyz[1];
            _z = this->getPoints().at(i)->xyz[2];

            a1 = _X0 + _x * qCos(_beta) + _y * qSin(_alpha) * qSin(_beta) + _z * qCos(_alpha) * qSin(_beta);
            a2 = _Y0 + _y * qCos(_alpha) - _z * qSin(_alpha);

            //B.setAt(i, i*3, -1.0 * qCos(_beta) * a1 / qSqrt(a1*a1 + a2*a2)); // x
            //B.setAt(i, i*3+1, -1.0 * (qSin(_alpha) * qSin(_beta) * a1 + qCos(_alpha) * a2) / qSqrt(a1*a1 + a2*a2)); // y
            //B.setAt(i, i*3+2, -1.0 * (qCos(_alpha) * qSin(_beta) * a1 - qSin(_alpha) * a2) / qSqrt(a1*a1 + a2*a2)); // z

            v.setAt(i*3, (-1.0 * qCos(_beta) * a1 / qSqrt(a1*a1 + a2*a2)) * res.getAt(i));
            v.setAt(i*3+1, (-1.0 * (qSin(_alpha) * qSin(_beta) * a1 + qCos(_alpha) * a2) / qSqrt(a1*a1 + a2*a2)) * res.getAt(i));
            v.setAt(i*3+2, (-1.0 * (qCos(_alpha) * qSin(_beta) * a1 - qSin(_alpha) * a2) / qSqrt(a1*a1 + a2*a2)) * res.getAt(i));



            /*vx = verb.getAt(i*3);
            vy = verb.getAt(i*3+1);
            vz = verb.getAt(i*3+2);

            double r0 = qSqrt( (x + vx - xm) * (x + vx - xm)
                               + (y + vy - ym) * (y + vy - ym)
                               + (z + vz - zm) * (z + vz - zm) );

            a1 = (x + vx - xm) / r0;
            a2 = (y + vy - ym) / r0;
            a3 = (z + vz - zm) / r0;

            verb.setAt(i*3, vx + res.getAt(i) * a1);
            verb.setAt(i*3+1, vy + res.getAt(i) * a2);
            verb.setAt(i*3+2, vz + res.getAt(i) * a3);*/

        }


        //s0 a posteriori
        //double sigma = 0.0;
        //sigma = qSqrt((v.t() * v).getAt(0) / 13.0);

        //qDebug() << "loop e: " << 1000.0 * ((clock() - test)/(double)CLOCKS_PER_SEC) << " msec." << endl;

        //qDebug() << "vor armijo " << (clock() - test)/(double)CLOCKS_PER_SEC;

        //Armijo Regel
        do{

            sigma = sigma / 2.0;

            _r_armijo = _r + sigma * PS_CylinderSegment::x.getAt(0);
            _X0_armijo = _X0 + sigma * PS_CylinderSegment::x.getAt(1);
            _Y0_armijo = _Y0 + sigma * PS_CylinderSegment::x.getAt(2);
            _alpha_armijo = _alpha + sigma * PS_CylinderSegment::x.getAt(3);
            _beta_armijo = _beta + sigma * PS_CylinderSegment::x.getAt(4);

            for(int i = 0; i < 5; i++){
                _x = L0.getAt(i*3);
                _y = L0.getAt(i*3+1);
                _z = L0.getAt(i*3+2);

                PS_CylinderSegment::a.setAt(i, _r_armijo - qSqrt( (_X0_armijo + _x*qCos(_beta_armijo) + _y*qSin(_alpha_armijo)*qSin(_beta_armijo) + _z*qCos(_alpha_armijo)*qSin(_beta_armijo))*(_X0_armijo + _x*qCos(_beta_armijo) + _y*qSin(_alpha_armijo)*qSin(_beta_armijo) + _z*qCos(_alpha_armijo)*qSin(_beta_armijo))
                                       + (_Y0_armijo + _y*qCos(_alpha_armijo) - _z*qSin(_alpha_armijo))*(_Y0_armijo + _y*qCos(_alpha_armijo) - _z*qSin(_alpha_armijo)) ));

                PS_CylinderSegment::b.setAt(i, _r - qSqrt( (_X0 + _x*qCos(_beta) + _y*qSin(_alpha)*qSin(_beta) + _z*qCos(_alpha)*qSin(_beta))*(_X0 + _x*qCos(_beta) + _y*qSin(_alpha)*qSin(_beta) + _z*qCos(_alpha)*qSin(_beta))
                                               + (_Y0 + _y*qCos(_alpha) - _z*qSin(_alpha))*(_Y0 + _y*qCos(_alpha) - _z*qSin(_alpha)) ));
            }

        OiVec::dot(stopAA, PS_CylinderSegment::a,PS_CylinderSegment::a);
        OiVec::dot(stopBB, PS_CylinderSegment::b,PS_CylinderSegment::b);

        //qDebug() << "next " << (clock() - test)/(double)CLOCKS_PER_SEC;

        //}while( OiVec::dot(a,a) > 0.5 * OiVec::dot(b,b) );
        }while( stopAA > ( stopBB - 2.0 * 0.001 * sigma * stopBB ) );

        PS_CylinderSegment::x = sigma * PS_CylinderSegment::x;

        //neue Verbesserungen
        //v = B.t() * sigma * k;

        numIterations++;
        OiVec::dot(stopXX, PS_CylinderSegment::x,PS_CylinderSegment::x);

    }while( (stopXX > 0.0000001) && numIterations < 16 );

    //qDebug() << "nach it " << (clock() - test)/(double)CLOCKS_PER_SEC;

    if(numIterations >= 16){
        this->myState->isValid = false;
        return;
    }

    _r += PS_CylinderSegment::x.getAt(0);
    _X0 += PS_CylinderSegment::x.getAt(1);
    _Y0 += PS_CylinderSegment::x.getAt(2);
    _alpha += PS_CylinderSegment::x.getAt(3);
    _beta += PS_CylinderSegment::x.getAt(4);

    //qDebug() << "genauigkeit " << (clock() - test)/(double)CLOCKS_PER_SEC;

    //calc sigma
    PS_CylinderSegment::Ralpha.setAt(0, 0, 1.0);
    PS_CylinderSegment::Ralpha.setAt(1, 1, qCos(_alpha));
    PS_CylinderSegment::Ralpha.setAt(1, 2, -qSin(_alpha));
    PS_CylinderSegment::Ralpha.setAt(2, 1, qSin(_alpha));
    PS_CylinderSegment::Ralpha.setAt(2, 2, qCos(_alpha));
    PS_CylinderSegment::Rbeta.setAt(0, 0, qCos(_beta));
    PS_CylinderSegment::Rbeta.setAt(0, 2, qSin(_beta));
    PS_CylinderSegment::Rbeta.setAt(1, 1, 1.0);
    PS_CylinderSegment::Rbeta.setAt(2, 0, -qSin(_beta));
    PS_CylinderSegment::Rbeta.setAt(2, 2, qCos(_beta));

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

        distance = radiusActual - _r; //distance error

        sumVV += distance * distance;

    }

    //qDebug() << "fertig " << (clock() - test)/(double)CLOCKS_PER_SEC;

    this->myCylinderState->radius = _r;
    this->myCylinderState->xyz[0] = _X0;
    this->myCylinderState->xyz[1] = _Y0;
    this->myCylinderState->alpha = _alpha;
    this->myCylinderState->beta = _beta;
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
void PS_CylinderSegment::fitBySample(int numPoints){

    if(this->myState->myPoints.size() < 6 || numPoints < 6){
        this->myState->isValid = false;
        return;
    }

    //get random subset of numPoints points
    if(numPoints > this->getPoints().size()){
        numPoints = this->getPoints().size();
    }
    QMap<int, QList<PS_Point_PC*> > randomSampleMap;
    PS_GeneralMath::getRandomSubsets(randomSampleMap, 1, numPoints, this->getPoints());
    QList<PS_Point_PC*> &randomSample = randomSampleMap.first();

    if(randomSample.size() == 0){
        this->myState->isValid = false;
        return;
    }

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
    centroid[0] = centroid[0] / (double)this->myState->myPoints.size();
    centroid[1] = centroid[1] / (double)this->myState->myPoints.size();
    centroid[2] = centroid[2] / (double)this->myState->myPoints.size();

    //initialize variables
    //OiVec L(numPoints*3); //observations
    OiVec v(numPoints*3); //approximation of corrections
    OiVec L0(numPoints*3); //L + v
    //OiMat B(numPoints, 3*numPoints); //derivations with respect to (L+v0)
    //OiMat A(numPoints, 5); //derivations with respect to unknowns
    OiVec c(numPoints+5); //right side
    //OiMat BBT(numPoints, numPoints);
    //OiMat AT(5, numPoints);
    OiMat N(numPoints+5, numPoints+5); //normal equation matrix
    OiVec res(numPoints+5); //adjustment result
    //OiVec k(numPoints);
    double _r = 0.0, _X0 = 0.0, _Y0 = 0.0, _alpha = 0.0, _beta = 0.0;
    double _r_armijo = 0.0, _X0_armijo = 0.0, _Y0_armijo = 0.0, _alpha_armijo = 0.0, _beta_armijo = 0.0;
    double _x = 0.0, _y = 0.0, _z = 0.0;
    double a1 = 0.0, a2 = 0.0;
    double diff = 0.0, _xr = 0.0, _yr = 0.0;
    double sigma = 2.0;
    //OiVec ua;

    //set approximations of unknowns
    _r = this->getRadius();
    _X0 = this->getXYZ()[0];
    _Y0 = this->getXYZ()[1];
    _alpha = this->getAlpha();
    _beta = this->getBeta();
    for(int i = 0; i < 5; i++){
        PS_CylinderSegment::x.setAt(i, 0.0);
    }

    //fill L vector
    for(int i = 0; i < numPoints; i++){
        L0.setAt(i*3, randomSample.at(i)->xyz[0]);
        L0.setAt(i*3+1, randomSample.at(i)->xyz[1]);
        L0.setAt(i*3+2, randomSample.at(i)->xyz[2]);
    }

    int numIterations = 0;

    double stopAA = 0.0, stopBB = 0.0, stopXX = 0.0;
    do{

        //improve unknowns
        _r += PS_CylinderSegment::x.getAt(0);
        _X0 += PS_CylinderSegment::x.getAt(1);
        _Y0 += PS_CylinderSegment::x.getAt(2);
        _alpha += PS_CylinderSegment::x.getAt(3);
        _beta += PS_CylinderSegment::x.getAt(4);

        //improve observations
        L0 = L0 + v;

        //build rotations matrices
        PS_CylinderSegment::Ralpha.setAt(0, 0, 1.0);
        PS_CylinderSegment::Ralpha.setAt(1, 1, qCos(_alpha));
        PS_CylinderSegment::Ralpha.setAt(1, 2, -qSin(_alpha));
        PS_CylinderSegment::Ralpha.setAt(2, 1, qSin(_alpha));
        PS_CylinderSegment::Ralpha.setAt(2, 2, qCos(_alpha));
        PS_CylinderSegment::Rbeta.setAt(0, 0, qCos(_beta));
        PS_CylinderSegment::Rbeta.setAt(0, 2, qSin(_beta));
        PS_CylinderSegment::Rbeta.setAt(1, 1, 1.0);
        PS_CylinderSegment::Rbeta.setAt(2, 0, -qSin(_beta));
        PS_CylinderSegment::Rbeta.setAt(2, 2, qCos(_beta));

        PS_CylinderSegment::Rall = PS_CylinderSegment::Rbeta * PS_CylinderSegment::Ralpha;

        //fill A and B matrix + w vector + rechte Seite
        for(int i = 0; i < numPoints; i++){

            _x = L0.getAt(i*3);
            _y = L0.getAt(i*3+1);
            _z = L0.getAt(i*3+2);

            a1 = _X0 + _x * qCos(_beta) + _y * qSin(_alpha) * qSin(_beta) + _z * qCos(_alpha) * qSin(_beta);
            a2 = _Y0 + _y * qCos(_alpha) - _z * qSin(_alpha);

            //A
            N.setAt(i, numPoints, 1.0);
            N.setAt(i, numPoints+1, -1.0 * a1 / qSqrt(a1*a1 + a2*a2));
            N.setAt(i, numPoints+2, -1.0 * a2 / qSqrt(a1*a1 + a2*a2));
            N.setAt(i, numPoints+3, -1.0 * ((_y * qSin(_beta) * qCos(_alpha) - _z * qSin(_beta) * qSin(_alpha)) * a1 - (_y * qSin(_alpha) + _z * qCos(_alpha)) * a2) / qSqrt(a1*a1 + a2*a2));
            N.setAt(i, numPoints+4, -1.0 * (_y * qSin(_alpha) * qCos(_beta) - _x * qSin(_beta) + _z * qCos(_alpha) * qCos(_beta)) * a1 / qSqrt(a1*a1 + a2*a2));

            //AT
            N.setAt(numPoints, i, 1.0);
            N.setAt(numPoints+1, i, -1.0 * a1 / qSqrt(a1*a1 + a2*a2));
            N.setAt(numPoints+2, i, -1.0 * a2 / qSqrt(a1*a1 + a2*a2));
            N.setAt(numPoints+3, i, -1.0 * ((_y * qSin(_beta) * qCos(_alpha) - _z * qSin(_beta) * qSin(_alpha)) * a1 - (_y * qSin(_alpha) + _z * qCos(_alpha)) * a2) / qSqrt(a1*a1 + a2*a2));
            N.setAt(numPoints+4, i, -1.0 * (_y * qSin(_alpha) * qCos(_beta) - _x * qSin(_beta) + _z * qCos(_alpha) * qCos(_beta)) * a1 / qSqrt(a1*a1 + a2*a2));

            //BBT
            N.setAt(i, i, (-1.0 * qCos(_beta) * a1 / qSqrt(a1*a1 + a2*a2))*(-1.0 * qCos(_beta) * a1 / qSqrt(a1*a1 + a2*a2))
                        + (-1.0 * (qSin(_alpha) * qSin(_beta) * a1 + qCos(_alpha) * a2) / qSqrt(a1*a1 + a2*a2))*(-1.0 * (qSin(_alpha) * qSin(_beta) * a1 + qCos(_alpha) * a2) / qSqrt(a1*a1 + a2*a2))
                        + (-1.0 * (qCos(_alpha) * qSin(_beta) * a1 - qSin(_alpha) * a2) / qSqrt(a1*a1 + a2*a2))*(-1.0 * (qCos(_alpha) * qSin(_beta) * a1 - qSin(_alpha) * a2) / qSqrt(a1*a1 + a2*a2)));

            _xr = PS_CylinderSegment::Rall.getAt(0,0) * _x + PS_CylinderSegment::Rall.getAt(0,1) * _y + PS_CylinderSegment::Rall.getAt(0,2) * _z + X0.getAt(1);
            _yr = PS_CylinderSegment::Rall.getAt(1,0) * _x + PS_CylinderSegment::Rall.getAt(1,1) * _y + PS_CylinderSegment::Rall.getAt(1,2) * _z + X0.getAt(2);

            //genäherter Radius des Zylinders minus Abstand Punkt i zur z-Achse ist der Widerspruch
            diff = _r - qSqrt(_xr*_xr + _yr*_yr);

            c.setAt(i, diff);

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
        for(int i = numPoints; i < numPoints+5; i++){
            PS_CylinderSegment::x.setAt(i-numPoints, res.getAt(i));
        }

        for(int i = 0; i < numPoints; ++i){

            _x = this->getPoints().at(i)->xyz[0];
            _y = this->getPoints().at(i)->xyz[1];
            _z = this->getPoints().at(i)->xyz[2];

            a1 = _X0 + _x * qCos(_beta) + _y * qSin(_alpha) * qSin(_beta) + _z * qCos(_alpha) * qSin(_beta);
            a2 = _Y0 + _y * qCos(_alpha) - _z * qSin(_alpha);

            v.setAt(i*3, (-1.0 * qCos(_beta) * a1 / qSqrt(a1*a1 + a2*a2)) * res.getAt(i));
            v.setAt(i*3+1, (-1.0 * (qSin(_alpha) * qSin(_beta) * a1 + qCos(_alpha) * a2) / qSqrt(a1*a1 + a2*a2)) * res.getAt(i));
            v.setAt(i*3+2, (-1.0 * (qCos(_alpha) * qSin(_beta) * a1 - qSin(_alpha) * a2) / qSqrt(a1*a1 + a2*a2)) * res.getAt(i));

        }

        //s0 a posteriori
        //double sigma = 0.0;
        //sigma = qSqrt((v.t() * v).getAt(0) / 13.0);

        //qDebug() << "loop e: " << 1000.0 * ((clock() - test)/(double)CLOCKS_PER_SEC) << " msec." << endl;

        //Armijo Regel
        do{

            sigma = sigma / 2.0;

            _r_armijo = _r + sigma * PS_CylinderSegment::x.getAt(0);
            _X0_armijo = _X0 + sigma * PS_CylinderSegment::x.getAt(1);
            _Y0_armijo = _Y0 + sigma * PS_CylinderSegment::x.getAt(2);
            _alpha_armijo = _alpha + sigma * PS_CylinderSegment::x.getAt(3);
            _beta_armijo = _beta + sigma * PS_CylinderSegment::x.getAt(4);

            for(int i = 0; i < 5; i++){
                _x = L0.getAt(i*3);
                _y = L0.getAt(i*3+1);
                _z = L0.getAt(i*3+2);

                PS_CylinderSegment::a.setAt(i, _r_armijo - qSqrt( (_X0_armijo + _x*qCos(_beta_armijo) + _y*qSin(_alpha_armijo)*qSin(_beta_armijo) + _z*qCos(_alpha_armijo)*qSin(_beta_armijo))*(_X0_armijo + _x*qCos(_beta_armijo) + _y*qSin(_alpha_armijo)*qSin(_beta_armijo) + _z*qCos(_alpha_armijo)*qSin(_beta_armijo))
                                       + (_Y0_armijo + _y*qCos(_alpha_armijo) - _z*qSin(_alpha_armijo))*(_Y0_armijo + _y*qCos(_alpha_armijo) - _z*qSin(_alpha_armijo)) ));

                PS_CylinderSegment::b.setAt(i, _r - qSqrt( (_X0 + _x*qCos(_beta) + _y*qSin(_alpha)*qSin(_beta) + _z*qCos(_alpha)*qSin(_beta))*(_X0 + _x*qCos(_beta) + _y*qSin(_alpha)*qSin(_beta) + _z*qCos(_alpha)*qSin(_beta))
                                               + (_Y0 + _y*qCos(_alpha) - _z*qSin(_alpha))*(_Y0 + _y*qCos(_alpha) - _z*qSin(_alpha)) ));
            }

            OiVec::dot(stopAA, PS_CylinderSegment::a,PS_CylinderSegment::a);
            OiVec::dot(stopBB, PS_CylinderSegment::b,PS_CylinderSegment::b);

        //}while( OiVec::dot(a,a) > 0.5 * OiVec::dot(b,b) );
        }while( stopAA > ( stopBB - 2.0 * 0.001 * sigma * stopBB ) );

        PS_CylinderSegment::x = sigma * PS_CylinderSegment::x;

        //neue Verbesserungen
        //v = B.t() * sigma * k;

        numIterations++;
        OiVec::dot(stopXX, PS_CylinderSegment::x,PS_CylinderSegment::x);

    }while( (stopXX > 0.0000001) && numIterations < 16 );

    if(numIterations >= 16){
        this->myState->isValid = false;
        return;
    }

    _r += PS_CylinderSegment::x.getAt(0);
    _X0 += PS_CylinderSegment::x.getAt(1);
    _Y0 += PS_CylinderSegment::x.getAt(2);
    _alpha += PS_CylinderSegment::x.getAt(3);
    _beta += PS_CylinderSegment::x.getAt(4);

    //calc sigma
    PS_CylinderSegment::Ralpha.setAt(0, 0, 1.0);
    PS_CylinderSegment::Ralpha.setAt(1, 1, qCos(_alpha));
    PS_CylinderSegment::Ralpha.setAt(1, 2, -qSin(_alpha));
    PS_CylinderSegment::Ralpha.setAt(2, 1, qSin(_alpha));
    PS_CylinderSegment::Ralpha.setAt(2, 2, qCos(_alpha));
    PS_CylinderSegment::Rbeta.setAt(0, 0, qCos(_beta));
    PS_CylinderSegment::Rbeta.setAt(0, 2, qSin(_beta));
    PS_CylinderSegment::Rbeta.setAt(1, 1, 1.0);
    PS_CylinderSegment::Rbeta.setAt(2, 0, -qSin(_beta));
    PS_CylinderSegment::Rbeta.setAt(2, 2, qCos(_beta));

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
    /*for(int i = 0; i < this->myState->myPoints.size(); i++){

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

        distance = radiusActual - _r; //distance error

        sumVV += distance * distance;

    }*/

    for(int i = 0; i < numPoints; i++){

        _x = randomSample.at(i)->xyz[0];
        _y = randomSample.at(i)->xyz[1];
        _z = randomSample.at(i)->xyz[2];

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

        distance = radiusActual - _r; //distance error

        sumVV += distance * distance;

    }

    this->myCylinderState->radius = _r;
    this->myCylinderState->xyz[0] = _X0;
    this->myCylinderState->xyz[1] = _Y0;
    this->myCylinderState->alpha = _alpha;
    this->myCylinderState->beta = _beta;
    this->myState->mainFocus[0] = centroid[0];
    this->myState->mainFocus[1] = centroid[1];
    this->myState->mainFocus[2] = centroid[2];
    //this->myState->sigma = qSqrt( sumVV / ((double)this->myState->myPoints.size() - 5.0) );
    this->myState->sigma = qSqrt( sumVV / ((double)numPoints - 5.0) );
    this->myState->isValid = true;

}

/*!
 * \brief PS_CylinderSegment::minimumSolution
 * Calculate cylinder from 9 or more points
 * \param points
 */
void PS_CylinderSegment::minimumSolution(const QList<PS_Point_PC *> &points){

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
        PS_Point_PC *p = points.at(i);
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

    /*double H_val[9];
    for(int i = 0; i < 9; i++){
        H_val[i] = 0.0;
    }*/
    for (int k = 0; k < numPoints; k++) {
        PS_Point_PC *p = points.at(k);
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
                //H_val[i*3+j] = H_val[i*3+j] + a * b;
                PS_CylinderSegment::H.setAt(i,j, PS_CylinderSegment::H.getAt(i,j) + a * b);
            }
        }
    }
    /*for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            PS_CylinderSegment::H.setAt(i,j, H_val[i*3+j]);
        }
    }*/

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

        PS_CylinderSegment::u.getCol(pn, i); //Eigenvektor

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
            PS_Point_PC *p = points.at(j);

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
        OiVec::dot(sum_vv, v,v);
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
 * \param toleranceFactor
 */
void PS_CylinderSegment::sortOut(PS_CylinderSegment *myCylinder, const PS_InputParameter &param, const int &toleranceFactor){

    QList<PS_Point_PC *> myPoints = myCylinder->getPoints();
    myCylinder->removeAllPoints();
    PS_CylinderSegment::checkPointsInCylinder(myCylinder, myPoints, param, toleranceFactor);

}

/*!
 * \brief PS_CylinderSegment::mergeCylinders
 * \param detectedCylinders
 * \param mergedCylinders
 * \param param
 */
void PS_CylinderSegment::mergeCylinders(const QList<PS_CylinderSegment *> &detectedCylinders, QList<PS_CylinderSegment *> &mergedCylinders, const PS_InputParameter &param){

    /*float diffAlpha = 0.0, diffBeta = 0.0; //rotation angle differences
    float diffXYZ = 0.0; //distance of the 2D centroid of one cylinder to the other cylinder
    float diffRadius = 0.0; //radius difference*/

    float diffRadius = 0.0; //radius difference
    double distX01C2 = 0.0, distX02C1 = 0.0; //distance of a main focus of the one cylinder to the other cylinder axis

    OiVec distHelper(3);
    OiVec ijk1Helper(3);
    OiVec ijk2Helper(3);
    OiVec x01Helper(3);
    OiVec x02Helper(3);
    OiVec pc01Helper(3);
    OiVec pc02Helper(3);

    if(mergedCylinders.size() != 0){
        mergedCylinders.clear();
    }

    //for each cylinder save its merged state
    QMap<int, bool> mergeMap;
    for(int i = 0; i < detectedCylinders.size(); ++i){
        mergeMap.insert(i, false);
    }

    for(int i = 0; i < detectedCylinders.size(); ++i){

        //if the cylinder at position i was merged before
        if(mergeMap.value(i) == true){
            continue;
        }
        mergeMap.insert(i, true);

        PS_CylinderSegment *c1 = detectedCylinders.at(i);

        //compare all cylinder's params and merge them if they fit together
        for(int j = i+1; j < detectedCylinders.size(); ++j){

            //if the cylinder at position j was merged before
            if(mergeMap.value(j) == true){
                continue;
            }

            PS_CylinderSegment *c2 = detectedCylinders.at(j);


            diffRadius = qAbs(c2->getRadius() - c1->getRadius());

            float ijk1[3], ijk2[3], *x01, *x02, pc1[3], pc2[3];
            c2->getIJK(ijk2);
            c1->getIJK(ijk1);
            c2->getX0(pc2);
            c1->getX0(pc1);
            x01 = c1->getMainFocus();
            x02 = c2->getMainFocus();

            ijk1Helper.setAt(0, ijk1[0]);
            ijk1Helper.setAt(1, ijk1[1]);
            ijk1Helper.setAt(2, ijk1[2]);
            ijk2Helper.setAt(0, ijk2[0]);
            ijk2Helper.setAt(1, ijk2[1]);
            ijk2Helper.setAt(2, ijk2[2]);
            x01Helper.setAt(0, x01[0]);
            x01Helper.setAt(1, x01[1]);
            x01Helper.setAt(2, x01[2]);
            x02Helper.setAt(0, x02[0]);
            x02Helper.setAt(1, x02[1]);
            x02Helper.setAt(2, x02[2]);
            pc01Helper.setAt(0, pc1[0]);
            pc01Helper.setAt(1, pc1[1]);
            pc01Helper.setAt(2, pc1[2]);
            pc02Helper.setAt(0, pc2[0]);
            pc02Helper.setAt(1, pc2[1]);
            pc02Helper.setAt(2, pc2[2]);

            /*qDebug() << "a " << ijk2Helper.getAt(0) << " " << ijk2Helper.getAt(1) << " " << ijk2Helper.getAt(2);
            qDebug() << x01[0] << " " << x01[1] << " " << x01[2];
            qDebug() << x02[0] << " " << x02[1] << " " << x02[2];*/

            OiVec::cross(distHelper, ijk2Helper, x01Helper - pc02Helper);
            //qDebug() << "cross " << distHelper.getAt(0) << " " << distHelper.getAt(1) << " " << distHelper.getAt(2);
            OiVec::dot(distX01C2, distHelper, distHelper);
            distX01C2 = qSqrt(distX01C2);

            //qDebug() << "b " << ijk1Helper.getAt(0) << " " << ijk1Helper.getAt(1) << " " << ijk1Helper.getAt(2);

            OiVec::cross(distHelper, ijk1Helper, x02Helper - pc01Helper);
            OiVec::dot(distX02C1, distHelper, distHelper);
            distX02C1 = qSqrt(distX02C1);




            /*diffAlpha = qAbs(c2->getAlpha() - c1->getAlpha());
            diffBeta = qAbs(c2->getBeta() - c1->getBeta());
            diffXYZ = qSqrt( (c2->getXYZ()[0]-c1->getXYZ()[0])*(c2->getXYZ()[0]-c1->getXYZ()[0])
                    + (c2->getXYZ()[1]-c1->getXYZ()[1])*(c2->getXYZ()[1]-c1->getXYZ()[1]) );
            diffRadius = qAbs(c2->getRadius() - c1->getRadius());

            if(qAbs(diffAlpha - 2.0*PS_PI) < diffAlpha){ diffAlpha = qAbs(diffAlpha - 2.0*PS_PI); }
            if(qAbs(diffBeta - 2.0*PS_PI) < diffBeta){ diffBeta = qAbs(diffBeta - 2.0*PS_PI); }*/



            //if the parameter-differences are under a threshold
            /*if(diffXYZ < 10.0 && diffRadius < 10.0*param.cylinderParams.maxDistance
                    && diffAlpha < 0.4 && diffBeta < 0.4){*/
            /*qDebug() << "diff alpha " << diffAlpha;
            qDebug() << "diff beta " << diffBeta;
            qDebug() << "diff radius " << diffRadius;*/

            /*if(diffRadius < 10.0*param.cylinderParams.maxDistance && diffXYZ < 10.0*param.cylinderParams.maxDistance
                                && diffAlpha < 0.0873 && diffBeta < 0.0873){ //0.4*/

            //qDebug() << "diff x01c2 " << distX01C2;
            //qDebug() << "diff x02c1 " << distX02C1;
            //qDebug() << "diff radius " << diffRadius;

            /*if(diffRadius < 3.0*param.cylinderParams.maxDistance &&
                    distX01C2 < 3.0*param.cylinderParams.maxDistance
                    && distX02C1 < 3.0*param.cylinderParams.maxDistance){*/
                //qDebug() << "check";
            //qDebug() << c1->getRadius() << " " << c2->getRadius() << " " << distX01C2 << " " << distX02C1;
            if(distX01C2 < 5.0*c2->getRadius() && distX02C1 < 5.0*c1->getRadius()){

                PS_CylinderSegment *mergedCylinder = new PS_CylinderSegment();
                QList<PS_Point_PC *> mergeList;
                //mergedCylinder->setIsValid(true);
                foreach(PS_Point_PC *myPoint, c1->getPoints()){
                    myPoint->isUsed = false;
                    mergedCylinder->addPoint(myPoint);
                    mergeList.append(myPoint);
                }
                foreach(PS_Point_PC *myPoint, c2->getPoints()){
                    myPoint->isUsed = false;
                    mergedCylinder->addPoint(myPoint);
                    mergeList.append(myPoint);
                }

                //set approximate values
                /*if(c1->getPointCount() > c2->getPointCount()){
                    mergedCylinder->setApproximation(c1->getAlpha(), c1->getBeta(), c1->getRadius(), c1->getXYZ()[0], c1->getXYZ()[1]);
                }else{
                    mergedCylinder->setApproximation(c2->getAlpha(), c2->getBeta(), c2->getRadius(), c2->getXYZ()[0], c2->getXYZ()[1]);
                }*/

                PS_CylinderSegment *tmp = PS_CylinderSegment::detectCylinder(mergeList, param);
                mergedCylinder->setApproximation(tmp->getAlpha(), tmp->getBeta(), tmp->getRadius(), tmp->getXYZ()[0], tmp->getXYZ()[1]);
                delete tmp;

                /*qDebug() << "size both " << mergedCylinder->getPointCount();
                qDebug() << "c1 size " << c1->getPointCount();
                qDebug() << "c2 size " << c2->getPointCount();*/

                //fit cylinder and sort out points that do not satisfy the distance criterion
                //mergedCylinder->fitBySample(param.fitSampleSize * 3);
                mergedCylinder->fitBySample(param.fitSampleSize * 10);
                PS_CylinderSegment::sortOut(mergedCylinder, param, 3);
                //mergedCylinder->fitBySample(param.fitSampleSize);

                /*qDebug() << "valid " << mergedCylinder->getIsValid();
                qDebug() << "sigma " << mergedCylinder->getSigma();
                qDebug() << "radius " << mergedCylinder->getRadius();
                qDebug() << "size " << mergedCylinder->getPointCount();*/

                if(mergedCylinder->getIsValid() //&& mergedCylinder->getSigma() <= param.cylinderParams.maxDistance
                        && mergedCylinder->getRadius() >= param.cylinderParams.minRadius
                        && mergedCylinder->getRadius() <= param.cylinderParams.maxRadius
                        && mergedCylinder->getPoints().size() > c1->getPoints().size()
                        && mergedCylinder->getPoints().size() > c2->getPoints().size()){

                    //delete single cylinders
                    delete c1;
                    delete c2;

                    //set the planes to be merged
                    mergeMap.insert(j, true);

                    c1 = mergedCylinder;

                    //qDebug() << "accept";

                }else{
                    delete mergedCylinder;
                    //qDebug() << "reject";
                }

            }

        }

        //add the cylinder to result list
        mergedCylinders.append(c1);

    }

}

/*!
 * \brief PS_CylinderSegment::reviewNodes
 * Review all points of used nodes and try to add them to the cylinder
 * \param detectedCylinders
 * \param param
 */
void PS_CylinderSegment::reviewNodes(const QList<PS_CylinderSegment *> &detectedCylinders, const PS_InputParameter &param){

    bool pointsAdded = false;
    int numAdded = 0; //number of added points

    //list to save all unmerged points of each node
    QList<PS_Point_PC *> unmergedPoints;

    foreach(PS_CylinderSegment *c, detectedCylinders){

        //consider each node from which points were used for the cylinder
        foreach(PS_Node *n, c->getUsedNodes()){

            //try to add points that were not added yet
            n->getUnmergedPoints(unmergedPoints);
            numAdded = PS_CylinderSegment::checkPointsInCylinder(c, unmergedPoints, param, 3);
            QList<PS_Point_PC *> cylinderPoints = c->getPoints();
            for(int i = 0; i < numAdded; ++i){
                cylinderPoints.at(cylinderPoints.size()-1-i)->isUsed = true;
            }

            if(numAdded > 0){
                pointsAdded = true;
            }

            //set node as merged
            n->setWasConsideredInMerge(true);

        }

        if(pointsAdded){
            //refit the cylinder
            //c->fitBySample(param.fitSampleSize);
        }

        //set all nodes as unconsidered in merge
        foreach(PS_Node *n, c->getUsedNodes()){
            n->setWasConsideredInMerge(false);
        }

    }

}

/*!
 * \brief PS_CylinderSegment::verifyCylinders
 * Verify the detected cylinders (eg. sort out cylinders whose points almost lie in a plane)
 * \param detectedCylinders
 * \param verifiedCylinders
 * \param param
 */
void PS_CylinderSegment::verifyCylinders(const QList<PS_CylinderSegment *> &detectedCylinders, QList<PS_CylinderSegment *> &verifiedCylinders, const PS_InputParameter &param){

    //initialize variables
    double centroid[3]; //main focus of sphere points
    int numPoints;
    double eVal; //smallest eigen-value
    double planeSigma; //sigma of a fitted plane
    OiMat ata(3,3);

    foreach(PS_CylinderSegment *cylinder, detectedCylinders){

        //crCoord.clear();

        numPoints = cylinder->getPointCount();

        //calc centroid of cylinder points
        centroid[0] = 0.0;
        centroid[1] = 0.0;
        centroid[2] = 0.0;
        for(unsigned int i = 0; i < numPoints; ++i){
            PS_Point_PC *p = cylinder->getPoints().at(i);
            centroid[0] += p->xyz[0];
            centroid[1] += p->xyz[1];
            centroid[2] += p->xyz[2];
        }
        centroid[0] = centroid[0] / (double)numPoints;
        centroid[1] = centroid[1] / (double)numPoints;
        centroid[2] = centroid[2] / (double)numPoints;

        double value = 0.0;
        for(int i = 0; i < 3; ++i){
            for(int j = 0; j < 3; ++j){
                value = 0.0;
                for(unsigned int k = 0; k < numPoints; ++k){
                    PS_Point_PC *p = cylinder->getPoints().at(k);
                    value += (p->xyz[i] - centroid[i]) * (p->xyz[j] - centroid[j]);
                }
                ata.setAt(i, j, value);
            }
        }

        ata.svd(PS_CylinderSegment::verify_u, PS_CylinderSegment::verify_d, PS_CylinderSegment::verify_v);

        eVal = numeric_limits<double>::max();
        for(int i = 0; i < 3; ++i){
            if(PS_CylinderSegment::verify_d.getAt(i) < eVal){
                eVal = PS_CylinderSegment::verify_d.getAt(i);
            }
        }

        planeSigma = qSqrt(eVal / (double)(numPoints - 3.0));

        if( 3.0 * planeSigma > cylinder->getSigma()){//3.0*param.sphereParams.maxDistance ){

            //qDebug() << "sigma plane " << planeSigma;
            verifiedCylinders.append(cylinder);

        }else{
            delete cylinder;
        }

    }

}

/*!
 * \brief PS_CylinderSegment::getX0
 * Get a 3D point on the cylinder axis
 * \param x0
 */
void PS_CylinderSegment::getX0(float *x0) const{

    PS_CylinderSegment::Ralpha.setAt(0, 0, 1.0);
    PS_CylinderSegment::Ralpha.setAt(1, 1, qCos(this->getAlpha()));
    PS_CylinderSegment::Ralpha.setAt(1, 2, -qSin(this->getAlpha()));
    PS_CylinderSegment::Ralpha.setAt(2, 1, qSin(this->getAlpha()));
    PS_CylinderSegment::Ralpha.setAt(2, 2, qCos(this->getAlpha()));
    PS_CylinderSegment::Rbeta.setAt(0, 0, qCos(this->getBeta()));
    PS_CylinderSegment::Rbeta.setAt(0, 2, qSin(this->getBeta()));
    PS_CylinderSegment::Rbeta.setAt(1, 1, 1.0);
    PS_CylinderSegment::Rbeta.setAt(2, 0, -qSin(this->getBeta()));
    PS_CylinderSegment::Rbeta.setAt(2, 2, qCos(this->getBeta()));

    PS_CylinderSegment::Rall = PS_CylinderSegment::Rbeta * PS_CylinderSegment::Ralpha;

    PS_CylinderSegment::x_m_n.setAt(0, this->getXYZ()[0]);
    PS_CylinderSegment::x_m_n.setAt(1, this->getXYZ()[1]);
    PS_CylinderSegment::x_m_n.setAt(2, 0.0);
    OiMat::solve(PS_CylinderSegment::x_m_n, PS_CylinderSegment::Rall, (-1.0) * PS_CylinderSegment::x_m_n);

    x0[0] = PS_CylinderSegment::x_m_n.getAt(0);
    x0[1] = PS_CylinderSegment::x_m_n.getAt(1);
    x0[2] = PS_CylinderSegment::x_m_n.getAt(2);

}

/*!
 * \brief PS_CylinderSegment::getIJK
 * Get the cylinder axis as a unit vector
 * \param ijk
 */
void PS_CylinderSegment::getIJK(float *ijk) const{

    PS_CylinderSegment::Ralpha.setAt(0, 0, 1.0);
    PS_CylinderSegment::Ralpha.setAt(1, 1, qCos(this->getAlpha()));
    PS_CylinderSegment::Ralpha.setAt(1, 2, -qSin(this->getAlpha()));
    PS_CylinderSegment::Ralpha.setAt(2, 1, qSin(this->getAlpha()));
    PS_CylinderSegment::Ralpha.setAt(2, 2, qCos(this->getAlpha()));
    PS_CylinderSegment::Rbeta.setAt(0, 0, qCos(this->getBeta()));
    PS_CylinderSegment::Rbeta.setAt(0, 2, qSin(this->getBeta()));
    PS_CylinderSegment::Rbeta.setAt(1, 1, 1.0);
    PS_CylinderSegment::Rbeta.setAt(2, 0, -qSin(this->getBeta()));
    PS_CylinderSegment::Rbeta.setAt(2, 2, qCos(this->getBeta()));

    PS_CylinderSegment::Rall = PS_CylinderSegment::Rbeta * PS_CylinderSegment::Ralpha;

    PS_CylinderSegment::n0.setAt(0, 0.0);
    PS_CylinderSegment::n0.setAt(1, 0.0);
    PS_CylinderSegment::n0.setAt(2, 1.0);
    OiMat::solve(PS_CylinderSegment::n0, PS_CylinderSegment::Rall, PS_CylinderSegment::n0);

    PS_CylinderSegment::n0 = PS_CylinderSegment::n0.normalize();

    ijk[0] = PS_CylinderSegment::n0.getAt(0);
    ijk[1] = PS_CylinderSegment::n0.getAt(1);
    ijk[2] = PS_CylinderSegment::n0.getAt(2);

}

/*!
 * \brief PS_CylinderSegment::setApproximation
 * \param alpha
 * \param beta
 * \param radius
 * \param x
 * \param y
 */
void PS_CylinderSegment::setApproximation(const float &alpha, const float &beta, const float &radius, const float &x, const float &y)
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
