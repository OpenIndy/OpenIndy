#include "p_pointbetweentwopoints.h"

/*!
 * \brief PointBetweenTwoPoints::init
 */
void PointBetweenTwoPoints::init(){

    //set plugin meta data
    this->metaData.name = "PointBetweenTwoPoints";
    this->metaData.pluginName = "OpenIndy Default Plugin";
    this->metaData.author = "bra";
    this->metaData.description = QString("%1 %2")
            .arg("This function calculates the point between two points.")
            .arg("Therefore you have to input those two points.");
    this->metaData.iid = "de.openIndy.plugin.function.constructFunction.v001";

    //set needed elements
    NeededElement param1;
    param1.description = "Select first point.";
    param1.infinite = false;
    param1.typeOfElement = ePointElement;
    NeededElement param2;
    param2.description = "Select second point.";
    param2.infinite = false;
    param2.typeOfElement = ePointElement;
    this->neededElements.append(param2);

    //set spplicable for
    this->applicableFor.append(ePointFeature);

}

/*!
 * \brief PointBetweenTwoPoints::exec
 * \param point
 * \return
 */
bool PointBetweenTwoPoints::exec(Point &point){
    return this->setUpResult(point);
}

/*!
 * \brief PointBetweenTwoPoints::setUpResult
 * \param point
 * \return
 */
bool PointBetweenTwoPoints::setUpResult(Point &point){

    //get and check input points
    if(!this->inputElements.contains(0) || this->inputElements[0].size() != 1
            || !this->inputElements.contains(1) || this->inputElements.size() != 1){
        return false;
    }
    QPointer<Point> point1 = this->inputElements[0].at(0).point;
    QPointer<Point> point2 = this->inputElements[1].at(0).point;
    if(point1.isNull() || point2.isNull() || !point1->getIsSolved() || !point2->getIsSolved()){
        return false;
    }

    //calculate midpoint
    OiVec center = (point1->getPosition().getVector() + point2->getPosition().getVector()) * 0.5;

    //set result
    Position position;
    position.setVector(center);
    point.setPoint(position);

    //set statistic
    this->statistic.setIsValid(true);
    this->statistic.setStdev((point1->getStatistic().getStdev() + point2->getStatistic().getStdev()) * 0.5);
    point.setStatistic(this->statistic);

    return true;

}
