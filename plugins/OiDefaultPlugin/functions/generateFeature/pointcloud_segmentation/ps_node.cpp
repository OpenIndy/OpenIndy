#include "ps_node.h"

PS_Node::PS_Node()
{
    this->depth = 0;
    for(int i = 0; i < 3; i++){
        this->position[i] = 0.0;
    }

    this->isLeaf = false;

    this->parent = NULL;

    this->back = NULL;
    this->front = NULL;
    this->bottom = NULL;
    this->top = NULL;
    this->left = NULL;
    this->right = NULL;

    this->consideredAsSeed = false;
    this->consideredInMerge = false;
}

/*!
 * \brief Node::getWasConsideredInMerge
 * Returns true if all points of this node were considered in current merging step otherwise false
 * \return
 */
/*bool PS_Node::getWasConsideredInMerge(){
    return this->consideredInMerge;
}*/

/*!
 * \brief Node::setWasConsideredInMerge
 */
void PS_Node::setWasConsideredInMerge(const bool &state){
    this->consideredInMerge = state;

    if(state == true){ //set parent as considered in merge if all its subnodes were considered in merge

        //if this node has children then set them as considered in merge if not yet done
        if(!this->isLeaf){
            for(int i = 0; i < 8; i++){
                if(!this->children[i]->getWasConsideredInMerge()){
                    this->children[i]->setWasConsideredInMerge(true);
                }
            }
        }

        //set parent as considered in merge if all children were considered in merge and the parent is not yet considered
        if(this->parent != NULL && !this->parent->getWasConsideredInMerge()){
            bool parentState = true;
            for(unsigned int i = 0; i < 8; i++){
                if(this->parent->children[i]->points.size() > 0 && !this->parent->children[i]->getWasConsideredInMerge()){
                    parentState = false;
                    break;
                }
            }
            if(parentState){
               this->parent->setWasConsideredInMerge(true);
            }
        }

    }else{ //set all children of this node to not have been considered in merge

        //set parent to not have been considered in merge
        if(this->parent != NULL && this->parent->getWasConsideredInMerge()){
            this->parent->setWasConsideredInMerge(false);
        }

        //set children to not have been considered in merge
        if(!this->isLeaf){
            for(unsigned int i = 0; i < 8; i++){
                if(this->children[i]->getWasConsideredInMerge()){
                    this->children[i]->setWasConsideredInMerge(false);
                }
            }
        }

    }
}

/*!
 * \brief Node::setWasConsideredAsSeed
 */
void PS_Node::setWasConsideredAsSeed(const bool &state){
    this->consideredAsSeed = state;
}

/*!
 * \brief PS_Node::getUnusedPoints
 * Returns a list of all points that are not used for another shape
 * \param result
 */
void PS_Node::getUnusedPoints(QList<PS_Point_PC *> &result) const{
    if(result.size() != 0){
        result.clear();
    }
    for(int i = 0; i < this->points.size(); i++){
        PS_Point_PC *p = this->points.at(i);
        if(!p->isUsed){
            result.append(p);
        }
    }
}

/*!
 * \brief PS_Node::getUnusedPointsCount
 * Returns the number of points that are not used for another shape
 * \return
 */
unsigned long PS_Node::getUnusedPointsCount() const{
    unsigned long result = 0;
    for(unsigned long i = 0; i < this->points.size(); i++){
        PS_Point_PC *p = this->points.at(i);
        if(!p->isUsed){
            result++;
        }
    }
    return result;
}

/*!
 * \brief PS_Node::getUnmergedPoints
 * Returns a list of all points that are not used for another shape and that have not been considered in merging step
 * \param result
 */
void PS_Node::getUnmergedPoints(QList<PS_Point_PC *> &result) const{
    if(result.size() != 0){
        result.clear();
    }
    this->getUnmergedChildPoints(result);
}

/*!
 * \brief PS_Node::getUnmergedChildPoints
 * Returns the unused points of the children of a node
 * \param unmergedPoints
 */
void PS_Node::getUnmergedChildPoints(QList<PS_Point_PC *> &unmergedPoints) const{

    //if already considered in merge no points are added
    if(this->consideredInMerge){
        return;
    }

    //if this is a leaf node add the unused points
    if(this->isLeaf){
        foreach(PS_Point_PC *p, this->points){
            if(!p->isUsed){
                unmergedPoints.append(p);
            }
        }
    }else{
        for(int i = 0; i < 8; i++){
            this->children[i]->getUnmergedChildPoints(unmergedPoints);
        }
    }

}

/*!
 * \brief Node::getUnmergedPoints
 * Returns a vector of unmerged, unused points
 * \return
 */
/*QList<Point_PC *> Node::getUnmergedPoints(){
    QList<Point_PC *> result;
    for(int i = 0; i < this->points.size(); i++){
        Point_PC *p = this->points.at(i);
        if(!p->isMerged && !p->isUsed){
            result.append(p);
        }
    }
    return result;
}*/

/*!
 * \brief Node::getUnmergedPointsCount
 * \return
 */
/*int Node::getUnmergedPointsCount(){
    int result = 0;
    for(int i = 0; i < this->points.size(); i++){
        Point_PC *p = this->points.at(i);
        if(!p->isMerged && !p->isUsed){
            result++;
        }
    }
    return result;
}*/
