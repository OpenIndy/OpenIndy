#include "p_extendedtemperaturecompensation.h"

ExtendedTemperatureCompensation::ExtendedTemperatureCompensation()
{
}

/*!
 * \brief getMetaData of the function
 * \return
 */
PluginMetaData *ExtendedTemperatureCompensation::getMetaData()
{
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "9ParameterHelmertTransformation";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "jw";
    metaData->description = QString("%1 %2 %3").arg("This is a 9 parameter helmert transformation. You can claculate the")
            .arg("parameters by the given points (at least 3) or by the given points and an additional temperature input.")
            .arg("So in the second case the scale gets approximated by the input actual and reference temperature and the material.");
    metaData->iid = "de.openIndy.Plugin.Function.SystemTransformation.v001";
    return metaData;
}

/*!
 * \brief getNeededElements of the function
 * \return
 */
QList<InputParams> ExtendedTemperatureCompensation::getNeededElements()
{
    QList<InputParams> result;
    InputParams param;
    param.description = "Select points for calculating the transformation.";
    param.infinite = true;
    param.typeOfElement = Configuration::ePointElement;
    result.append(param);
    return result;
}

/*!
 * \brief applicableFor
 * \return
 */
QList<Configuration::FeatureTypes> ExtendedTemperatureCompensation::applicableFor()
{
   QList<Configuration::FeatureTypes> result;
   result.append(Configuration::eTrafoParamFeature);
   return result;
}

/*!
 * \brief exec function of the class.
 * \return
 */
bool ExtendedTemperatureCompensation::exec(TrafoParam &tp)
{
    this->protocol.clear();
    this->svdError = false;

    if(this->isValid()){
        this->init();
        if(locSystem.count() == refSystem.count() && locSystem.count()>2){

            //get translation and rotation from loc to ref system, so you can transform later
            this->rotation = this->approxRotation();
            this->scale = this->approxScale(this->rotation);
            this->translation = this->approxTranslation(this->rotation,this->scale);

            if(locSystem.count()>3){
                return this->calc(tp);
            }else{
                tp.setRotation(this->rotation.getAt(0),this->rotation.getAt(1),this->rotation.getAt(2));
                tp.setTranslation(this->translation.getAt(0),this->translation.getAt(1),this->translation.getAt(2));
                tp.setScale(this->scale.getAt(0),this->scale.getAt(1),this->scale.getAt(2));
                return true;
            }
        }else{
            this->writeToConsole("Not enough common points");
            return false;
        }
    }else{
        this->writeToConsole("No valid points");
        return false;
    }
}

/*!
 * \brief calc the trafo param using iterations
 * \param tp
 * \return
 */
bool ExtendedTemperatureCompensation::calc(TrafoParam &tp)
{
    bool result = false;

    //transform loc (start system) to "pseudo"-loc system
    //transformation with previosly approximated translation, rotation and scale
    this->preliminaryTransformation(); 

    //get rotation between pseudo loc and ref system
    OiVec tmpRotation = this->approxRotation();

    //get scale between pseudo loc and ref system (using temperature or distance)
    OiVec tmpScale = this->approxScale(tmpRotation);

    //get translation between pseudo loc and ref system
    OiVec tmpTranslation = this->approxTranslation(tmpRotation,tmpScale);

    //approximation
    OiVec x0(9);

    x0.setAt(0,tmpRotation.getAt(0));
    x0.setAt(1,tmpRotation.getAt(1));
    x0.setAt(2,tmpRotation.getAt(2));
    x0.setAt(3,tmpScale.getAt(0));
    x0.setAt(4,tmpScale.getAt(1));
    x0.setAt(5,tmpScale.getAt(2));
    x0.setAt(6,tmpTranslation.getAt(0));
    x0.setAt(7,tmpTranslation.getAt(1));
    x0.setAt(8,tmpTranslation.getAt(2));

    //observations
    OiVec l = this->fillLVector();

    OiVec l_diff;
    OiMat a;
    OiVec x;
    OiMat qxx;

    //A matrix
    a = this->fillAMatrix(x0);
    //normal equation
    OiMat n = a.t()*a;

    try{
        qxx = n.inv(); // try to calc the inverse
    }catch(runtime_error e){
       return false;
    }catch(logic_error e){
        return false;
    }

    //differential l vector
    OiVec l0 = this->fillL0Vector(x0);
    l_diff = l -l0;

    //right side
    OiVec c = a.t()*l_diff;

    //calc x
    x = qxx*c;
    x0 = x0+x;

    OiVec v = a * x - l_diff;
    OiVec vtv = v.t() * v;
    double s0_post = sqrt(vtv.getAt(0) / (3 * this->locSystem.length() - 7));
    OiMat sxx = s0_post * s0_post * qxx;


    //get previously calculated scales
    double sx = this->scale.getAt(0);
    double sy = this->scale.getAt(1);
    double sz = this->scale.getAt(2);

    //set the trafo parameters with the previously calulated values and the additional values from adjustment
    tp.setTranslation(this->translation.getAt(0)+x0.getAt(6),this->translation.getAt(1)+x0.getAt(7),this->translation.getAt(2)+x0.getAt(8));
    tp.setRotation(this->rotation.getAt(0)+x0.getAt(0),this->rotation.getAt(1)+x0.getAt(1),this->rotation.getAt(2)+x0.getAt(2));
    tp.setScale(sx*x0.getAt(3),sy*x0.getAt(4),sz*x0.getAt(5));

    //calculate the representing temperature out of each scale to show in the protocoll
    if(useTemp){
        double tx = (((sx*x0.getAt(3))-1.0)+(this->refTemp*this->expansionCoefficient))/this->expansionCoefficient;
        this->protocol.append(QString("scale x representing an expansion of " + QString::number(tx,'f',2)+"[°C]"));
        double ty = (((sy*x0.getAt(4))-1.0)+(this->refTemp*this->expansionCoefficient))/this->expansionCoefficient;
        this->protocol.append(QString("scale y representing an expansion of " + QString::number(ty,'f',2)+"[°C]"));
        double tz = (((sz*x0.getAt(5))-1.0)+(this->refTemp*this->expansionCoefficient))/this->expansionCoefficient;
        this->protocol.append(QString("scale z representing an expansion of " + QString::number(tz,'f',2)+"[°C]"));
    }
    tp.generateHomogenMatrix();
    result = true;


    return result;
}

/*!
* \brief getStringParameter gets all additionally needed string parameters for the function
* \return
*/
QMap<QString, QStringList> ExtendedTemperatureCompensation::getStringParameter()
{
QMap<QString, QStringList> result;
    QString key ="material";
    QStringList value;
    value = Materials::getMaterials();
    result.insert(key,value);
    key = "useTemperature";
    value.clear();
    value.append("false");
    value.append("true");
    result.insert(key,value);

    return result;
}

/*!
* \brief getDoubleParameter gets all additionally needed double parameters for the function
* \return
*/
QMap<QString, double> ExtendedTemperatureCompensation::getDoubleParameter()
{
QMap<QString,double> result;
    QString key = "referenceTemperature";
    double value = 20.0;
    result.insert(key,value);
    key = "actualTemperature";
    value = 20.0;
    result.insert(key,value);
    key = "temperatureAccuracy";
    value = 0.1;
    result.insert(key, value);

    return result;
}

/*!
 * \brief getResultProtocol returns a protocoll of the function
 * \return
 */
QStringList ExtendedTemperatureCompensation::getResultProtocol()
{
    return this->protocol;
}

/*!
 * \brief ExtendedTemperatureCompensation::init fills local system and reference system vectors.
 */
void ExtendedTemperatureCompensation::init()
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

    this->getExtraParameter();
}

/*!
 * \brief getExtraParameter from input dialog of function dialog.
 */
void ExtendedTemperatureCompensation::getExtraParameter()
{
    FunctionConfiguration myConfig = this->getFunctionConfiguration();
    QMap<QString, QString> stringParameter = myConfig.stringParameter;
    QMap<QString, double> doubleParameter = myConfig.doubleParameter;

    refTemp = 0.0;
    actTemp = 0.0;
    expansionCoefficient = 0.0;
    tempAccuracy = 0.0;

    //get material from string parameter
    if(stringParameter.contains("material")){
        QString mat = static_cast<QString>(stringParameter.find("material").value());
        expansionCoefficient = Materials::getExpansionCoefficient(mat);
        this->protocol.append(QString("material: " + mat));
        this->protocol.append(QString("expansion coefficient: " + QString::number(expansionCoefficient,'f',6)));
    }
    //get parameter that decides if scale should be calculated from distances or temperature difference
    if(stringParameter.contains("useTemperature")){
        QString temp = static_cast<QString>(stringParameter.find("useTemperature").value());
        if(temp.compare("true")== 0){
            useTemp = true;
            this->protocol.append("use temperature as scale: yes");
        }else{
            useTemp = false;
            this->protocol.append("use temperature as scale: no");
        }
    }
    //get reference temperature from double parameter
    if(doubleParameter.contains("referenceTemperature")){
        refTemp = static_cast<double>(doubleParameter.find("referenceTemperature").value());
        this->protocol.append(QString("reference temperature: " + QString::number(refTemp,'f',2)));
    }
    //get actual temperature from double parameter
    if(doubleParameter.contains("actualTemperature")){
        actTemp = static_cast<double>(doubleParameter.find("actualTemperature").value());
        this->protocol.append(QString("actual temperature: " + QString::number(actTemp,'f',2)));
    }
    //get temperature accuracy from double parameter
    if(doubleParameter.contains("temperatureAccuracy")){
        tempAccuracy = static_cast<double>(doubleParameter.find("temperatureAccuracy").value());
        this->protocol.append(QString("accuracy of temperature measurement: " + QString::number(tempAccuracy,'f',2)));
    }
}

/*!
 * \brief approxTranslation calculates approximated translation of given local and reference system.
 * \param points
 * \return
 */
OiVec ExtendedTemperatureCompensation::approxTranslation(OiVec rot, OiVec s)
{
    //get rotation matrix of approx values
    OiMat r = this->getRotationMatrix(rot);

    //get scale matrix of approx valies
    OiMat scaleMat = this->scaleMatrix(s);

    //centroid point of reference system
    OiVec centroidPref = this->calcCentroidPoint(this->refSystem);
    centroidPref.add(1.0);

    //get centroid point of loc system
    OiVec centroidPloc = this->calcCentroidPoint(this->locSystem);
    centroidPloc.add(1.0);

    //rotate and scale local centroid point to reference system
    OiVec tmp = centroidPloc;
    OiVec st = scaleMat*tmp;
    OiVec rst = r *st;
    OiVec trans = centroidPref - rst;
    trans.setAt(3,1.0);

    return trans;
}

/*!
 * \brief approxRotation calculates the rotation between the given local and reference system
 * \param Translation t
 * \return
 */
OiVec ExtendedTemperatureCompensation::approxRotation()
{
    //calculate approximated rotation angles with Drixler algorithm
    //=> good approximation of preliminary transformation, so a rotation matrix with small angles can be used later at
    //adjusting the parameters iterativ

    vector<OiVec> centroidCoords;
    //get centroid coords
    centroidCoords.push_back(this->calcCentroidPoint(this->locSystem));
    centroidCoords.push_back(this->calcCentroidPoint(this->refSystem));
    if(centroidCoords.size() == 2){
        //centroid reduced coords
        vector<OiVec> locC = this->centroidReducedCoord(locSystem, centroidCoords.at(0));
        vector<OiVec> refC = this->centroidReducedCoord(refSystem, centroidCoords.at(1));
        vector<OiMat> modelMatrices = this->modelMatrix(locC, refC);
        OiMat n = this->normalEquationMatrix(modelMatrices);
        OiVec q = this->quaternion(n);
        if(!svdError){
            OiMat r = this->rotationMatrix(q);
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
 * \brief approxScale calculates the scale depending on temperature
 */
OiVec ExtendedTemperatureCompensation::approxScale(OiVec rot)
{
    OiVec s(4);

    //get the current rotation matrix
    OiMat rotMat;
    try{
       rotMat = this->getRotationMatrix(rot).inv(); // try to calc the inverse
    }catch(runtime_error e){
       return false;
    }catch(logic_error e){
        return false;
    }
    QList<OiVec> tmpRefList;

    //rotate the reference system to local system
    for(int i=0; i<this->refSystem.size(); i++){
        OiVec tmpRef = rotMat*this->refSystem.at(i);
        tmpRefList.append(tmpRef);
    }

    if(useTemp){
        //get approx of scale from temperature and expansion coefficient
        s.setAt(0,1.0/(1.0+((actTemp-refTemp)*expansionCoefficient)));
        s.setAt(1,1.0/(1.0+((actTemp-refTemp)*expansionCoefficient)));
        s.setAt(2,1.0/(1.0+((actTemp-refTemp)*expansionCoefficient)));
        s.setAt(3,1.0);
    }else{  //get scale from differces in distance components (x, y, z)
            //between two points in each system
        double sx = 0.0;
        double sy = 0.0;
        double sz = 0.0;

        for(int i=1; i<this->locSystem.size(); i++){
            //get x y and z component of vector from point 0 to i in loc system
            double sxLoc = qFabs(locSystem.at(0).getAt(0)-locSystem.at(i).getAt(0));
            double syLoc = qFabs(locSystem.at(0).getAt(1)-locSystem.at(i).getAt(1));
            double szLoc = qFabs(locSystem.at(0).getAt(2)-locSystem.at(i).getAt(2));

            //get x y and z component of vector from point 0 to i in ref system
            double sxRef = qFabs(tmpRefList.at(0).getAt(0)-tmpRefList.at(i).getAt(0));
            double syRef = qFabs(tmpRefList.at(0).getAt(1)-tmpRefList.at(i).getAt(1));
            double szRef = qFabs(tmpRefList.at(0).getAt(2)-tmpRefList.at(i).getAt(2));


            //check if the scale from current two points is bigger than the scale from last points
            //if yes, set current scale as actual scale for the coordinate component

            //0.005 is criteria, because of noisy measurements
            if(sxLoc <= 0.005 || sxRef <= 0.005){

            }else{
                if(sxRef/sxLoc > sx){sx = (sxRef/sxLoc);}
            }

            if(syLoc <= 0.005 || syRef <=0.005){

            }else{
                if(syRef/syLoc > sy){sy = (syRef/syLoc);}
            }

            if(szLoc <= 0.005 || szRef <= 0.005){

            }else{
                if(szRef/szLoc > sz){sz = (szRef/szLoc);}
            }
        }
        //if no scale could be calculated (noisy measurements),
        //set this scale component to 1.000000
        if(sx == 0.0){sx = 1.0;}
        if(sy == 0.0){sy = 1.0;}
        if(sz == 0.0){sz = 1.0;}

        s.setAt(0,sx);
        s.setAt(1,sy);
        s.setAt(2,sz);
        s.setAt(3,1.0);
    }

    return s;
}

/*!
 * \brief calcCentroidPoint of given points
 * \param points
 * \return
 */
OiVec ExtendedTemperatureCompensation::calcCentroidPoint(QList<OiVec> points)
{
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    for(int i=0;i<points.size();i++){
        x += points.at(i).getAt(0);
        y += points.at(i).getAt(1);
        z += points.at(i).getAt(2);
    }
    x = x/points.size();
    y = y/points.size();
    z = z/points.size();

    OiVec centroidPoint(3);
    centroidPoint.setAt(0,x);
    centroidPoint.setAt(1,y);
    centroidPoint.setAt(2,z);

    return centroidPoint;
}

/*!
 * \brief fillLVector with observations
 * \return
 */
OiVec ExtendedTemperatureCompensation::fillLVector()
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
 * \brief fillAMatrix fills the A matrix
 * rotation x, rotationy, rotation z, scale x, scale y, scale z, tx, ty, tz
 * \param x0
 * \return
 */
OiMat ExtendedTemperatureCompensation::fillAMatrix(OiVec x0)
{
    OiMat a(locSystem.length()*3,9);

    //loc system is transformed with approx values of translation and rotation. Scale is always near 1.00000, so
    //this should be approxed enough for the pre transformation.
    //That´s why I can use the rotation matrix for small angles.
    for(int row=0; row<locSystem.length()*3; row++){
        if((row+1) % 3 == 1){ // x
            a.setAt(row,0,0.0);
            a.setAt(row,1,-locSystem.at(row/3).getAt(2)*(x0.getAt(3)));
            a.setAt(row,2,locSystem.at(row/3).getAt(1)*(x0.getAt(3)));
            a.setAt(row,3,(locSystem.at(row/3).getAt(0)+x0.getAt(2)*locSystem.at(row/3).getAt(1)-x0.getAt(1)*locSystem.at(row/3).getAt(2)));
            a.setAt(row,4,0.0);
            a.setAt(row,5,0.0);
            a.setAt(row,6,1.0);
            a.setAt(row,7,0.0);
            a.setAt(row,8,0.0);
        }else if((row+1) % 3 == 2){ //y
            a.setAt(row,0,locSystem.at(row/3).getAt(2)*(x0.getAt(4)));
            a.setAt(row,1,0.0);
            a.setAt(row,2,-locSystem.at(row/3).getAt(0)*(x0.getAt(4)));
            a.setAt(row,3,0.0);
            a.setAt(row,4,(-x0.getAt(2)*locSystem.at(row/3).getAt(0)+locSystem.at(row/3).getAt(1)+x0.getAt(0)*locSystem.at(row/3).getAt(2)));
            a.setAt(row,5,0.0);
            a.setAt(row,6,0.0);
            a.setAt(row,7,1.0);
            a.setAt(row,8,0.0);
        }else if((row+1) % 3 == 0){ //z
            a.setAt(row,0,-locSystem.at(row/3).getAt(1)*(x0.getAt(5)));
            a.setAt(row,1,locSystem.at(row/3).getAt(0)*(x0.getAt(5)));
            a.setAt(row,2,0.0);
            a.setAt(row,3,0.0);
            a.setAt(row,4,0.0);
            a.setAt(row,5,(x0.getAt(1)*locSystem.at(row/3).getAt(0)-x0.getAt(0)*locSystem.at(row/3).getAt(1)+locSystem.at(row/3).getAt(2)));
            a.setAt(row,6,0.0);
            a.setAt(row,7,0.0);
            a.setAt(row,8,1.0);
        }
    }
    return a;
}

/*!
 * \brief rotationMatrix sets up the rotation matrix
 * \return
 */
OiMat ExtendedTemperatureCompensation::getRotationMatrix(OiVec rot)
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
 * \brief preliminaryTransformation calculates the first approxiamted transformation
 */
void ExtendedTemperatureCompensation::preliminaryTransformation()
{
    //get rotation matrix of current rotation angles
    OiMat rot = this->getRotationMatrix(this->rotation);

    //get scale matrix of current scales
    OiMat s = this->scaleMatrix(this->scale);

    QList<OiVec> tmpLoc;
    for(int i=0; i<this->locSystem.size();i++){
        //get vector point i
        OiVec tmp = this->locSystem.at(i);
        tmp.setAt(3,1.0);

        //scale the point
        OiVec st = s*tmp;
        //rotate the point
        OiVec rst = rot*st;

        //add translation
        OiVec tmptrafo = this->translation + rst;
        tmptrafo.setAt(3,1.0);
        tmpLoc.append(tmptrafo);
    }
    this->locSystem = tmpLoc;
}

/*!
 * \brief scaleMatrix generates a matrix with the 3 scales on the main diagonal
 * \return
 */
OiMat ExtendedTemperatureCompensation::scaleMatrix(OiVec s)
{
    OiMat scaleMat(4,4);
    scaleMat.setAt(0,0,s.getAt(0));
    scaleMat.setAt(0,1,0.0);
    scaleMat.setAt(0,2,0.0);
    scaleMat.setAt(0,3,0.0);
    scaleMat.setAt(1,0,0.0);
    scaleMat.setAt(1,1,s.getAt(1));
    scaleMat.setAt(1,2,0.0);
    scaleMat.setAt(1,3,0.0);
    scaleMat.setAt(2,0,0.0);
    scaleMat.setAt(2,1,0.0);
    scaleMat.setAt(2,2,s.getAt(2));
    scaleMat.setAt(2,3,0.0);
    scaleMat.setAt(3,0,0.0);
    scaleMat.setAt(3,1,0.0);
    scaleMat.setAt(3,2,0.0);
    scaleMat.setAt(3,3,1.0);

    return scaleMat;
}

/*!
 * \brief fillL0Vector fills the l0 vector
 * \param x0
 * \return
 */
OiVec ExtendedTemperatureCompensation::fillL0Vector(OiVec x0)
{
    OiVec l0(this->locSystem.length() * 3);

    double sx, sy, sz;

    sx = x0.getAt(3);
    sy = x0.getAt(4);
    sz = x0.getAt(5);

    for(int row = 0; row < this->locSystem.length() * 3; row++){
        if( (row+1) % 3 == 1 ){ //observation of x
            l0.setAt(row, sx * this->locSystem.at(row/3).getAt(0) + sx * x0.getAt(2) * this->locSystem.at(row/3).getAt(1) - sx * x0.getAt(1) * this->locSystem.at(row/3).getAt(2) + x0.getAt(6));
        }else if( (row+1) % 3 == 2 ){ //observation of y
            l0.setAt(row, sy * this->locSystem.at(row/3).getAt(1) - sy * x0.getAt(2) * this->locSystem.at(row/3).getAt(0) + sy * x0.getAt(0) * this->locSystem.at(row/3).getAt(2) + x0.getAt(7));
        }else if( (row+1) % 3 == 0 ){ //observation of z
            l0.setAt(row, sz * this->locSystem.at(row/3).getAt(2) + sz * x0.getAt(1) * this->locSystem.at(row/3).getAt(0) - sz * x0.getAt(0) * this->locSystem.at(row/3).getAt(1) + x0.getAt(8));
        }
    }
    return l0;
}

/*!
 * \brief centroidReducedCoord calculates the centroid reduced coordinates
 * \param input
 * \param centroid
 * \return
 */
vector<OiVec> ExtendedTemperatureCompensation::centroidReducedCoord(QList<OiVec> input, OiVec centroid)
{
    vector<OiVec> result;
    foreach (OiVec coord, input) {
       OiVec c;
       c.add(coord.getAt(0));
       c.add(coord.getAt(1));
       c.add(coord.getAt(2));
       OiVec rc;
       rc = c-centroid;
       result.push_back(rc);
    }

    return result;
}

/*!
 * \brief modelMatrix
 * \param locC
 * \param refC
 * \return
 */
vector<OiMat> ExtendedTemperatureCompensation::modelMatrix(vector<OiVec> locC, vector<OiVec> refC)
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
 * \param vecA
 * \return
 */
OiMat ExtendedTemperatureCompensation::normalEquationMatrix(vector<OiMat> vecA)
{
    OiMat result(4, 4);
    foreach(OiMat a, vecA){
        result +=  a.t() * a;
    }
    return result;
}

/*!
 * \brief quaternion; SVD to get the transformation vector (quaternion)
 * \param n
 * \return
 */
OiVec ExtendedTemperatureCompensation::quaternion(OiMat n)
{
    OiVec q(4);
    try{
        OiMat u;
        OiMat v;
        OiVec s;
        n.svd(u, s, v);
        q = u.getCol(3);
    }catch(runtime_error& e){
        svdError = true;
    }
    return q;
}

/*!
 * \brief rotationMatrix
 * \param q
 * \return
 */
OiMat ExtendedTemperatureCompensation::rotationMatrix(OiVec q)
{
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
 * \brief getRotationAngles
 * \param r
 * \return
 */
OiVec ExtendedTemperatureCompensation::getRotationAngles(OiMat r)
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


