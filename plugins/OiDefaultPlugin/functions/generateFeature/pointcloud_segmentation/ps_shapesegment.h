#ifndef PS_SHAPESEGMENT_H
#define PS_SHAPESEGMENT_H

#include <QList>
#include <QtMath>
#include <QtXml>

#include "oivec.h"
#include "oimat.h"

#include "ps_pointcloud.h"
#include "ps_generalmath.h"
#include "ps_point_pc.h"
#include "ps_node.h"

struct ShapeState{

    ShapeState(){
        this->isValid = false;
        this->sigma = 0.0;
        this->mainFocus[0] = 0.0;
        this->mainFocus[1] = 0.0;
        this->mainFocus[2] = 0.0;
    }

    virtual ShapeState& operator=(const ShapeState &copy){
        this->isValid = copy.isValid;
        this->myPoints = copy.myPoints;
        this->sigma = copy.sigma;
        this->mainFocus[0] = copy.mainFocus[0];
        this->mainFocus[1] = copy.mainFocus[1];
        this->mainFocus[2] = copy.mainFocus[2];
        return *this;
    }

    bool isValid;
    float sigma; //variance factor
    float mainFocus[3]; //main focus of the points on the shape surface
    QList<PS_Point_PC *> myPoints; //points that define the shape

};

class PS_ShapeSegment
{

public:
    PS_ShapeSegment();
    virtual ~PS_ShapeSegment();

    virtual void fit() = 0;
    virtual void fitBySample(int numPoints) = 0;

    virtual void minimumSolution(const QList<PS_Point_PC *> &points) = 0;

    virtual bool writeToObj(const QString &filePath);
    virtual bool writeToPts(const QString &filePath);
    virtual bool writeToX3D(const QString &filePath) = 0;

    //! \brief Returns all points of the shape
    inline const QList<PS_Point_PC *> &getPoints(){
        return this->myState->myPoints;
    }

    //! \brief Returns the number of shape points
    inline const unsigned int getPointCount(){
        return this->myState->myPoints.size();
    }

    void addPoint(PS_Point_PC *p);
    void removePoint(const int &index);
    void removeUsedPoints();
    void removeAllPoints();

    void saveCurrentState();
    void fallBack();

    //! \brief Returns the current state of the shape
    inline bool getIsValid(){
        return this->myState->isValid;
    }
    void setIsValid(const bool &isValid);

    //! \brief Returns the sigma value of the shape
    inline float getSigma() const{
        return this->myState->sigma;
    }

    //! \brief Returns the main focus of the shape points
    inline float *getMainFocus() const{
        return this->myState->mainFocus;
    }

    //! \brief Returns all nodes used in merging step
    inline const QList<PS_Node *> &getUsedNodes() const{
        return this->usedNodes;
    }
    void addUsedNode(PS_Node *n);

protected:
    ShapeState *myState; //current state of the plane
    ShapeState *myOldState; //old parameters of the plane to be able to reset the current solution to the last one

    QList<PS_Node *> usedNodes; //nodes from which points were used for this shape

};

#endif // PS_SHAPESEGMENT_H
