#ifndef PS_SHAPESEGMENT_H
#define PS_SHAPESEGMENT_H

#include <QList>
#include <QtMath>

#include "oivec.h"
#include "oimat.h"

#include "pointcloud.h"

#include "ps_pointcloud.h"
#include "ps_generalmath.h"

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
    QList<Point_PC *> myPoints; //points that define the shape

};

class PS_ShapeSegment
{

public:
    PS_ShapeSegment();
    virtual ~PS_ShapeSegment();

    virtual void fit() = 0;
    virtual void fitBySample(unsigned int numPoints) = 0;

    virtual void minimumSolution(QList<Point_PC *> points) = 0;

    virtual bool writeToObj(QString filePath);
    virtual bool writeToPts(QString filePath);

    QList<Point_PC *> getPoints();
    void addPoint(Point_PC *p);
    void removePoint(int index);
    void removeAllPoints();

    void saveCurrentState();
    void fallBack();

    bool getIsValid();
    void setIsValid(bool isValid);

    float getSigma();

    float *getMainFocus();

protected:
    ShapeState *myState; //current state of the plane
    ShapeState *myOldState; //old parameters of the plane to be able to reset the current solution to the last one

};

#endif // PS_SHAPESEGMENT_H
