#ifndef PS_NODE_H
#define PS_NODE_H

#include <QList>

#include "ps_point_pc.h"

class PS_Node
{
public:
    PS_Node();

    //! \brief Returns true if this node has been considered in merging step
    inline const bool &getWasConsideredInMerge() const{
        return this->consideredInMerge;
    }
    void setWasConsideredInMerge(const bool &state);

    //! \brief Returns true if this node has been considered as seed
    inline const bool &getWasConsideredAsSeed() const{
        return this->consideredAsSeed;
    }
    void setWasConsideredAsSeed(const bool &state);

    void getUnusedPoints(QList<PS_Point_PC *> &result) const;
    unsigned long getUnusedPointsCount() const;

    void getUnmergedPoints(QList<PS_Point_PC *> &result) const;

    int depth; //depth of the level at which this node lies
    float position[3]; //position of node's center

    bool isLeaf; //true if this node is a leaf node

    PS_Node *parent; //this node's parent
    PS_Node *children[8]; //children of this node (only if isLeaf = false)

    //neighbours of this node at level equal or greater than this node's level
    PS_Node *top, *bottom, *left, *right, *front, *back;

    QList<PS_Point_PC*> points; //vector with references to points in the pointcloud

private:
    bool consideredAsSeed; //is set to true as soon as this node has been considered as a seed region
    bool consideredInMerge; //is set to true as soon as this node or all its subnodes were considered in merging step

    void getUnmergedChildPoints(QList<PS_Point_PC *> &unmergedPoints) const;
};

#endif // PS_NODE_H
