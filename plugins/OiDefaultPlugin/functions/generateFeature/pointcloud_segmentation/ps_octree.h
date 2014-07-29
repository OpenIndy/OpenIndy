#ifndef PS_OCTREE_H
#define PS_OCTREE_H

#include <QList>
#include <vector>

#include <ctime>

#include "pointcloud.h"

#include "ps_node.h"

struct NodeSorter
{
    inline bool operator() (const PS_Node* node1, const PS_Node* node2)
    {
        return (node1->points.size() > node2->points.size());
    }
};

using namespace std;

class PS_Octree
{
public:
    PS_Octree();

    bool setUp(vector<Point_PC*> *points, BoundingBox_PC *boundingBox, unsigned int minPoints);
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
    BoundingBox_PC *myBoundingBox;
    double dx, dy, dz;
    vector<Point_PC*> *myPoints;

    void computeNode(PS_Node *node);
    void computeOuterNeighbours(PS_Node *node);
};

#endif // PS_OCTREE_H
