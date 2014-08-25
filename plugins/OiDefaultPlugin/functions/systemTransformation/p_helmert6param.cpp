#include "p_helmert6param.h"

Helmert6Param::Helmert6Param()
{
}

/*!
 * \brief getMetaData
 * \return
 */
PluginMetaData *Helmert6Param::getMetaData()
{
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "6ParameterHelmertTransformation";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "jw";
    metaData->description = QString("%1 %2 %3 %4 %5")
            .arg("This function calculates a 6 parameter helmert transformation.")
            .arg("That transformation is based on identical points in start and target system.")
            .arg("If you measure a expanded part object then create the movement first.")
            .arg("If the movement is valid the nominals get expanded with the scale to get a correct translation.")
            .arg("If no movement is set, the translation can be wrong.");
    metaData->iid = "de.openIndy.Plugin.Function.SystemTransformation.v001";
    return metaData;
}

/*!
 * \brief exec
 * \param tp
 * \return
 */
bool Helmert6Param::exec(TrafoParam &tp)
{
   this->svdError = false;
    if(this->isValid()){ //check wether all parameters for calculation are available
        this->init(); //fills the locSystem and refSystem vectors based on the given common points.
        if(locSystem.count() == refSystem.count() && locSystem.count() > 1){ //if enough common points available

            //apply movement if necessary

            this->applyMovements(tp);

            //get rotation and translation
            this->rotation = this->approxRotation();
            this->translation =  this->approxTranslation(this->rotation);

            if(locSystem.count() > 2){

                //adjust rotation and translation if more than 2 points are available
                return this->adjust(tp);

            }else if(locSystem.count() == 3){

                //fill trafo parameter
                OiVec scale(4);
                scale.setAt(0,1.0);
                scale.setAt(1,1.0);
                scale.setAt(2,1.0);
                scale.setAt(3,1.0);

                OiMat s = this->getScaleMatrix(scale);
                OiMat t = this->getTranslationMatrix(this->translation);
                OiMat r = this->getRotationMatrix(this->rotation);

                tp.setHomogenMatrix(r,t,s);
                return true;
            }

        }else{
            this->writeToConsole("Not enough common points!");
        }
    }else{
        this->writeToConsole("The Input arguments are not valid!");
    }

    return false;
}

/*!
 * \brief getNeededElements for this function
 * \return
 */
QList<InputParams> Helmert6Param::getNeededElements()
{
    QList<InputParams> result;
    InputParams param;
    param.index = 0;
    param.description = "Select points to be used for transformation.";
    param.infinite = true;
    param.typeOfElement = Configuration::ePointElement;
    result.append(param);
    return result;
}

/*!
 * \brief This function isapplicable for the following features
 * \return
 */
QList<Configuration::FeatureTypes> Helmert6Param::applicableFor()
{
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::eTrafoParamFeature);
    return result;
}

QMap<QString, QStringList> Helmert6Param::getStringParameter()
{
    QMap<QString,QStringList> result;
    QString key = "useTempComp";
    QStringList value;
    value.append("true");
    value.append("false");
    result.insert(key,value);

    return result;
}

/*!
 * \brief getResultProtocol returns the result protocol set by the function
 * \return
 */
QStringList Helmert6Param::getResultProtocol()
{
    QStringList result;

    result.append("no protocol set.");

    return result;
}

/*!
 * \brief init
 */
void Helmert6Param::init()
{
    this->locSystem.clear();
    this->refSystem.clear();

    if(this->points_startSystem.size() == this->points_targetSystem.size()){
        for(int i = 0; i < this->points_startSystem.size(); i++){
            if(this->points_startSystem.at(i).getIsSolved() && this->points_targetSystem.at(i).getIsSolved()){
                this->locSystem.append(this->points_startSystem.at(i).xyz);
                this->refSystem.append(this->points_targetSystem.at(i).xyz);
                this->setUseState(this->points_startSystem.at(i).getId(), true);
                this->setUseState(this->points_targetSystem.at(i).getId(), true);
            }else{
                this->setUseState(this->points_startSystem.at(i).getId(), false);
                this->setUseState(this->points_targetSystem.at(i).getId(), false);
            }
        }
    }
}

/*!
 * \brief calcCentroidCoord
 * Calculate centroid coordinates for start and target system
 * \return
 */
vector<OiVec> Helmert6Param::calcCentroidCoord()
{
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
 * \brief centroidReducedCoord
 * Calculates centroid reduced coordinates
 * \param input
 * \param centroid
 * \return
 */
vector<OiVec> Helmert6Param::centroidReducedCoord(QList<OiVec> input, OiVec centroid)
{
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
 * \brief modelMatrix
 * Returns a vector of model matrices. One model matrix for each common point.
 * \param locC
 * \param refC
 * \return
 */
vector<OiMat> Helmert6Param::modelMatrix(vector<OiVec> locC, vector<OiVec> refC)
{
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
 * \brief normalEquationMatrix
 * Calculate normal equation matrix
 * \param vecA
 * \return
 */
OiMat Helmert6Param::normalEquationMatrix(vector<OiMat> vecA)
{
    OiMat result(4, 4);
    foreach(OiMat a, vecA){
        result +=  a.t() * a;
    }
    return result;
}

/*!
 * \brief quaternion
 * SVD to get the transformation vector (quaternion)
 * \param n
 * \return
 */
OiVec Helmert6Param::quaternion(OiMat n)
{
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
 * \brief rotationMatrix
 * Set up the rotation matrix by using the quaternion q
 * \param q
 * \return
 */
OiMat Helmert6Param::rotationMatrix(OiVec q)
{
    //get rotation matrix out of quaternions
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

bool Helmert6Param::adjust(TrafoParam &tp)
{
    //adjust trafo param if enough points are given
    bool result = false;

    //transform loc (start system) to "pseudo"-loc system
    //transformation with previosly approximated translation and rotation
    this->preliminaryTransformation();

    //get standard deviation
    double sumVV = 0.0;

    for (int i = 0;i<this->locSystem.size();i++) {
        OiVec diffVec = this->refSystem.at(i)-this->locSystem.at(i);
        sumVV += diffVec.getAt(0)*diffVec.getAt(0);
        sumVV += diffVec.getAt(1)*diffVec.getAt(1);
        sumVV += diffVec.getAt(2)*diffVec.getAt(2);

    }

    tp.getStatistic()->stdev = sqrt(sumVV/(3.0*this->locSystem.size()-6.0));

    //get new rotation and translation between pseudo-loc system and ref system
    OiVec tmpRotation = this->approxRotation();
    OiVec tmpTranslation = this->approxTranslation(tmpRotation);

    //approximation
    OiVec x0(6);
    x0.setAt(0, tmpRotation.getAt(0));
    x0.setAt(1, tmpRotation.getAt(1));
    x0.setAt(2, tmpRotation.getAt(2));
    x0.setAt(3, tmpTranslation.getAt(0));
    x0.setAt(4, tmpTranslation.getAt(1));
    x0.setAt(5, tmpTranslation.getAt(2));

    //observations
    OiVec l = this->fillLVector();

    OiVec l_diff;
    OiMat a;
    OiVec x;
    OiMat qxx;

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

    OiVec v = a * x - l_diff;
    OiVec vtv = v.t() * v;
    double s0_post = sqrt(vtv.getAt(0) / (3 * this->locSystem.length() - 6));
    OiMat sxx = s0_post * s0_post * qxx;

    //tp.getStatistic()->stdev = s0_post;

    //set the trafo parameters with the previously calulated values and the additional values from adjustment

    OiVec scale(4);
    scale.setAt(0,1.0);
    scale.setAt(1,1.0);
    scale.setAt(2,1.0);
    scale.setAt(3,1.0);

    this->translation.setAt(0,this->translation.getAt(0)+x0.getAt(3));
    this->translation.setAt(1,this->translation.getAt(1)+x0.getAt(4));
    this->translation.setAt(2,this->translation.getAt(2)+x0.getAt(5));

    this->rotation.setAt(0,this->rotation.getAt(0)+x0.getAt(0));
    this->rotation.setAt(1,this->rotation.getAt(1)+x0.getAt(1));
    this->rotation.setAt(2,this->rotation.getAt(2)+x0.getAt(2));

    OiMat r = this->getRotationMatrix(this->rotation);
    OiMat t = this->getTranslationMatrix(this->translation);
    OiMat s = this->getScaleMatrix(scale);

    tp.setHomogenMatrix(r,t,s);

    result = true;

    return result;
}

/*!
 * \brief fillAMatrix fills the A matrix
 * rotation x, rotationy, rotation z, tx, ty, tz
 * \param x0
 * \return
 */
OiMat Helmert6Param::fillAMatrix(OiVec x0)
{
    OiMat a(locSystem.length()*3,6);

    //loc system is transformed with approx values of translation and rotation, so
    //this should be approxed enough for the pre transformation.
    //That´s why I can use the rotation matrix for small angles.
    for(int row=0; row<locSystem.length()*3; row++){
        if((row+1) % 3 == 1){ // x
            a.setAt(row,0,0.0);
            a.setAt(row,1,-locSystem.at(row/3).getAt(2));
            a.setAt(row,2,locSystem.at(row/3).getAt(1));
            a.setAt(row,3,1.0);
            a.setAt(row,4,0.0);
            a.setAt(row,5,0.0);
        }else if((row+1) % 3 == 2){ //y
            a.setAt(row,0,locSystem.at(row/3).getAt(2));
            a.setAt(row,1,0.0);
            a.setAt(row,2,-locSystem.at(row/3).getAt(0));
            a.setAt(row,3,0.0);
            a.setAt(row,4,1.0);
            a.setAt(row,5,0.0);
        }else if((row+1) % 3 == 0){ //z
            a.setAt(row,0,-locSystem.at(row/3).getAt(1));
            a.setAt(row,1,locSystem.at(row/3).getAt(0));
            a.setAt(row,2,0.0);
            a.setAt(row,3,0.0);
            a.setAt(row,4,0.0);
            a.setAt(row,5,1.0);
        }
    }
    return a;
}

/*!
 * \brief fillLVector
 * \return
 */
OiVec Helmert6Param::fillLVector()
{
    OiVec l(this->locSystem.length()*3);
    for(int row=0;row<this->locSystem.length()*3;row++){
        if((row+1)%3 == 1){ //x observation
            l.setAt(row,this->locSystem.at(row/3).getAt(0));
        }
        if((row+1)%3 == 2){ // y observation
            l.setAt(row,this->locSystem.at(row/3).getAt(1));
        }
        if((row+1)%3 == 0){ // z observation
            l.setAt(row,this->locSystem.at(row/3).getAt(2));
        }
    }
    return l;
}

/*!
 * \brief fillL0Vector
 * \param x0
 * \return
 */
OiVec Helmert6Param::fillL0Vector(OiVec x0)
{
    OiVec l0(this->locSystem.length() * 3);

    for(int row = 0; row < this->locSystem.length() * 3; row++){
        if( (row+1) % 3 == 1 ){ //observation of x
            l0.setAt(row, this->locSystem.at(row/3).getAt(0) + x0.getAt(2) * this->locSystem.at(row/3).getAt(1) -  x0.getAt(1) * this->locSystem.at(row/3).getAt(2) + x0.getAt(3));
        }else if( (row+1) % 3 == 2 ){ //observation of y
            l0.setAt(row, this->locSystem.at(row/3).getAt(1) - x0.getAt(2) * this->locSystem.at(row/3).getAt(0) + x0.getAt(0) * this->locSystem.at(row/3).getAt(2) + x0.getAt(4));
        }else if( (row+1) % 3 == 0 ){ //observation of z
            l0.setAt(row, this->locSystem.at(row/3).getAt(2) + x0.getAt(1) * this->locSystem.at(row/3).getAt(0) - x0.getAt(0) * this->locSystem.at(row/3).getAt(1) + x0.getAt(5));
        }
    }
    return l0;
}

/*!
 * \brief preliminaryTransformation transforms the loc-system to pseudo-loc system with previoulsy approximated rotation and translation.
 */
void Helmert6Param::preliminaryTransformation()
{
    //get rotation matrix of current rotation angles
    OiMat rot = this->getRotationMatrix(this->rotation);

    QList<OiVec> tmpLoc;
    for(int i=0; i<this->locSystem.size();i++){
        //get vector point i
        OiVec tmp = this->locSystem.at(i);
        tmp.setAt(3,1.0);


        //rotate the point
        OiVec rt = rot*tmp;

        //add translation
        OiVec tmptrafo = this->translation + rt;
        tmptrafo.setAt(3,1.0);
        tmpLoc.append(tmptrafo);
    }
    this->locSystem = tmpLoc;
}

/*!
 * \brief approxRotation
 * \return
 */
OiVec Helmert6Param::approxRotation()
{
    vector<OiVec> centroidCoords = this->calcCentroidCoord(); //centroid coordinates
        if(centroidCoords.size() == 2){
            vector<OiVec> locC = this->centroidReducedCoord(locSystem, centroidCoords.at(0)); //centroid reduced destination coordinates
            vector<OiVec> refC = this->centroidReducedCoord(refSystem, centroidCoords.at(1)); //centroid reduced target coordinates
            vector<OiMat> modelMatrices = this->modelMatrix(locC, refC); //vector of model matrices - one for each common point
            OiMat n = this->normalEquationMatrix(modelMatrices); //calculate the normal equation matrix
            OiVec q = this->quaternion(n);
            if( !svdError ){
                OiMat r = this->rotationMatrix(q); //fill rotation matrix
                OiVec result = this->getRotationAngles(r);
                if(result.getAt(0) == -0.0){
                    result.setAt(0,0.0);
                }
                if(result.getAt(1) == -0.0){
                    result.setAt(1,0.0);
                }
                if(result.getAt(2) == -0.0){
                    result.setAt(2,0.0);
                }
                return result;
            }
        }
}

/*!
 * \brief approxTranslation
 * \param rot
 * \return
 */
OiVec Helmert6Param::approxTranslation(OiVec rot)
{
    //get rotation matrix of approx values
    OiMat r = this->getRotationMatrix(rot);

    vector<OiVec> centroidCoords = this->calcCentroidCoord(); //centroid coordinates

    //centroid point of reference system
    OiVec centroidPref = centroidCoords.at(1);
    centroidPref.add(1.0);

    //get centroid point of loc system
    OiVec centroidPloc = centroidCoords.at(0);
    centroidPloc.add(1.0);

    //rotate local centroid point to reference system
    OiVec tmp = centroidPloc;
    OiVec rt = r *tmp;
    OiVec trans = centroidPref - rt;
    trans.setAt(3,1.0);

    return trans;
}

/*!
 * \brief applyMovements applys the first inverse movement to the nominal values to have a homogeneous system
 * Otherwise the expanded measured points will result in incorrect translation values to the nominal data
 * The thermal expansion of an part object should have no influence on this transformation, so it is necessary to
 * apply the movement to the nominal values. This results in an homogeneous system that has no temperature
 * influence. The expansion is compensated with another function.
 * If you don´t eliminate the influence of the expansion in this transformation, you compensate it twice and get wrong
 * correction values.
 * You can only expand the nominal points, because the actual points that should be compensated are not in the PART system
 * yet. So you need to go the inverse way.
 * \param tp
 */
void Helmert6Param::applyMovements(TrafoParam &tp)
{
    FunctionConfiguration myConfig = this->getFunctionConfiguration();
    QString use = "";

    QMap<QString, QString> stringParameter = myConfig.stringParameter;


    if(stringParameter.contains("useTempComp")){
        use = static_cast<QString>(stringParameter.find("useTempComp").value());
    }
    //if temperature compensation should be used
    if(use.compare("true") == 0){
        bool stationStart = false;
        bool stationDest = false;

        //find the coordsys that has movements
        //parts don´t have movements
        stationStart = this->getCoordSysWithMovements(tp.getStartSystem());
        stationDest = this->getCoordSysWithMovements(tp.getDestinationSystem());

        TrafoParam *t = NULL;
        OiVec expansionOrigin(4);

        //get movement parameters and expansion origin
        if(stationStart && !stationDest){
             t = this->getMovement(tp.getStartSystem());
             expansionOrigin = tp.getDestinationSystem()->getExpansionOrigin();
        }else if(!stationStart && stationDest){
            t = this->getMovement(tp.getDestinationSystem());
            expansionOrigin = tp.getStartSystem()->getExpansionOrigin();
        }

        if(t != NULL){

            //expand nominals with inverse of movement parameters
            for(int i=0; i<this->refSystem.size();i++){
                OiVec refP = this->refSystem.at(i);

                refP = refP - expansionOrigin;
                refP = t->getHomogenMatrix().inv()*refP;
                refP = refP + expansionOrigin;
                refP.setAt(3,1.0);

                this->refSystem.replace(i,refP);
            }
            this->protocol.append("reference points were expanded with inverse of");
            this->protocol.append(" movement transformation to get correct translation values.");
        }
    }
}

/*!
 * \brief getRotationMatrix from rotation angles
 * \param rot
 * \return
 */
OiMat Helmert6Param::getRotationMatrix(OiVec rot)
{
    OiMat result(4,4);
    double alpha = rot.getAt(0);
    double beta = rot.getAt(1);
    double gamma = rot.getAt(2);
    result.setAt(0,0,qCos(beta)*qCos(gamma));
    result.setAt(0,1,qCos(alpha)*qSin(gamma)+qSin(alpha)*qSin(beta)*qCos(gamma));
    result.setAt(0,2,qSin(alpha)*qSin(gamma)-qCos(alpha)*qSin(beta)*qCos(gamma));
    result.setAt(0,3,0.0);
    result.setAt(1,0,-qCos(beta)*qSin(gamma));
    result.setAt(1,1,qCos(alpha)*qCos(gamma)-qSin(alpha)*qSin(beta)*qSin(gamma));
    result.setAt(1,2,qSin(alpha)*qCos(gamma)+qCos(alpha)*qSin(beta)*qSin(gamma));
    result.setAt(1,3,0.0);
    result.setAt(2,0,qSin(beta));
    result.setAt(2,1,-qSin(alpha)*qCos(beta));
    result.setAt(2,2,qCos(alpha)*qCos(beta));
    result.setAt(2,3,0.0);
    result.setAt(3,0,0.0);
    result.setAt(3,1,0.0);
    result.setAt(3,2,0.0);
    result.setAt(3,3,1.0);

    return result;
}

/*!
 * \brief getRotationAngles from rotation matrix
 * \param r
 * \return
 */
OiVec Helmert6Param::getRotationAngles(OiMat r)
{
    OiVec rot(3);
    rot.setAt(0, qAtan2(-r.getAt(2,1), r.getAt(2,2))); //alpha
    rot.setAt(1, qAsin(r.getAt(2,0))); //beta
    rot.setAt(2, qAtan2(-r.getAt(1,0), r.getAt(0,0))); //gamma
    if( qFabs(qCos(rot.getAt(1)) * qCos(rot.getAt(2))) - qFabs(r.getAt(0,0)) > 0.01 ){
        rot.setAt(1, PI - rot.getAt(1));
    }
    rot.add(1.0);

    return rot;
}

/*!
 * \brief getTranslationMatrix
 * \param trans
 * \return
 */
OiMat Helmert6Param::getTranslationMatrix(OiVec trans)
{
    OiMat tmpTranslation(4,4);

    tmpTranslation.setAt(0,0,1.0);
    tmpTranslation.setAt(0,1,0.0);
    tmpTranslation.setAt(0,2,0.0);
    tmpTranslation.setAt(0,3,trans.getAt(0));
    tmpTranslation.setAt(1,0,0.0);
    tmpTranslation.setAt(1,1,1.0);
    tmpTranslation.setAt(1,2,0.0);
    tmpTranslation.setAt(1,3,trans.getAt(1));
    tmpTranslation.setAt(2,0,0.0);
    tmpTranslation.setAt(2,1,0.0);
    tmpTranslation.setAt(2,2,1.0);
    tmpTranslation.setAt(2,3,trans.getAt(2));
    tmpTranslation.setAt(3,0,0.0);
    tmpTranslation.setAt(3,1,0.0);
    tmpTranslation.setAt(3,2,0.0);
    tmpTranslation.setAt(3,3,1.0);

    return tmpTranslation;
}

/*!
 * \brief getScaleMatrix
 * \param s
 * \return
 */
OiMat Helmert6Param::getScaleMatrix(OiVec s)
{
    OiMat tmpScale(4,4);

    tmpScale.setAt(0,0,s.getAt(0));
    tmpScale.setAt(0,1,0.0);
    tmpScale.setAt(0,2,0.0);
    tmpScale.setAt(0,3,0.0);
    tmpScale.setAt(1,0,0.0);
    tmpScale.setAt(1,1,s.getAt(1));
    tmpScale.setAt(1,2,0.0);
    tmpScale.setAt(1,3,0.0);
    tmpScale.setAt(2,0,0.0);
    tmpScale.setAt(2,1,0.0);
    tmpScale.setAt(2,2,s.getAt(2));
    tmpScale.setAt(2,3,0.0);
    tmpScale.setAt(3,0,0.0);
    tmpScale.setAt(3,1,0.0);
    tmpScale.setAt(3,2,0.0);
    tmpScale.setAt(3,3,1.0);

    return tmpScale;
}

/*!
 * \brief getCoordSysWithMovements checks if the given coordinate systen has movement transformations
 * \param cs
 * \return
 */
bool Helmert6Param::getCoordSysWithMovements(CoordinateSystem *cs)
{
    bool result = false;

    for(int i=0; i<cs->getTransformationParameters().size();i++){
        if(cs->getTransformationParameters().at(i)->getIsMovement() &&
                cs->getTransformationParameters().at(i)->getIsUsed()){
            //if has movements and they are on "use" state
            result = true;
        }
    }

    return result;
}

/*!
 * \brief getMovement returns the movement of the given coordinate system.
 * If the coord. sys. has more than one movement it will return the movement with the lowest valid time
 * \param cs
 * \return
 */
TrafoParam *Helmert6Param::getMovement(CoordinateSystem *cs)
{
    TrafoParam *tp = NULL;
    QMap<QDateTime,TrafoParam*> movements;

    for(int i=0; i<cs->getTransformationParameters().size();i++){
        if(cs->getTransformationParameters().at(i)->getIsMovement() &&
                cs->getTransformationParameters().at(i)->getIsUsed()){

            movements.insert(cs->getTransformationParameters().at(i)->getValidTime(),cs->getTransformationParameters().at(i));
        }
    }

    if(movements.size() > 0){
        tp = movements.values().at(0);
    }


    return tp;
}
