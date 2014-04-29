#include "p_bestfitpoint.h"

/*!
 * \brief BestFitPoint::getMetaData
 * \return
 */
PluginMetaData* BestFitPoint::getMetaData(){
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "BestFitPoint";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "br";
    metaData->description = QString("%1 %2")
            .arg("This function calculates an adjusted point.")
            .arg("You can input as many observations as you want which are then used to find the best fit 3D point.");
    metaData->iid = "de.openIndy.Plugin.Function.FitFunction.v001";
    return metaData;
}

/*!
 * \brief BestFitPoint::getNeededElements
 * \return
 */
QList<InputParams> BestFitPoint::getNeededElements(){
    QList<InputParams> result;
    InputParams param;
    param.index = 0;
    param.description = "Select observations to calculate the best fit point.";
    param.infinite = true;
    param.typeOfElement = Configuration::eObservationElement;
    result.append(param);
    return result;
}

/*!
 * \brief BestFitPoint::applicableFor
 * \return
 */
QList<Configuration::FeatureTypes> BestFitPoint::applicableFor(){
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::ePointFeature);
    return result;
}

/*!
 * \brief BestFitPoint::exec
 * \param p
 * \return
 */
bool BestFitPoint::exec(Point &p){
    if(this->isValid() && this->checkObservationCount()){
        this->setUpPointResult( p );
        this->writeToConsole("point fit completed");
        return true;
    }else{
        this->writeToConsole("Not enough observations available for calculation");
        return false;
    }

}

/*!
 * \brief BestFitPoint::setUpResult
 * Set up result and statistic for type point
 * \param midpoint
 */
void BestFitPoint::setUpPointResult(Point &point){
    //Fill l vector
    OiVec l;
    vector<double> sigma;
    foreach(Observation *obs, this->observations){
        if(obs->isValid){
            l.add( obs->myXyz.getAt(0) );
            l.add( obs->myXyz.getAt(1) );
            l.add( obs->myXyz.getAt(2) );
            //sigma.push_back( obs->sigmaXyz.getAt(0)*obs->sigmaXyz.getAt(0) );
            //sigma.push_back( obs->sigmaXyz.getAt(1)*obs->sigmaXyz.getAt(1) );
            //sigma.push_back( obs->sigmaXyz.getAt(2)*obs->sigmaXyz.getAt(2) );
            this->setUseState(obs->id, true);
        }else{
            this->setUseState(obs->id, false);
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
        //Fill P matrix
        //OiMat sll(l.getSize(), l.getSize());
        //OiMat p(l.getSize(), l.getSize());
        //sll.diag(sigma);
        //OiMat neu = 3.3 * a;
        //p = sll.inv();
        //Adjust
        OiMat n = a.t() * a;
        OiVec c = a.t() * l;
        try{
            OiMat qxx = n.inv();
            OiVec x = qxx * c;
            OiVec v = a * x - l;
            x.add(1.0);

            double stdv = 0.0;
            if(v.getSize() > 3){
                double sum_vv = 0.0;
                for(int i = 0; i < v.getSize(); i++){
                    sum_vv += v.getAt(i) * v.getAt(i);
                }
                stdv = qSqrt(sum_vv / (v.getSize() - 3));
            }else{
                stdv = 0.0;
            }
            point.myStatistic.s0_apriori = 1.0;
            point.myStatistic.s0_aposteriori = stdv;

            //set point
            point.xyz = x;
            point.myStatistic.qxx = qxx;
            point.myStatistic.v.replace(v);
            point.myStatistic.stdev = stdv;

            for(int i = 0; i < point.myStatistic.v.getSize() / 3; i++){
                Residual r;

                r.addValue("vx", v.getAt(i*3), UnitConverter::eMetric);
                r.addValue("vy", v.getAt(1+i*3), UnitConverter::eMetric);
                r.addValue("vz", v.getAt(2+i*3), UnitConverter::eMetric);

                point.myStatistic.displayResiduals.append(r);
            }

            point.myStatistic.isValid = true;
            this->myStatistic = point.myStatistic;

        }catch(logic_error e){
            this->writeToConsole(e.what());
            return;
        }catch(runtime_error e){
            this->writeToConsole(e.what());
            return;
        }
    }else{
        this->writeToConsole("Not enough valid observations available for calculation");
    }

}

/*!
 * \brief BestFitPoint::checkObservationCount
 * Check wether there are enough valid observations for calculation
 * \return
 */
bool BestFitPoint::checkObservationCount(){
    int count = 0;
    foreach(Observation *obs, this->observations){
        if(obs->isValid){
            count++;
        }else{
            this->setUseState(obs->id, false);
        }
    }
    if(count >= 1){
        return true;
    }else{
        return false;
    }
}

QStringList BestFitPoint::getResultProtocol(){
    QStringList dummy;
    dummy.append("test protocol");
    return dummy;
}
