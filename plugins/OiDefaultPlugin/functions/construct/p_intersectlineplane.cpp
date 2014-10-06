#include "p_intersectlineplane.h"

/*!
 * \brief IntersectLinePlane::getMetaData
 * \return
 */
PluginMetaData* IntersectLinePlane::getMetaData(){
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "IntersectLinePlane";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "br";
    metaData->description = QString("%1")
            .arg("This function calculates the intersection of a line with a plane.");
    metaData->iid = "de.openIndy.Plugin.Function.ConstructFunction.v001";
    return metaData;
}

/*!
 * \brief IntersectLinePlane::getNeededElements
 * \return
 */
QList<InputParams> IntersectLinePlane::getNeededElements(){
    QList<InputParams> result;
    InputParams param;
    param.index = 0;
    param.description = "Select a line to calculate the intersection.";
    param.infinite = false;
    param.typeOfElement = Configuration::eLineElement;
    result.append(param);
    InputParams param2;
    param2.index = 1;
    param2.description = "Select a plane to calculate the intersection.";
    param2.infinite = false;
    param2.typeOfElement = Configuration::ePlaneElement;
    result.append(param2);
    return result;
}

/*!
 * \brief IntersectLinePlane::applicableFor
 * \return
 */
QList<Configuration::FeatureTypes> IntersectLinePlane::applicableFor(){
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::ePointFeature);
    return result;
}

/*!
 * \brief IntersectLinePlane::exec
 * \param p
 * \return
 */
bool IntersectLinePlane::exec(Point &p){
    if(this->isValid() && this->checkElementCount()){
        return this->setUpResult( p );
    }else{
        this->writeToConsole("Not enough elements available for calculation");
        return false;
    }
}

/*!
 * \brief IntersectLinePlane::setUpResult
 * Set up result and statistic for type point
 * \param point
 */
bool IntersectLinePlane::setUpResult(Point &point){
    bool check = false;
    //get line and plane to intersect
    Line *line = NULL;
    Plane *plane = NULL;
    foreach(Line *l, this->lines){
        if(l->getIsSolved()){
            line = l;
            break;
        }
    }
    foreach(Plane *p, this->planes){
        if(p->getIsSolved()){
            plane = p;
            break;
        }
    }
    //if line and plane are available
    if(line != NULL && plane != NULL){
        //calculate intersection if possible
        bool isIntersection = false;
        isIntersection = this->isIntersection(line, plane);
        if(isIntersection){
            //perform intersection
            OiVec l_v(3); //direction vector for line
            OiVec p_n(3); //normal vector for plane
            OiVec l_x0(3); //position vector for line
            OiVec p_x0(3); //position vector for plane
            for(int i = 0; i < 3; i++){
                l_v.setAt(i, line->ijk.getAt(i));
                p_n.setAt(i, plane->ijk.getAt(i));
                l_x0.setAt(i, line->xyz.getAt(i));
                p_x0.setAt(i, plane->xyz.getAt(i));
            }
            OiVec p_n0 = p_n.normalize(); //normalized normal vector for plane
            double d = 0.0; //distance plane to origin
            OiVec::dot(d, p_x0, p_n0);
            double h1, h2;
            OiVec::dot(h1, l_x0, p_n0);
            OiVec::dot(h2, l_v, p_n0);
            double lamda = (d - h1) / (h2);
            OiVec s = l_x0 + lamda * l_v;
            //set result
            s.add(1.0);
            point.xyz = s;
            check = true;
        }
    }
    return check;
}

/*!
 * \brief IntersectLinePlane::checkElementCount
 * Check wether there are enough valid elements for calculation
 * \return
 */
bool IntersectLinePlane::checkElementCount(){
    int count = 0;
    foreach(Line *l, this->lines){
        if(l->getIsSolved() && count == 0){
            this->setUseState(l->getId(), true);
            count++;
        }else{
            this->setUseState(l->getId(), false);
        }
    }
    foreach(Plane *p, this->planes){
        if(p->getIsSolved() && count == 1){
            count++;
        }else{
            this->setUseState(p->getId(), false);
        }
    }
    if(count == 2){
        return true;
    }else{
        return false;
    }
}

/*!
 * \brief IntersectLinePlane::isIntersection
 * Check wether the line and the plane intersect
 * \return
 */
bool IntersectLinePlane::isIntersection(Line *l, Plane *p){
    //scalar product of the lines's direction vector and the plane's normal vector
    double sp = l->ijk.getAt(0) * p->ijk.getAt(0) + l->ijk.getAt(1) * p->ijk.getAt(1)
            + l->ijk.getAt(2) * p->ijk.getAt(2);
    if(sp > 0.0001 || sp < -0.0001){
        return true;
    }
    return false;
}
