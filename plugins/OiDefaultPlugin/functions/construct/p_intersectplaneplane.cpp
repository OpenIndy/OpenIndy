#include "p_intersectplaneplane.h"

/*!
 * \brief IntersectPlanePlane::getMetaData
 * \return
 */
PluginMetaData* IntersectPlanePlane::getMetaData(){
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "IntersectPlanePlane";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "br";
    metaData->description = QString("%1")
            .arg("This function calculates the line of intersection of a plane with another plane.");
    metaData->iid = "de.openIndy.Plugin.Function.ConstructFunction.v001";
    return metaData;
}

/*!
 * \brief IntersectPlanePlane::getNeededElements
 * \return
 */
QList<InputParams> IntersectPlanePlane::getNeededElements(){
    QList<InputParams> result;
    InputParams param1;
    param1.index = 0;
    param1.description = "Select a plane to calculate the line of intersection.";
    param1.infinite = false;
    param1.typeOfElement = Configuration::ePlaneElement;
    result.append(param1);
    InputParams param2;
    param2.index = 0;
    param2.description = "Select another plane to calculate the line intersection.";
    param2.infinite = false;
    param2.typeOfElement = Configuration::ePlaneElement;
    result.append(param2);
    return result;
}

/*!
 * \brief IntersectPlanePlane::applicableFor
 * \return
 */
QList<Configuration::FeatureTypes> IntersectPlanePlane::applicableFor(){
    QList<Configuration::FeatureTypes> result;
    result.append(Configuration::eLineFeature);
    return result;
}

/*!
 * \brief IntersectPlanePlane::exec
 * \param l
 * \return
 */
bool IntersectPlanePlane::exec(Line &l){
    if(this->isValid() && this->checkElementCount()){
        return this->setUpResult( l );
    }else{
        this->writeToConsole("Not enough elements available for calculation");
        return false;
    }
}

/*!
 * \brief IntersectPlanePlane::setUpResult
 * Set up result and statistic for type line
 * \param line
 */
bool IntersectPlanePlane::setUpResult(Line &line){
    bool check = false;
    //get planes to intersect
    Plane *plane1 = NULL;
    Plane *plane2 = NULL;
    int count = 0;
    foreach(Plane *p, this->planes){
        if(p->getIsSolved() && count == 0){
            plane1 = p;
            count++;
        }else if(p->getIsSolved() && count == 1){
            plane2 = p;
            break;
        }
    }
    //if planes are available
    if(plane1 != NULL && plane2 != NULL){
        //calculate intersection if possible
        if(this->isIntersection(plane1, plane2)){
            //get vectors which define the 2 planes
            OiVec p1_n(3); //normal vector for plane 1
            OiVec p2_n(3); //normal vector for plane 2
            OiVec p1_x0(3); //position vector for plane 1
            OiVec p2_x0(3); //position vector for plane 2
            for(int i = 0; i < 3; i++){
                p1_n.setAt(i, plane1->ijk.getAt(i));
                p2_n.setAt(i, plane2->ijk.getAt(i));
                p1_x0.setAt(i, plane1->xyz.getAt(i));
                p2_x0.setAt(i, plane2->xyz.getAt(i));
            }
            OiVec p1_n0 = p1_n.normalize(); //normalized normal vector for plane 1
            OiVec p2_n0 = p2_n.normalize(); //normalized normal vector for plane 2
            OiVec d(3); //distance vector with distances origin to plane 1, 2, 3 (plane 3 is normal to plane 1 and 2)
            double h1, h2;
            OiVec::dot(h1, p1_x0, p1_n0);
            OiVec::dot(h2, p2_x0, p2_n0);
            d.setAt(0, h1);
            d.setAt(1, h2);
            d.setAt(2, 0.0);
            //perform intersection
            OiVec l_v = this->calcDirectionVector(p1_n0, p2_n0);
            OiVec l_x0 = this->calcPointOnLine(p1_n0, p2_n0, l_v, d);
            l_v.add(1.0);
            l_x0.add(1.0);
            //set result
            line.xyz = l_x0;
            line.ijk = l_v;
            check = true;
        }
    }
    return check;
}

/*!
 * \brief IntersectPlanePlane::checkElementCount
 * Check wether there are enough valid elements for calculation
 * \return
 */
bool IntersectPlanePlane::checkElementCount(){
    int count = 0;
    foreach(Plane *p, this->planes){
        if(p->getIsSolved() && count < 2){
            this->setUseState(p->getId(), true);
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
 * \brief IntersectPlanePlane::isIntersection
 * Check wether the line and the plane intersect
 * \param p1
 * \param p2
 * \return
 */
bool IntersectPlanePlane::isIntersection(Plane *p1, Plane *p2){
    OiVec n1(3);
    OiVec n2(3);
    for(int i = 0; i < 3; i++){
        n1.setAt(i, p1->ijk.getAt(i));
        n2.setAt(i, p2->ijk.getAt(i));
    }
    OiVec n01 = n1.normalize();
    OiVec n02 = n2.normalize();
    double sp;
    OiVec::dot(sp, n01, n02);
    if(sp > -0.99990 && sp < 0.99990){
        return true;
    }
    return false;
}

/*!
 * \brief IntersectPlanePlane::calcDirectionVector
 * Calculate vector which is normal to n1 and n2.
 * The result is the direction vector of the line of intersection.
 * \param n1
 * \param n2
 * \return
 */
OiVec IntersectPlanePlane::calcDirectionVector(OiVec n1, OiVec n2){
    OiVec result;
    OiVec::cross(result, n1, n2);
    return result;
}

/*!
 * \brief IntersectPlanePlane::calcPointOnLine
 * Calculate a point on the line of intersection by solving a system of equations.
 * The system of equations is given by the normal vectors of three planes and their distances d.
 * \param n1
 * \param n2
 * \param n3
 * \param d
 * \return
 */
OiVec IntersectPlanePlane::calcPointOnLine(OiVec n1, OiVec n2, OiVec n3, OiVec d){
    OiMat a(3, 3);
    for(int i = 0; i < 3; i++){
        a.setAt(0, i, n1.getAt(i));
        a.setAt(1, i, n2.getAt(i));
        a.setAt(2, i, n3.getAt(i));
    }
    return a.inv() * d;
}
