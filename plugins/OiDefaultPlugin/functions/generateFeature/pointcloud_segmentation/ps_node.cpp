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
bool PS_Node::getWasConsideredInMerge(){
    /*if(this->consideredInMerge){
        return this->consideredInMerge;
    }else{
        if(this->isLeaf){

        }
    }*/
    return this->consideredInMerge;
}

/*!
 * \brief Node::setWasConsideredInMerge
 */
void PS_Node::setWasConsideredInMerge(bool state){
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


        //TODO reset parent and children to not have been considered in merge so that by saving a list of merged
        //node in the mergin step and afterwards setting all the nodes considerdInMerge(false) all nodes are false
        //and the octree must not be traversed completely

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
 * \brief Node::getWasConsideredAsSeed
 *  Returns true if this node has been considered as seed
 * \return
 */
bool PS_Node::getWasConsideredAsSeed(){
    return this->consideredAsSeed;
}

/*!
 * \brief Node::setWasConsideredAsSeed
 */
void PS_Node::setWasConsideredAsSeed(bool state){
    this->consideredAsSeed = state;
}

QList<Point_PC *> PS_Node::getUnusedPoints(){
    QList<Point_PC *> result;
    for(int i = 0; i < this->points.size(); i++){
        Point_PC *p = this->points.at(i);
        if(!p->isUsed){
            result.append(p);
        }
    }
    return result;
}

int PS_Node::getUnusedPointsCount()
{
    int result = 0;
    for(int i = 0; i < this->points.size(); i++){
        Point_PC *p = this->points.at(i);
        if(!p->isUsed){
            result++;
        }
    }
    return result;
}
