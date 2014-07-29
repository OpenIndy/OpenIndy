#ifndef PS_NODE_H
#define PS_NODE_H

#include <QList>

#include "pointcloud.h"

//#include "ps_point_pc.h"

class PS_Node
{
public:
    PS_Node();

    bool getWasConsideredInMerge();
    void setWasConsideredInMerge(bool state);

    bool getWasConsideredAsSeed();
    void setWasConsideredAsSeed(bool state);

    QList<Point_PC *> getUnusedPoints();
    int getUnusedPointsCount();

    int depth; //depth of the level at which this node lies
    float position[3]; //position of node's center

    bool isLeaf; //true if this node is a leaf node

    PS_Node *parent; //this node's parent
    PS_Node *children[8]; //children of this node (only if isLeaf = false)

    //neighbours of this node at level equal or greater than this node's level
    PS_Node *top;
    PS_Node *bottom;
    PS_Node *left;
    PS_Node *right;
    PS_Node *front;
    PS_Node *back;

    QList<Point_PC*> points; //list with pointers to points in the pointcloud

private:
    bool consideredAsSeed; //is set to true as soon as this node has been considered as a seed region
    bool consideredInMerge; //is set to true as soon as this node or all its subnodes were considered in merging step
};

#endif // PS_NODE_H
