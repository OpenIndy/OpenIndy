#include "ps_octree.h"

#include "ps_pointcloud.h"

PS_Octree::PS_Octree()
{
    this->isValid = false;
    this->root = NULL;
}

/*!
 * \brief Octree::setUp
 * Build the Octree structure based on the input points
 * \param points
 * \param boundingBox
 * \param minPoints
 * \return
 */
bool PS_Octree::setUp(vector<Point_PC *> *points, BoundingBox_PC *boundingBox, unsigned int minPoints){
    if(points != NULL && points->size() > 0){

        clock_t c1 = clock();

        unsigned int numPoints = points->size();

        qDebug() << "number of points: " << numPoints;

        this->minPoints = minPoints;
        this->myBoundingBox = boundingBox;
        this->myPoints = points;

        //create root node and set its points
        this->root = new PS_Node();
        for(unsigned int i = 0; i < numPoints; i++){
            this->root->points.push_back(points->at(i));
        }
        this->root->position[0] = this->myBoundingBox->min[0] + (this->myBoundingBox->max[0]
                - this->myBoundingBox->min[0]) * 0.5;
        this->root->position[1] = this->myBoundingBox->min[1] + (this->myBoundingBox->max[1]
                - this->myBoundingBox->min[1]) * 0.5;
        this->root->position[2] = this->myBoundingBox->min[2] + (this->myBoundingBox->max[2]
                - this->myBoundingBox->min[2]) * 0.5;
        this->dx = this->myBoundingBox->max[0] - this->myBoundingBox->min[0];
        this->dy = this->myBoundingBox->max[1] - this->myBoundingBox->min[1];
        this->dz = this->myBoundingBox->max[2] - this->myBoundingBox->min[2];

        //build Octree and set inner neighbours for each node
        this->computeNode(this->root);

        cout << "octree fertig " << (clock() - c1)/(double)CLOCKS_PER_SEC << " seconds." << endl;
        c1 = clock();

        qDebug() << "number of leafs: " << this->leafs.size();

        //traverse Octree and set the outer neighbours for each node
        this->computeOuterNeighbours(this->root);

        cout << "outer neighbours fertig " << (clock() - c1)/(double)CLOCKS_PER_SEC << " seconds." << endl;

        this->isValid = true;

        return true;
    }
    return false;
}

/*!
 * \brief Octree::getIsValid
 * Returns true if Octree was set up successfully
 * \return
 */
bool PS_Octree::getIsValid(){
    return this->isValid;
}

/*!
 * \brief Octree::getLeafs
 * \return
 */
QList<PS_Node *> *PS_Octree::getLeafs(){
    sort(this->leafs.begin(), this->leafs.end(), NodeSorter());
    return &this->leafs;
}

/*!
 * \brief Octree::getRoot
 * \return
 */
PS_Node *PS_Octree::getRoot(){
    return this->root;
}

/*!
 * \brief Octree::setNodesUnmerged
 */
void PS_Octree::setNodesUnmerged(PS_Node *n){
    /*if(n != NULL){
        n->consideredInMerge = false;
        if(!n->isLeaf){
            for(int i = 0; i < 8; i++){
                this->setNodesUnmerged(n->children[i]);
            }
        }
    }*/
}

/*!
 * \brief Octree::clear
 * Clear the Octree
 * \return
 */
bool PS_Octree::clear(){

    return false;
}

/*!
 * \brief Octree::computeNode
 * Compute a node and its children recurively
 * \param node
 */
void PS_Octree::computeNode(PS_Node *node){
    //if this node has to be subdivided into 8 child-nodes
    if(node->points.size() > this->minPoints){

        //set up sub-nodes
        for(int i = 0; i < 8; i++){

            //create sub-node
            node->children[i] = new PS_Node();

            //set sub-node's parent and octree-depth
            node->children[i]->parent = node;
            node->children[i]->depth = node->depth + 1;

        }

        //set inner neighbour nodes for each sub-node
        node->children[0]->top = node->children[4];
        node->children[0]->back = node->children[1];
        node->children[0]->right = node->children[3];
        node->children[1]->top = node->children[5];
        node->children[1]->front = node->children[0];
        node->children[1]->right = node->children[2];
        node->children[2]->top = node->children[6];
        node->children[2]->front = node->children[3];
        node->children[2]->left = node->children[1];
        node->children[3]->top = node->children[7];
        node->children[3]->back = node->children[2];
        node->children[3]->left = node->children[0];
        node->children[4]->bottom = node->children[0];
        node->children[4]->back = node->children[5];
        node->children[4]->right = node->children[7];
        node->children[5]->bottom = node->children[1];
        node->children[5]->front = node->children[4];
        node->children[5]->right = node->children[6];
        node->children[6]->bottom = node->children[2];
        node->children[6]->front = node->children[7];
        node->children[6]->left = node->children[5];
        node->children[7]->bottom = node->children[3];
        node->children[7]->back = node->children[6];
        node->children[7]->left = node->children[4];

        //categorize node's points and add them to the corresponding sub-node
        float mx, my, mz;
        mx = node->position[0];
        my = node->position[1];
        mz = node->position[2];
        foreach(Point_PC *myPoint, node->points){
            if(myPoint != NULL){
                if(myPoint->xyz[0] <= mx && myPoint->xyz[1] <= my && myPoint->xyz[2] <= mz){
                    node->children[0]->points.push_back(myPoint);
                }else if(myPoint->xyz[0] <= mx && myPoint->xyz[1] > my && myPoint->xyz[2] <= mz){
                    node->children[1]->points.push_back(myPoint);
                }else if(myPoint->xyz[0] > mx && myPoint->xyz[1] > my && myPoint->xyz[2] <= mz){
                    node->children[2]->points.push_back(myPoint);
                }else if(myPoint->xyz[0] > mx && myPoint->xyz[1] <= my && myPoint->xyz[2] <= mz){
                    node->children[3]->points.push_back(myPoint);
                }else if(myPoint->xyz[0] <= mx && myPoint->xyz[1] <= my && myPoint->xyz[2] > mz){
                    node->children[4]->points.push_back(myPoint);
                }else if(myPoint->xyz[0] <= mx && myPoint->xyz[1] > my && myPoint->xyz[2] > mz){
                    node->children[5]->points.push_back(myPoint);
                }else if(myPoint->xyz[0] > mx && myPoint->xyz[1] > my && myPoint->xyz[2] > mz){
                    node->children[6]->points.push_back(myPoint);
                }else if(myPoint->xyz[0] > mx && myPoint->xyz[1] <= my && myPoint->xyz[2] > mz){
                    node->children[7]->points.push_back(myPoint);
                }
            }
        }

        //set center for each sub-node
        int idx = 2;
        for(int i = 0; i < node->depth+1; i++){
            idx = idx * 2;
        }
        node->children[0]->position[0] = mx - (this->dx / idx);
        node->children[0]->position[1] = my - (this->dy / idx);
        node->children[0]->position[2] = mz - (this->dz / idx);
        node->children[1]->position[0] = mx - (this->dx / idx);
        node->children[1]->position[1] = my + (this->dy / idx);
        node->children[1]->position[2] = mz - (this->dz / idx);
        node->children[2]->position[0] = mx + (this->dx / idx);
        node->children[2]->position[1] = my + (this->dy / idx);
        node->children[2]->position[2] = mz - (this->dz / idx);
        node->children[3]->position[0] = mx + (this->dx / idx);
        node->children[3]->position[1] = my - (this->dy / idx);
        node->children[3]->position[2] = mz - (this->dz / idx);
        node->children[4]->position[0] = mx - (this->dx / idx);
        node->children[4]->position[1] = my - (this->dy / idx);
        node->children[4]->position[2] = mz + (this->dz / idx);
        node->children[5]->position[0] = mx - (this->dx / idx);
        node->children[5]->position[1] = my + (this->dy / idx);
        node->children[5]->position[2] = mz + (this->dz / idx);
        node->children[6]->position[0] = mx + (this->dx / idx);
        node->children[6]->position[1] = my + (this->dy / idx);
        node->children[6]->position[2] = mz + (this->dz / idx);
        node->children[7]->position[0] = mx + (this->dx / idx);
        node->children[7]->position[1] = my - (this->dy / idx);
        node->children[7]->position[2] = mz + (this->dz / idx);

        //call computeNode for each sub-node
        for(int i = 0; i < 8; i++){
            this->computeNode(node->children[i]);
        }

        return;
    }
    node->isLeaf = true;
    this->leafs.push_back(node);
    return;
}

/*!
 * \brief Octree::computeOuterNeighbours
 * \param node
 */
void PS_Octree::computeOuterNeighbours(PS_Node *node){
    if(!node->isLeaf){

        if(node->bottom != NULL){
            if(node->bottom->isLeaf){
                node->children[0]->bottom = node->bottom;
                node->children[1]->bottom = node->bottom;
                node->children[2]->bottom = node->bottom;
                node->children[3]->bottom = node->bottom;
            }else{
                node->children[0]->bottom = node->bottom->children[4];
                node->children[1]->bottom = node->bottom->children[5];
                node->children[2]->bottom = node->bottom->children[6];
                node->children[3]->bottom = node->bottom->children[7];
            }
        }

        if(node->top != NULL){
            if(node->top->isLeaf){
                node->children[4]->top = node->top;
                node->children[5]->top = node->top;
                node->children[6]->top = node->top;
                node->children[7]->top = node->top;
            }else{
                node->children[4]->top = node->top->children[0];
                node->children[5]->top = node->top->children[1];
                node->children[6]->top = node->top->children[2];
                node->children[7]->top = node->top->children[3];
            }
        }

        if(node->left != NULL){
            if(node->left->isLeaf){
                node->children[0]->left = node->left;
                node->children[1]->left = node->left;
                node->children[4]->left = node->left;
                node->children[5]->left = node->left;
            }else{
                node->children[0]->left = node->left->children[3];
                node->children[1]->left = node->left->children[2];
                node->children[4]->left = node->left->children[7];
                node->children[5]->left = node->left->children[6];
            }
        }

        if(node->right != NULL){
            if(node->right->isLeaf){
                node->children[2]->right = node->right;
                node->children[3]->right = node->right;
                node->children[6]->right = node->right;
                node->children[7]->right = node->right;
            }else{
                node->children[2]->right = node->right->children[1];
                node->children[3]->right = node->right->children[0];
                node->children[6]->right = node->right->children[5];
                node->children[7]->right = node->right->children[4];
            }
        }

        if(node->front != NULL){
            if(node->front->isLeaf){
                node->children[0]->front = node->front;
                node->children[3]->front = node->front;
                node->children[4]->front = node->front;
                node->children[7]->front = node->front;
            }else{
                node->children[0]->front = node->front->children[1];
                node->children[3]->front = node->front->children[2];
                node->children[4]->front = node->front->children[5];
                node->children[7]->front = node->front->children[6];
            }
        }

        if(node->back != NULL){
            if(node->back->isLeaf){
                node->children[1]->back = node->back;
                node->children[2]->back = node->back;
                node->children[5]->back = node->back;
                node->children[6]->back = node->back;
            }else{
                node->children[1]->back = node->back->children[0];
                node->children[2]->back = node->back->children[3];
                node->children[5]->back = node->back->children[4];
                node->children[6]->back = node->back->children[7];
            }
        }

        //compute outer neighbours for each sub-node's sub-nodes
        for(int i = 0; i < 8; i++){
            this->computeOuterNeighbours(node->children[i]);
        }

    }
    return;
}
