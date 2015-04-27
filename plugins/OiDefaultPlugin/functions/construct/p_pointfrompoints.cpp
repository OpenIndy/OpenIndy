#include "p_pointfrompoints.h"

/*!
 * \brief PointFromPoints::init
 */
void PointFromPoints::init(){

    //set plugin meta data
    this->metaData.name = "PointFromPoints";
    this->metaData.pluginName = "OpenIndy Default Plugin";
    this->metaData.author = "bra";
    this->metaData.description = QString("%1 %2")
            .arg("This function calculates an adjusted point.")
            .arg("You can input as many points as you want which are then used to find the best fit 3D point.");
    this->metaData.iid = "de.openIndy.plugin.function.constructFunction.v001";

    //set needed elements
    NeededElement param1;
    param1.description = "Select at least one points to calculate the best fit point.";
    param1.infinite = true;
    param1.typeOfElement = ePointElement;
    this->neededElements.append(param1);

    //set spplicable for
    this->applicableFor.append(ePointFeature);

}

/*!
 * \brief PointFromPoints::exec
 * \param point
 * \return
 */
bool PointFromPoints::exec(Point &point){
    return this->setUpResult(point);
}

/*!
 * \brief PointFromPoints::setUpResult
 * \param point
 */
bool PointFromPoints::setUpResult(Point &point){

    //get and check input points
    if(!this->inputElements.contains(0) || this->inputElements[0].size() < 1){
        emit this->sendMessage(QString("Not enough valid points to fit the point %1").arg(point.getFeatureName()));
        return false;
    }
    QList<QPointer<Point> > inputPoints;
    foreach(const InputElement &element, this->inputElements[0]){
        if(!element.point.isNull() && element.point->getIsSolved()){
            inputPoints.append(element.point);
            this->setUseState(0, element.id, true);
        }
        this->setUseState(0, element.id, false);
    }
    if(inputPoints.size() < 1){
        emit this->sendMessage(QString("Not enough valid points to fit the point %1").arg(point.getFeatureName()));
        return false;
    }

    //fill l vector
    OiVec l;
    foreach(const QPointer<Point> &p, inputPoints){
        l.add( p->getPosition().getVector().getAt(0) );
        l.add( p->getPosition().getVector().getAt(1) );
        l.add( p->getPosition().getVector().getAt(2) );
    }

    //fill A matrix
    OiMat a(l.getSize(), 3);
    for(int i = 0; i < l.getSize(); i++){
        if( (i%3) == 0 ){
            a.setAt(i, Point::unknownCenterX, 1.0);
        }else if( (i%3) == 1 ){
            a.setAt(i, Point::unknownCenterY, 1.0);
        }else if( (i%3) == 2 ){
            a.setAt(i, Point::unknownCenterZ, 1.0);
        }
    }

    //adjust
    OiMat n = a.t() * a;
    OiVec c = a.t() * l;
    OiMat qxx;
    OiVec x, v;
    try{
        qxx = n.inv();
        x = qxx * c;
        v = a * x - l;
    }catch(const logic_error &e){
        emit this->sendMessage(e.what());
        return false;
    }catch(const runtime_error &e){
        emit this->sendMessage(e.what());
        return false;
    }

    //calculate point based corrections
    OiVec corr;
    for(int i = 0; i < inputPoints.size(); i++){
        corr.add(qSqrt(v.getAt(3*i)*v.getAt(3*i)
                       + v.getAt(3*i+1)*v.getAt(3*i+1)
                       + v.getAt(3*i+2)*v.getAt(3*i+2)));
    }

    //calculate standard deviation
    double stdev = 0.0;
    OiVec::dot(stdev, corr, corr);
    stdev = qSqrt(stdev * (inputPoints.size() - 1.0));

    //set result
    Position position;
    position.setVector(x);
    point.setPoint(position);

    //set statistic
    this->statistic.setIsValid(true);
    this->statistic.setQxx(qxx);
    this->statistic.setV(corr);
    this->statistic.setStdev(stdev);
    point.setStatistic(this->statistic);

    return true;

}
