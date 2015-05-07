#include "p_helmert7Param.h"

void Helmert7Param::init(){

    //set plugin meta data
    this->metaData.name = "HelmertTransformation";
    this->metaData.pluginName = "OpenIndy Default Plugin";
    this->metaData.author = "bra";
    this->metaData.description = QString("%1 %2")
            .arg("This function calculates a 7 parameter helmert transformation.")
            .arg("That transformation is based on identical points in start and target system.");
    this->metaData.iid = "de.openIndy.plugin.function.systemTransformation.v001";

    //set needed elements
    NeededElement param1;
    param1.description = "Select points for calculating the transformation.";
    param1.infinite = true;
    param1.typeOfElement = ePointElement;
    this->neededElements.append(param1);

    //set spplicable for
    this->applicableFor.append(eTrafoParamFeature);

}

/*!
 * \brief Helmert7Param::exec
 * \param trafoParam
 * \return
 */
bool Helmert7Param::exec(TrafoParam &trafoParam){
    this->svdError = false;

    this->initPoints(); //fills the locSystem and refSystem vectors based on the given common points.
    if(locSystem.count() == refSystem.count() && locSystem.count() > 2){ //if enough common points available
        if(this->calc(trafoParam)){
            if(locSystem.count() > 3){
                return this->adjust(trafoParam);
            }else if(locSystem.count() == 3){
                return true;
            }
        }
    }else{
        emit this->sendMessage("Not enough common points!");
    }

    return false;
}

/*!
 * \brief Helmert7Param::initPoints
 */
void Helmert7Param::initPoints(){

    //clear lists
    this->locSystem.clear();
    this->refSystem.clear();

    //get and check input points
    /*if(!this->inputElements.contains(0) || this->inputElements[0].size() < 3
            || !this->inputElements.contains(1) || this->inputElements[1].size() != this->inputElements[0].size()){
        return;
    }
    for(int i = 0; i < this->inputElements[0].size(); i++){
        if(this->inputElements[0].at(i).point.isNull() || this->inputElements[1].at(i).point.isNull()
                || this->inputElements[0].at(i).point->getFeatureName().compare(this->inputElements[1].at(i).point->getFeatureName()) != 0){
            this->setUseState(0, this->inputElements[0].at(i).point->getId(), false);
            this->setUseState(1, this->inputElements[1].at(i).point->getId(), false);
            continue;
        }
        this->setUseState(0, this->inputElements[0].at(i).point->getId(), true);
        this->setUseState(1, this->inputElements[1].at(i).point->getId(), true);
        this->locSystem.append(this->inputElements[0].at(i).point->getPosition().getVectorH());
        this->refSystem.append(this->inputElements[1].at(i).point->getPosition().getVectorH());
    }*/

    if(this->inputPointsStartSystem.size() >= 3 &&
            this->inputPointsStartSystem.size() == this->inputPointsDestinationSystem.size()){

        for(int i = 0; i < this->inputPointsStartSystem.size(); i++){

            this->locSystem.append(this->inputPointsStartSystem.at(i).getPosition().getVector());
            this->refSystem.append(this->inputPointsDestinationSystem.at(i).getPosition().getVector());

        }

    }

}

/*!
 * \brief Helmert7Param::calc
 * Calculate transformation parameter
 * \param tp
 * \return
 */
bool Helmert7Param::calc(TrafoParam &tp){
    vector<OiVec> centroidCoords = this->calcCentroidCoord(); //centroid coordinates
    if(centroidCoords.size() == 2){
        vector<OiVec> locC = this->centroidReducedCoord(locSystem, centroidCoords.at(0)); //centroid reduced destination coordinates
        vector<OiVec> refC = this->centroidReducedCoord(refSystem, centroidCoords.at(1)); //centroid reduced target coordinates
        vector<OiMat> modelMatrices = this->modelMatrix(locC, refC); //vector of model matrices - one for each common point
        OiMat n = this->normalEquationMatrix(modelMatrices); //calculate the normal equation matrix
        OiVec q = this->quaternion(n);
        if( !svdError ){
            OiMat r = this->rotationMatrix(q); //fill rotation matrix
            this->fillTrafoParam(r, locC, refC, centroidCoords, tp); //fill trafo param object
            return true;
        }
    }
    return false;
}

/*!
 * \brief Helmert7Param::calcCentroidCoord
 * Calculate centroid coordinates for start and target system
 * \return
 */
vector<OiVec> Helmert7Param::calcCentroidCoord(){
    vector<OiVec> result;
    //centroid for destination system
    OiVec centroidLoc(3);
    foreach(OiVec loc, this->locSystem){
        OiVec coord;
        coord.add(loc.getAt(0));
        coord.add(loc.getAt(1));
        coord.add(loc.getAt(2));
        centroidLoc = centroidLoc + coord;
    }
    if(this->locSystem.count() > 0){ result.push_back( centroidLoc / this->locSystem.count() ); }
    //centroid for target system
    OiVec centroidRef(3);
    foreach(OiVec ref, this->refSystem){
        OiVec coord;
        coord.add(ref.getAt(0));
        coord.add(ref.getAt(1));
        coord.add(ref.getAt(2));
        centroidRef = centroidRef + coord;
    }
    if(refSystem.count() > 0){ result.push_back( centroidRef / this->refSystem.count() ); }
    return result;
}

/*!
 * \brief Helmert7Param::centroidReducedCoord
 * Calculates centroid reduced coordinates
 * \param input
 * \param centroid
 * \return
 */
vector<OiVec> Helmert7Param::centroidReducedCoord(QList<OiVec> input, OiVec centroid){
    vector<OiVec> result;
    foreach(OiVec coord, input){
        OiVec c;
        c.add(coord.getAt(0));
        c.add(coord.getAt(1));
        c.add(coord.getAt(2));
        OiVec rc = c - centroid;
        result.push_back( rc );
    }
    return result;
}

/*!
 * \brief Helmert7Param::modelMatrix
 * Returns a vector of model matrices. One model matrix for each common point.
 * \param locC
 * \param refC
 * \return
 */
vector<OiMat> Helmert7Param::modelMatrix(vector<OiVec> locC, vector<OiVec> refC){
    vector<OiMat> result;
    if(locC.size() == refC.size()){
        for(int i = 0; i < locC.size(); i++){
            OiMat a(4, 4);
            a.setAt(0, 1, -( locC.at(i).getAt(0) - refC.at(i).getAt(0) ));
            a.setAt(0, 2, -( locC.at(i).getAt(1) - refC.at(i).getAt(1) ));
            a.setAt(0, 3, -( locC.at(i).getAt(2) - refC.at(i).getAt(2) ));
            a.setAt(1, 0, ( locC.at(i).getAt(0) - refC.at(i).getAt(0) ));
            a.setAt(1, 2, ( locC.at(i).getAt(2) + refC.at(i).getAt(2) ));
            a.setAt(1, 3, -( locC.at(i).getAt(1) + refC.at(i).getAt(1) ));
            a.setAt(2, 0, ( locC.at(i).getAt(1) - refC.at(i).getAt(1) ));
            a.setAt(2, 1, -( locC.at(i).getAt(2) + refC.at(i).getAt(2) ));
            a.setAt(2, 3, ( locC.at(i).getAt(0) + refC.at(i).getAt(0) ));
            a.setAt(3, 0, ( locC.at(i).getAt(2) - refC.at(i).getAt(2) ));
            a.setAt(3, 1, ( locC.at(i).getAt(1) + refC.at(i).getAt(1) ));
            a.setAt(3, 2, -( locC.at(i).getAt(0) + refC.at(i).getAt(0) ));
            result.push_back(a);
        }
    }
    return result;
}

/*!
 * \brief Helmert7Param::normalEquationMatrix
 * Calculate normal equation matrix
 * \param vecA
 * \return
 */
OiMat Helmert7Param::normalEquationMatrix(vector<OiMat> vecA){
    OiMat result(4, 4);
    foreach(OiMat a, vecA){
        result +=  a.t() * a;
    }
    return result;
}

/*!
 * \brief Helmert7Param::quaternion
 * SVD to get the transformation vector (quaternion)
 * \param n
 * \return
 */
OiVec Helmert7Param::quaternion(OiMat n){
    OiVec q(4);
    try{
        OiMat u(4,4);
        OiMat v(4,4);
        OiVec s(4);
        n.svd(u, s, v);
        u.getCol(q, 3);
    }catch(runtime_error& e){
        svdError = true;
    }
    return q;
}

/*!
 * \brief Helmert7Param::rotationMatrix
 * Set up the rotation matrix by using the quaternion q
 * \param q
 * \return
 */
OiMat Helmert7Param::rotationMatrix(OiVec q){
    OiMat result(3, 3);
    result.setAt(0, 0, q.getAt(0)*q.getAt(0) + q.getAt(1)*q.getAt(1) - q.getAt(2)*q.getAt(2) - q.getAt(3)*q.getAt(3));
    result.setAt(0, 1, 2*(q.getAt(1)*q.getAt(2) - q.getAt(0)*q.getAt(3)));
    result.setAt(0, 2, 2*(q.getAt(1)*q.getAt(3) + q.getAt(0)*q.getAt(2)));
    result.setAt(1, 0, 2*(q.getAt(1)*q.getAt(2) + q.getAt(0)*q.getAt(3)));
    result.setAt(1, 1, q.getAt(0)*q.getAt(0) - q.getAt(1)*q.getAt(1) + q.getAt(2)*q.getAt(2) - q.getAt(3)*q.getAt(3));
    result.setAt(1, 2, 2*(q.getAt(2)*q.getAt(3) - q.getAt(0)*q.getAt(1)));
    result.setAt(2, 0, 2*(q.getAt(3)*q.getAt(1) - q.getAt(0)*q.getAt(2)));
    result.setAt(2, 1, 2*(q.getAt(3)*q.getAt(2) + q.getAt(0)*q.getAt(1)));
    result.setAt(2, 2, q.getAt(0)*q.getAt(0) - q.getAt(1)*q.getAt(1) - q.getAt(2)*q.getAt(2) + q.getAt(3)*q.getAt(3));
    return result;
}

/*!
 * \brief Helmert7Param::fillTrafoParam
 * Fills trafoParam object
 * \param r
 * \param tp
 */
void Helmert7Param::fillTrafoParam(OiMat r, vector<OiVec> locC, vector<OiVec> refC, vector<OiVec> centroidCoords, TrafoParam &tp){
    //calc scale
    double o = 0.0;
    double u = 0.0;
    for(int i = 0; i < locC.size(); i++){
        OiVec vecO = refC.at(i).t() * r * locC.at(i);
        OiVec vecU = locC.at(i).t() * locC.at(i);
        o += vecO.getAt(0);
        u += vecU.getAt(0);
    }
    double m = 1.0;
    if(u > 0){ m = o / u; }
    //tp.setScale(m, m, m);
    //calc translation
    OiVec t = centroidCoords.at(1) - m * r * centroidCoords.at(0);
    //tp.setTranslation(t.getAt(0), t.getAt(1), t.getAt(2));
    //calc rotation
    OiVec rot(3);
    rot.setAt(0, qAtan2(-r.getAt(2,1), r.getAt(2,2))); //alpha
    rot.setAt(1, qAsin(r.getAt(2,0))); //beta
    rot.setAt(2, qAtan2(-r.getAt(1,0), r.getAt(0,0))); //gamma
    if( qFabs(qCos(rot.getAt(1)) * qCos(rot.getAt(2))) - qFabs(r.getAt(0,0)) > 0.01 ){
        rot.setAt(1, PI - rot.getAt(1));
    }
    //tp.setRotation(rot.getAt(0), rot.getAt(1), rot.getAt(2));
    //fill transformation matrix
    OiMat translation(4, 4);
    translation.setAt(0, 0, 1.0);
    translation.setAt(1, 1, 1.0);
    translation.setAt(2, 2, 1.0);
    translation.setAt(3, 3, 1.0);
    translation.setAt(0, 3, t.getAt(0));
    translation.setAt(1, 3, t.getAt(1));
    translation.setAt(2, 3, t.getAt(2));
    OiMat scale(4, 4);
    scale.setAt(0, 0, m);
    scale.setAt(1, 1, m);
    scale.setAt(2, 2, m);
    scale.setAt(3, 3, 1.0);
    OiMat rotation(4, 4);
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            rotation.setAt(i, j, r.getAt(i, j));
        }
    }
    rotation.setAt(3, 3, 1.0);
    tp.setTransformationParameters(rotation, translation, scale);

    double sumVV = 0.0;

    for (int i = 0;i<this->locSystem.size();i++) {
        OiVec diffVec = this->refSystem.at(i)-(tp.getHomogenMatrix()*this->locSystem.at(i));
        sumVV += diffVec.getAt(0)*diffVec.getAt(0);
        sumVV += diffVec.getAt(1)*diffVec.getAt(1);
        sumVV += diffVec.getAt(2)*diffVec.getAt(2);

    }

    Statistic stats;
    stats.setStdev(sqrt(sumVV/(3.0*this->locSystem.size()-7.0)));
    tp.setStatistic(stats);
    //tp.generateHomogenMatrix();
}

/*!
 * \brief Helmert7Param::adjust
 * Transformation with small angles
 * \param tp
 * \return
 */
bool Helmert7Param::adjust(TrafoParam &tp){
    bool result = false;

    //approximation
    OiVec x0(7);
    x0.setAt(0, tp.getRotation().getAt(0));
    x0.setAt(1, tp.getRotation().getAt(1));
    x0.setAt(2, tp.getRotation().getAt(2));
    x0.setAt(3, tp.getScale().getAt(0));
    x0.setAt(4, tp.getTranslation().getAt(0));
    x0.setAt(5, tp.getTranslation().getAt(1));
    x0.setAt(6, tp.getTranslation().getAt(2));

    //observations
    OiVec l = this->fillLVector();

    OiVec l_diff;
    OiMat a;
    OiVec x;
    OiMat qxx;

    //iterate till solution is ok
    int iterations = 0;
    OiVec stop;
    do{

        //A matrix
        a = this->fillAMatrix(x0);

        //normalequation --> trying to invert it
        OiMat n = a.t() * a;

        try{
            qxx = n.inv(); //try to calc the inverse
        }catch(runtime_error e){
            return false;
        }catch(logic_error e){
            return false;
        }

        //differential l vector
        OiVec l0 = this->fillL0Vector(x0);
        l_diff = l - l0;

        //right side
        OiVec c = a.t() * l_diff;

        //calc x
        x = qxx * c;
        x0 = x0 + x;

        stop = x.t() * x;
        iterations++;

    }while( stop.getAt(0) > (0.000001) || iterations < 100 ); //termination criterion

    //if(iterations < 100){ //adjustment successful
        OiVec v = a * x - l_diff;
        OiVec vtv = v.t() * v;
        double s0_post = sqrt(vtv.getAt(0) / (3.0 * this->locSystem.length() - 7.0));
        OiMat sxx = s0_post * s0_post * qxx;
        //tp.getStatistic()->stdev = s0_post;
        result = true;
    //}

    return result;
}

/*!
 * \brief Helmert7Param::fillAMatrix
 * Fills the A Matrix
 * x^T = alpha beta gamma m X0 Y0 Z0
 * \param x0
 * \return
 */
OiMat Helmert7Param::fillAMatrix(OiVec x0){
    OiMat a(locSystem.length() * 3, 7);
    for(int row = 0; row < locSystem.length() * 3; row++){
        if( (row+1) % 3 == 1 ){ //observation of x
            a.setAt(row, 0, 0);
            a.setAt(row, 1, -locSystem.at(row/3).getAt(2) * x0.getAt(3));
            a.setAt(row, 2, locSystem.at(row/3).getAt(1) * x0.getAt(3));
            a.setAt(row, 3, locSystem.at(row/3).getAt(0) + locSystem.at(row/3).getAt(1) * x0.getAt(2) - locSystem.at(row/3).getAt(2) * x0.getAt(1));
            a.setAt(row, 4, 1);
            a.setAt(row, 5, 0);
            a.setAt(row, 6, 0);
        }else if( (row+1) % 3 == 2 ){ //observation of y
            a.setAt(row, 0, locSystem.at(row/3).getAt(2) * x0.getAt(3));
            a.setAt(row, 1, 0);
            a.setAt(row, 2, -locSystem.at(row/3).getAt(0) * x0.getAt(3));
            a.setAt(row, 3, locSystem.at(row/3).getAt(1) - locSystem.at(row/3).getAt(0) * x0.getAt(2) + locSystem.at(row/3).getAt(2) * x0.getAt(0));
            a.setAt(row, 4, 0);
            a.setAt(row, 5, 1);
            a.setAt(row, 6, 0);
        }else if( (row+1) % 3 == 0 ){ //observation of z
            a.setAt(row, 0, -locSystem.at(row/3).getAt(1) * x0.getAt(3));
            a.setAt(row, 1, locSystem.at(row/3).getAt(0) * x0.getAt(3));
            a.setAt(row, 2, 0);
            a.setAt(row, 3, locSystem.at(row/3).getAt(2) + locSystem.at(row/3).getAt(0) * x0.getAt(1) - locSystem.at(row/3).getAt(1) * x0.getAt(0));
            a.setAt(row, 4, 0);
            a.setAt(row, 5, 0);
            a.setAt(row, 6, 1);
        }
    }
    return a;
}

/*!
 * \brief Helmert7Param::fillLVector
 * Fills the L Vector
 * \return
 */
OiVec Helmert7Param::fillLVector(){
    OiVec l(this->locSystem.length() * 3);
    for(int row = 0; row < this->locSystem.length() * 3; row++){
        if( (row+1) % 3 == 1 ){ //observation of x
            l.setAt(row, this->locSystem.at(row/3).getAt(0));
        }else if( (row+1) % 3 == 2 ){ //observation of y
            l.setAt(row, this->locSystem.at(row/3).getAt(1));
        }else if( (row+1) % 3 == 0 ){ //observation of z
            l.setAt(row, this->locSystem.at(row/3).getAt(2));
        }
    }
    return l;
}

/*!
 * \brief Helmert7Param::fillL0Vector
 * Fills the L0 Vector
 * \param x0
 * \return
 */
OiVec Helmert7Param::fillL0Vector(OiVec x0){
    OiVec l0(this->locSystem.length() * 3);
    for(int row = 0; row < this->locSystem.length() * 3; row++){
        if( (row+1) % 3 == 1 ){ //observation of x
            l0.setAt(row, x0.getAt(3) * this->locSystem.at(row/3).getAt(0) + x0.getAt(3) * x0.getAt(2) * this->locSystem.at(row/3).getAt(1) - x0.getAt(3) * x0.getAt(1) * this->locSystem.at(row/3).getAt(2) + x0.getAt(4));
        }else if( (row+1) % 3 == 2 ){ //observation of y
            l0.setAt(row, x0.getAt(3) * this->locSystem.at(row/3).getAt(1) - x0.getAt(3) * x0.getAt(2) * this->locSystem.at(row/3).getAt(0) + x0.getAt(3) * x0.getAt(0) * this->locSystem.at(row/3).getAt(2) + x0.getAt(5));
        }else if( (row+1) % 3 == 0 ){ //observation of z
            l0.setAt(row, x0.getAt(3) * this->locSystem.at(row/3).getAt(2) + x0.getAt(3) * x0.getAt(1) * this->locSystem.at(row/3).getAt(0) - x0.getAt(3) * x0.getAt(0) * this->locSystem.at(row/3).getAt(1) + x0.getAt(6));
        }
    }
    return l0;
}
