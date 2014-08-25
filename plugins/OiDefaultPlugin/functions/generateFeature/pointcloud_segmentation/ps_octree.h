#ifndef PS_OCTREE_H
#define PS_OCTREE_H

#include <QList>
#include <vector>

#include "ps_node.h"
#include "ps_point_pc.h"

struct PS_BoundingBox_PC;

struct NodeSorter
{
    inline bool operator() (const PS_Node* node1, const PS_Node* node2)
    {
        int v1 = node1->points.size(), v2 = node2->points.size();
        if(node1->depth - node2->depth > 0){
            for(int i = 0; i < node1->depth - node2->depth; i++){
                v1 = v1 * 8;
            }
        }else if(node1->depth - node2->depth < 0){
            for(int i = 0; i < node2->depth - node1->depth; i++){
                v2 = v2 * 8;
            }
        }
        return (v1 > v2);
        //return (node1->points.size() > node2->points.size());
    }
};

using namespace std;

class PS_Octree
{
public:
    PS_Octree();

    bool setUp(vector<PS_Point_PC*> *points, PS_BoundingBox_PC *boundingBox, unsigned int minPoints);
    bool clear();
    bool getIsValid();

    QList<PS_Node *> *getLeafs();
    PS_Node *getRoot();

    void setNodesUnmerged(PS_Node *n); //set all nodes to not have been considered during merging

private:
    PS_Node *root;
    QList<PS_Node*> leafs;

    bool isValid; //true if octree was set up successfully

    unsigned int minPoints;
    PS_BoundingBox_PC *myBoundingBox;
    double dx, dy, dz;
    vector<PS_Point_PC*> *myPoints;

    void computeNode(PS_Node *node);
    void computeOuterNeighbours(PS_Node *node);
};

#endif // PS_OCTREE_H
