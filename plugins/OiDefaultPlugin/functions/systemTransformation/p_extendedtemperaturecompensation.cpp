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
    metaData->name = "ExtendedTempComp";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "jw";
    metaData->description = QString("%1 %2").arg("This function is a helmert transformation with temperature compensation.")
            .arg("it calculates 3 scales, translation and rotation. You need at least 3 common points to calculate.");
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
    if(this->isValid()){
        this->init();
        if(locSystem.count() == refSystem.count() && locSystem.count()>2){
            this->translation = this->approxTranslation();
            this->rotation = this->approxRotation(this->translation);
            this->scale = this->approxScale();
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

    this->preliminaryTransformation();

    OiVec tmpTranslation = this->approxTranslation();
    OiVec tmpRotation = this->approxRotation(tmpTranslation);
    OiVec tmpScale = this->approxScale();

    this->translation = this->translation + tmpTranslation;
    this->translation.setAt(3,1.0);
    this->rotation = this->rotation + tmpRotation;
    this->rotation.setAt(3,1.0);
    this->scale.setAt(0,this->scale.getAt(0)*tmpScale.getAt(0));
    this->scale.setAt(1,this->scale.getAt(1)*tmpScale.getAt(1));
    this->scale.setAt(2,this->scale.getAt(2)*tmpScale.getAt(2));
    //this->scale = this->scale * tmpScale;
    this->scale.setAt(3,1.0);

    //approximation
    OiVec x0(9);
    if(useTemp){
        x0.setAt(0,tmpRotation.getAt(0));
        x0.setAt(1,tmpRotation.getAt(1));
        x0.setAt(2,tmpRotation.getAt(2));
        x0.setAt(3,this->actTemp);
        x0.setAt(4,this->actTemp);
        x0.setAt(5,this->actTemp);
        x0.setAt(6,tmpTranslation.getAt(0));
        x0.setAt(7,tmpTranslation.getAt(1));
        x0.setAt(8,tmpTranslation.getAt(2));
    }else{
        x0.setAt(0,tmpRotation.getAt(0));
        x0.setAt(1,tmpRotation.getAt(1));
        x0.setAt(2,tmpRotation.getAt(2));
        x0.setAt(3,tmpScale.getAt(0));
        x0.setAt(4,tmpScale.getAt(1));
        x0.setAt(5,tmpScale.getAt(2));
        x0.setAt(6,tmpTranslation.getAt(0));
        x0.setAt(7,tmpTranslation.getAt(1));
        x0.setAt(8,tmpTranslation.getAt(2));
    }


    //observations
    OiVec l = this->fillLVector();

    OiVec l_diff;
    OiMat a;
    OiVec x;
    OiMat qxx;

    //iterate until solution is ok
    int iterations =0;
    OiVec stop;
    do{
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

        //get stop criteria
        stop = x.t()*x;
        iterations++;

    }while(stop.getAt(0) > (1/10000000000) && iterations < 100);

    if( stop.getAt(0) <= (1/10000000000) ){ //adjustment successful
            OiVec v = a * x - l_diff;
            OiVec vtv = v.t() * v;
            double s0_post = sqrt(vtv.getAt(0) / (3 * this->locSystem.length() - 7));
            OiMat sxx = s0_post * s0_post * qxx;

            tp.setTranslation(this->translation.getAt(0)+x0.getAt(6),this->translation.getAt(1)+x0.getAt(7),this->translation.getAt(2)+x0.getAt(8));
            tp.setRotation(this->rotation.getAt(0)+x0.getAt(0),this->rotation.getAt(1)+x0.getAt(1),this->rotation.getAt(2)+x0.getAt(2));

            if(useTemp){
                double sx = 1+((x0.getAt(3)-refTemp)*expansionCoefficient);
                double sy = 1+((x0.getAt(4)-refTemp)*expansionCoefficient);
                double sz = 1+((x0.getAt(5)-refTemp)*expansionCoefficient);
                tp.setScale(sx,sy,sz);
            }else{
                tp.setScale(this->scale.getAt(0)*x0.getAt(3),this->scale.getAt(1)*x0.getAt(4),this->scale.getAt(2)*x0.getAt(5));
            }

            result = true;
        }

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
        value.append(Configuration::sSteel);
        value.append(Configuration::sAluminum);
        value.append(Configuration::sPlumb);
        value.append(Configuration::sIron);
        value.append(Configuration::sGrayCastIron);
        value.append(Configuration::sCopper);
        value.append(Configuration::sBrass);
        value.append(Configuration::sZinc);
        value.append(Configuration::sPlatinum);
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
    QStringList result;
    result.append("no protocoll set");
    return result;
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
        expansionCoefficient = Configuration::getExpansionCoefficient(mat);
    }
    //
    if(stringParameter.contains("useTemperature")){
        QString temp = static_cast<QString>(stringParameter.find("useTemperature").value());
        if(temp.compare("true")== 0){
            useTemp = true;
        }else{
            useTemp = false;
        }
    }
    //get reference temperature from double parameter
    if(doubleParameter.contains("referenceTemperature")){
        refTemp = static_cast<double>(doubleParameter.find("referenceTemperature").value());
    }
    //get actual temperature from double parameter
    if(doubleParameter.contains("actualTemperature")){
        actTemp = static_cast<double>(doubleParameter.find("actualTemperature").value());
    }
    //get temperature accuracy from double parameter
    if(doubleParameter.contains("temperatureAccuracy")){
        tempAccuracy = static_cast<double>(doubleParameter.find("temperatureAccuracy").value());
    }
}



/*!
 * \brief approxTranslation calculates approximated translation of given local and reference system.
 * \param points
 * \return
 */
OiVec ExtendedTemperatureCompensation::approxTranslation()
{

    OiVec centroidPloc = this->calcCentroidPoint(this->locSystem);
    OiVec centroidPref = this->calcCentroidPoint(this->refSystem);

    OiVec trans = centroidPref - centroidPloc;
    trans.add(1.0);

    return trans;
}

/*!
 * \brief approxRotation calculates the rotation between the given local and reference system
 */
/*OiVec ExtendedTemperatureCompensation::approxRotation()
{
    QList<OiVec> tmpLocalSys;

    //add translation values to local coordinates
    for(int i=0; i<this->locSystem.size();i++){
        OiVec v = this->locSystem.at(i);
        v = v + this->translation;
        tmpLocalSys.append(v);
    }

    //calculate approximated rotations between the systems
    OiVec ls1 = tmpLocalSys.at(1);
    OiVec ls0 = tmpLocalSys.at(0);
    OiVec lr1 = this->refSystem.at(1);

    OiVec loc = ls1 - ls0;  //vector from point 0 to 1 in local system
    OiVec locRef = lr1 - ls0; // vector from point 0 (local) to point 1 (ref system)

    double rotx = 0.0;
    double roty = 0.0;
    double rotz = 0.0;

    //get rotation about z axis. Set z to zero for this
    rotz = (loc.getAt(0)*locRef.getAt(0)+loc.getAt(1)*locRef.getAt(1))/(qFabs(qSqrt(loc.getAt(0)*loc.getAt(0)+loc.getAt(1)*loc.getAt(1)))*qFabs(qSqrt(locRef.getAt(0)*locRef.getAt(0)+locRef.getAt(1)*locRef.getAt(1))));
    //get rotation about y axis. Set y to zero for this
    roty = (loc.getAt(0)*locRef.getAt(0)+loc.getAt(2)*locRef.getAt(2))/(qFabs(qSqrt(loc.getAt(0)*loc.getAt(0)+loc.getAt(2)*loc.getAt(2)))*qFabs(qSqrt(locRef.getAt(0)*locRef.getAt(0)+locRef.getAt(2)*locRef.getAt(2))));
    //get rotation about x axis. Set x to zero for this
    rotx = (loc.getAt(1)*locRef.getAt(1)+loc.getAt(2)*locRef.getAt(2))/(qFabs(qSqrt(loc.getAt(1)*loc.getAt(1)+loc.getAt(2)*loc.getAt(2)))*qFabs(qSqrt(locRef.getAt(1)*locRef.getAt(1)+locRef.getAt(2)*locRef.getAt(2))));

    OiVec rot;
    rot.add(rotx);
    rot.add(roty);
    rot.add(rotz);

    return rot;
}*/

/*!
 * \brief ExtendedTemperatureCompensation::approxRotation calculates the rotation between the given local and reference system
 * \param Translation t
 * \return
 */
OiVec ExtendedTemperatureCompensation::approxRotation(OiVec t)
{
    QList<OiVec> tmpLocalSys;

    //add translation values to local coordinates
    for(int i=0; i<this->locSystem.size();i++){
        OiVec v = this->locSystem.at(i);
        v = v + t;
        v.setAt(3,1.0);
        tmpLocalSys.append(v);
    }

    //calculate approximated rotations between the systems
    OiVec ls1 = tmpLocalSys.at(1);
    OiVec ls0 = tmpLocalSys.at(0);
    OiVec lr1 = this->refSystem.at(1);

    OiVec loc = ls1 - ls0;  //vector from point 0 to 1 in local system
    OiVec locRef = lr1 - ls0; // vector from point 0 (local) to point 1 (ref system)

    double rotx = 0.0;
    double roty = 0.0;
    double rotz = 0.0;

    //get rotation about z axis. Set z to zero for this
    rotz = qAcos((loc.getAt(0)*locRef.getAt(0)+loc.getAt(1)*locRef.getAt(1))/(qFabs(qSqrt(loc.getAt(0)*loc.getAt(0)+loc.getAt(1)*loc.getAt(1)))*qFabs(qSqrt(locRef.getAt(0)*locRef.getAt(0)+locRef.getAt(1)*locRef.getAt(1)))));
    //get rotation about y axis. Set y to zero for this
    roty = qAcos((loc.getAt(0)*locRef.getAt(0)+loc.getAt(2)*locRef.getAt(2))/(qFabs(qSqrt(loc.getAt(0)*loc.getAt(0)+loc.getAt(2)*loc.getAt(2)))*qFabs(qSqrt(locRef.getAt(0)*locRef.getAt(0)+locRef.getAt(2)*locRef.getAt(2)))));
    //get rotation about x axis. Set x to zero for this
    rotx = qAcos((loc.getAt(1)*locRef.getAt(1)+loc.getAt(2)*locRef.getAt(2))/(qFabs(qSqrt(loc.getAt(1)*loc.getAt(1)+loc.getAt(2)*loc.getAt(2)))*qFabs(qSqrt(locRef.getAt(1)*locRef.getAt(1)+locRef.getAt(2)*locRef.getAt(2)))));


    //again with other points to eliminate special cases
    //vectors are equal one axis => the oher two rotations can´t be calculated
    ls1 = tmpLocalSys.at(2);
    ls0 = tmpLocalSys.at(0);
    lr1 = this->refSystem.at(2);

    loc = ls1 - ls0;  //vector from point 0 to 1 in local system
    locRef = lr1 - ls0; // vector from point 0 (local) to point 1 (ref system)

    double rotx2 = 0.0;
    double roty2 = 0.0;
    double rotz2 = 0.0;

    //get rotation about z axis. Set z to zero for this
    rotz2 = qAcos((loc.getAt(0)*locRef.getAt(0)+loc.getAt(1)*locRef.getAt(1))/(qFabs(qSqrt(loc.getAt(0)*loc.getAt(0)+loc.getAt(1)*loc.getAt(1)))*qFabs(qSqrt(locRef.getAt(0)*locRef.getAt(0)+locRef.getAt(1)*locRef.getAt(1)))));
    //get rotation about y axis. Set y to zero for this
    roty2 = qAcos((loc.getAt(0)*locRef.getAt(0)+loc.getAt(2)*locRef.getAt(2))/(qFabs(qSqrt(loc.getAt(0)*loc.getAt(0)+loc.getAt(2)*loc.getAt(2)))*qFabs(qSqrt(locRef.getAt(0)*locRef.getAt(0)+locRef.getAt(2)*locRef.getAt(2)))));
    //get rotation about x axis. Set x to zero for this
    rotx2 = qAcos((loc.getAt(1)*locRef.getAt(1)+loc.getAt(2)*locRef.getAt(2))/(qFabs(qSqrt(loc.getAt(1)*loc.getAt(1)+loc.getAt(2)*loc.getAt(2)))*qFabs(qSqrt(locRef.getAt(1)*locRef.getAt(1)+locRef.getAt(2)*locRef.getAt(2)))));

    OiVec rot;

    if(rotx == 0){
        rot.add(rotx);
    }else if(rotx2 == 0){
        rot.add(rotx2);
    }else if(rotx>0){
        rot.add(rotx);
    }else{
        rot.add(rotx2);
    }

    if(roty == 0){
        rot.add(roty);
    }else if(rotx2 == 0){
        rot.add(roty2);
    }else if(roty>0){
        rot.add(roty);
    }else{
        rot.add(roty2);
    }

    if(rotz == 0){
        rot.add(rotz);
    }else if(rotz2 == 0){
        rot.add(rotz2);
    }else if(rotz>0){
        rot.add(rotz);
    }else{
        rot.add(rotz2);
    }

    rot.add(1.0);
    return rot;
}

/*!
 * \brief ExtendedTemperatureCompensation::approxScale calculates the scale depending on temperature
 */
OiVec ExtendedTemperatureCompensation::approxScale()
{
    OiVec s(3);
    double scale;
    double scaleLoc;
    double scaleRef;

    if(useTemp){
        //get approx of scale from temperature and expansion coefficient
        s.setAt(0,1+((actTemp-refTemp)*expansionCoefficient));
        s.setAt(1,1+((actTemp-refTemp)*expansionCoefficient));
        s.setAt(2,1+((actTemp-refTemp)*expansionCoefficient));
    }else{
        //get scale from distance between two points
        scaleLoc = qSqrt(qPow(this->locSystem.at(1).getAt(0)-this->locSystem.at(0).getAt(0),2)+qPow(this->locSystem.at(1).getAt(1)-this->locSystem.at(0).getAt(1),2)+qPow(this->locSystem.at(1).getAt(2)-this->locSystem.at(0).getAt(2),2));
        scaleRef = qSqrt(qPow(this->refSystem.at(1).getAt(0)-this->refSystem.at(0).getAt(0),2)+qPow(this->refSystem.at(1).getAt(1)-this->refSystem.at(0).getAt(1),2)+qPow(this->refSystem.at(1).getAt(2)-this->refSystem.at(0).getAt(2),2));
        scale = scaleRef/scaleLoc;
        s.setAt(0,scale);
        s.setAt(1,scale);
        s.setAt(2,scale);
        s.add(1.0);
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
 * rotation x, rotationy, rotation z, temp x, temp y, temp z, tx, ty, tz
 * \param x0
 * \return
 */
OiMat ExtendedTemperatureCompensation::fillAMatrix(OiVec x0)
{
    OiMat a(locSystem.length()*3,9);

    //loc system is transformed with approx values, so I use the rotation matrix for smale angles.

    if(useTemp){//use temperature and expansion coefficient as scale
        for(int row=0; row<locSystem.length()*3; row++){
            if((row+1) % 3 == 1){ // x
                a.setAt(row,0,0.0);
                a.setAt(row,1,-locSystem.at(row/3).getAt(2)*(1+(x0.getAt(3)-refTemp)*expansionCoefficient));
                a.setAt(row,2,locSystem.at(row/3).getAt(1)*(1+(x0.getAt(3)-refTemp)*expansionCoefficient));
                a.setAt(row,3,((1-refTemp)*expansionCoefficient)*(locSystem.at(row/3).getAt(0)+x0.getAt(2)*locSystem.at(row/3).getAt(1)-x0.getAt(1)*locSystem.at(row/3).getAt(2)));
                a.setAt(row,4,0.0);
                a.setAt(row,5,0.0);
                a.setAt(row,6,1.0);
                a.setAt(row,7,0.0);
                a.setAt(row,8,0.0);
            }else if((row+1) % 3 == 2){ //y
                a.setAt(row,0,locSystem.at(row/3).getAt(2)*(1+(x0.getAt(4)-refTemp)*expansionCoefficient));
                a.setAt(row,1,0.0);
                a.setAt(row,2,-locSystem.at(row/3).getAt(0)*(1+(x0.getAt(4)-refTemp)*expansionCoefficient));
                a.setAt(row,3,0.0);
                a.setAt(row,4,((1-refTemp)*expansionCoefficient)*(-x0.getAt(2)*locSystem.at(row/3).getAt(0)+locSystem.at(row/3).getAt(1)+x0.getAt(0)*locSystem.at(row/3).getAt(2)));
                a.setAt(row,5,0.0);
                a.setAt(row,6,0.0);
                a.setAt(row,7,1.0);
                a.setAt(row,8,0.0);
            }else if((row+1) % 3 == 0){ //z
                a.setAt(row,0,-locSystem.at(row/3).getAt(1)*(1+(x0.getAt(5)-refTemp)*expansionCoefficient));
                a.setAt(row,1,locSystem.at(row/3).getAt(0)*(1+(x0.getAt(5)-refTemp)*expansionCoefficient));
                a.setAt(row,2,0.0);
                a.setAt(row,3,0.0);
                a.setAt(row,4,0.0);
                a.setAt(row,5,((1-refTemp)*expansionCoefficient)*(x0.getAt(1)*locSystem.at(row/3).getAt(0)-x0.getAt(0)*locSystem.at(row/3).getAt(1)+locSystem.at(row/3).getAt(2)));
                a.setAt(row,6,0.0);
                a.setAt(row,7,0.0);
                a.setAt(row,8,1.0);
            }
        }
    }else{ // use scale from distance differences
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
    }
    return a;
}

/*!
 * \brief ExtendedTemperatureCompensation::rotationMatrix sets up the rotation matrix
 * \return
 */
OiMat ExtendedTemperatureCompensation::rotationMatrix()
{
    OiMat result(4,4);
    double alpha = this->rotation.getAt(0);
    double beta = this->rotation.getAt(1);
    double gamma = this->rotation.getAt(2);
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
    QList<OiVec> tmpLoc;
    for(int i=0; i<this->locSystem.size();i++){
        OiVec tmp = this->locSystem.at(i);
        OiMat tmpScale = this->scaleMatrix();
        OiMat rot = this->rotationMatrix();
        OiVec tmptrafo = this->translation+tmpScale*this->rotationMatrix()*tmp;
        tmpLoc.append(tmptrafo);
    }
    this->locSystem = tmpLoc;
}

/*!
 * \brief ExtendedTemperatureCompensation::scaleMatrix generates a matrix with the 3 scales on the main diagonal
 * \return
 */
OiMat ExtendedTemperatureCompensation::scaleMatrix()
{
    OiMat scaleMat(4,4);
    scaleMat.setAt(0,0,scale.getAt(0));
    scaleMat.setAt(0,1,0.0);
    scaleMat.setAt(0,2,0.0);
    scaleMat.setAt(0,3,0.0);
    scaleMat.setAt(1,0,0.0);
    scaleMat.setAt(1,1,scale.getAt(1));
    scaleMat.setAt(1,2,0.0);
    scaleMat.setAt(1,3,0.0);
    scaleMat.setAt(2,0,0.0);
    scaleMat.setAt(2,1,0.0);
    scaleMat.setAt(2,2,scale.getAt(2));
    scaleMat.setAt(2,3,0.0);
    scaleMat.setAt(3,0,0.0);
    scaleMat.setAt(3,1,0.0);
    scaleMat.setAt(3,2,0.0);
    scaleMat.setAt(3,3,1.0);

    return scaleMat;
}

/*!
 * \brief ExtendedTemperatureCompensation::fillL0Vector fills the l0 vector
 * \param x0
 * \return
 */
OiVec ExtendedTemperatureCompensation::fillL0Vector(OiVec x0)
{
    OiVec l0(this->locSystem.length() * 3);

    double sx, sy,sz;

    if(useTemp){
        sx = (1+(x0.getAt(3)-refTemp)*expansionCoefficient);
        sy = (1+(x0.getAt(4)-refTemp)*expansionCoefficient);
        sz = (1+(x0.getAt(5)-refTemp)*expansionCoefficient);
    }else{
        sx = x0.getAt(3);
        sy = x0.getAt(4);
        sz = x0.getAt(5);
    }

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


