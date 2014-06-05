#include "p_pointfrompoints.h"

/*!
 * \brief PointFromPoints::getMetaData
 * \return
 */
PluginMetaData* PointFromPoints::getMetaData(){
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "PointFromPoints";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "br";
    metaData->description = QString("%1 %2")
            .arg("This function calculates an adjusted point.")
            .arg("You can input as many points as you want which are then used to find the best fit 3D point.");
    metaData->iid = "de.openIndy.Plugin.Function.ConstructFunction.v001";
    return metaData;
}

/*!
 * \brief PointFromPoints::getNeededElements
 * \return
 */
QList<InputParams> PointFromPoints::getNeededElements(){
    QList<InputParams> result;
    InputParams param;
    param.index = 0;
    param.description = "Select points to calculate the best fit point.";
    param.infinite = true;
    param.typeOfElement = Configuration::ePointElement;
    result.append(param);
    return result;
}

/*!
 * \brief PointFromPoints::applicableFor
 * \return
 */
QList<Configuration::FeatureTypes> PointFromPoints::applicableFor(){
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::ePointFeature);
    return result;
}

/*!
 * \brief PointFromPoints::exec
 * \param p
 * \return
 */
bool PointFromPoints::exec(Point &p){
    if(this->isValid() && this->checkPointCount()){
        this->setUpPointResult( p );
        return true;
    }else{
        this->writeToConsole("Not enough points available for calculation");
        return false;
    }
}

/*!
 * \brief PointFromPoints::setUpResult
 * Set up result and statistic for type point
 * \param point
 */
void PointFromPoints::setUpPointResult(Point &point){
    //Fill l vector
    OiVec l;
    foreach(Point *p, this->points){
        if(p->getIsSolved()){
            l.add( p->xyz.getAt(0) );
            l.add( p->xyz.getAt(1) );
            l.add( p->xyz.getAt(2) );
        }
    }
    if(l.getSize() > 0){
        //Fill A matrix
        OiMat a(l.getSize(), 3);
        for(int i = 0; i < l.getSize(); i++){
            if( (i%3) == 0 ){
                a.setAt(i, Point::unknownX, 1.0);
            }else if( (i%3) == 1 ){
                a.setAt(i, Point::unknownY, 1.0);
            }else if( (i%3) == 2 ){
                a.setAt(i, Point::unknownZ, 1.0);
            }
        }
        //Adjust
        OiMat n = a.t() * a;
        OiVec c = a.t() * l;
        try{
            OiMat qxx = n.inv();
            OiVec x = qxx * c;
            OiVec v = a * x - l;
            x.add(1.0);
            //set point
            point.xyz = x;
            Statistic myStats;
            myStats.qxx = qxx;
            myStats.v = v;
            myStats.isValid = true;
            point.setStatistic(myStats);
            this->myStatistic = point.getStatistic();
        }catch(logic_error e){
            this->writeToConsole(e.what());
            return;
        }catch(runtime_error e){
            this->writeToConsole(e.what());
            return;
        }
    }else{
        this->writeToConsole("Not enough valid points available for calculation");
    }

}

/*!
 * \brief PointFromPoints::checkPointCount
 * Check wether there are enough valid points for calculation
 * \return
 */
bool PointFromPoints::checkPointCount(){
    int count = 0;
    foreach(Point *p, this->points){
        if(p->getIsSolved()){
            this->setUseState(p->getId(), true);
            count++;
        }else{
            this->setUseState(p->getId(), false);
        }
    }
    if(count >= 1){
        return true;
    }else{
        return false;
    }
}
