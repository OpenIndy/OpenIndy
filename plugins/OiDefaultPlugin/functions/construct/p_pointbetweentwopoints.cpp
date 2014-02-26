#include "p_pointbetweentwopoints.h"

/*!
 * \brief PointBetweenTwoPoints::getMetaData
 * \return
 */
PluginMetaData* PointBetweenTwoPoints::getMetaData(){
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "PointBetweenTwoPoints";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "br";
    metaData->description = QString("%1 %2")
            .arg("This function calculates the point between two points.")
            .arg("Therefore you have to input those two points.");
    metaData->iid = "de.openIndy.Plugin.Function.ConstructFunction.v001";
    //...
    return metaData;
}

/*!
 * \brief PointBetweenTwoPoints::getNeededElements
 * \return
 */
QList<InputParams> PointBetweenTwoPoints::getNeededElements(){
    QList<InputParams> result;
    InputParams param1;
    param1.index = 0;
    param1.description = "Select first point.";
    param1.infinite = false;
    param1.typeOfElement = Configuration::ePointElement;
    result.append(param1);
    InputParams param2;
    param2.index = 0;
    param2.description = "Select second point.";
    param2.infinite = false;
    param2.typeOfElement = Configuration::ePointElement;
    result.append(param2);
    return result;
}

/*!
 * \brief PointBetweenTwoPoints::applicableFor
 * \return
 */
QList<Configuration::FeatureTypes> PointBetweenTwoPoints::applicableFor(){
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::ePointFeature);
    return result;
}

/*!
 * \brief PointBetweenTwoPoints::exec
 * \param p
 * \return
 */
bool PointBetweenTwoPoints::exec(Point &p){
    if(this->isValid() && this->getTwoPoints()){
        return this->calcMidPoint(p);
    }else{
        Console::addLine("Not enough points available for calculation");
        return false;
    }
}

/*!
 * \brief PointBetweenTwoPoints::getTwoPoints
 * Get start and destiantion point
 * \return
 */
bool PointBetweenTwoPoints::getTwoPoints(){
    this->startPoint = NULL;
    this->destPoint = NULL;
    if(this->featureOrder.contains(0) && this->featureOrder.contains(1)){ //if featureOrder contains 2 elements
        QMap<int, QList<InputFeature> >::iterator startIdx = this->featureOrder.find(0);
        QMap<int, QList<InputFeature> >::iterator destIdx = this->featureOrder.find(0);
        if(startIdx != this->featureOrder.end() && startIdx.value().size() > 0
                && destIdx != this->featureOrder.end() && destIdx.value().size() > 0){ //if both lists contain elements
            QList<InputFeature> startElementList = startIdx.value();
            QList<InputFeature> destElementList = destIdx.value();
            if(startElementList.at(0).typeOfElement == Configuration::ePointElement
                    && destElementList.at(0).typeOfElement == Configuration::ePointElement){ //if both elements are point elements
                this->startPoint = this->getPoint(startElementList.at(0).id);
                this->destPoint = this->getPoint(destElementList.at(0).id);
                if(this->startPoint != NULL && this->destPoint != NULL){ //if both points exist
                    return true;
                }
            }
        }
    }
    return false;
}

/*!
 * \brief PointBetweenTwoPoints::calcMidPoint
 * Calculate the point between start and destination point
 * \return
 */
bool PointBetweenTwoPoints::calcMidPoint(Point &p){
    if(this->startPoint->isSolved && this->startPoint->isUpdated
            && this->destPoint->isSolved && this->destPoint->isUpdated){
        OiVec result = (startPoint->xyz + destPoint->xyz) / 2.0;
        result.setAt(3, 1.0);
        p.xyz = result;
        return true;
    }
    return false;
}
