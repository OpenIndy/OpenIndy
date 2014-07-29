#ifndef PS_PLANESEGMENT_H
#define PS_PLANESEGMENT_H

#include <QList>
#include <QtMath>

#include "oivec.h"
#include "oimat.h"

#include "pointcloud.h"

#include "ps_pointcloud.h"
#include "ps_generalmath.h"
#include "ps_shapesegment.h"

//! \brief plane specific attributes
struct PlaneState : ShapeState{

    PlaneState() : ShapeState(){
        this->d = 0.0;
        this->n0[0] = 0.0;
        this->n0[1] = 0.0;
        this->n0[2] = 0.0;
    }

    ShapeState& operator=(const ShapeState &copy){

        //call base class assignment operator
        ShapeState::operator =(copy);

        //try to get pointer to derived struct (PlaneState)
        const PlaneState *myPlaneState = dynamic_cast<const PlaneState*>(&copy);

        if(myPlaneState){ //if cast is valid copy the attributes
            this->d = myPlaneState->d;
            this->n0[0] = myPlaneState->n0[0];
            this->n0[1] = myPlaneState->n0[1];
            this->n0[2] = myPlaneState->n0[2];
        }else{ //else set default values
            this->d = 0.0;
            this->n0[0] = 0.0;
            this->n0[1] = 0.0;
            this->n0[2] = 0.0;
        }

        return *this;

    }

    float d; //smallest distance of the plane from the origin
    float n0[3]; //unit normal vector of the plane

};

//! \brief used to find planes in unstructured pointclouds
class PS_PlaneSegment : public PS_ShapeSegment
{
public:
    PS_PlaneSegment();
    ~PS_PlaneSegment();

    void fit();
    void fitBySample(unsigned int numPoints);

    void minimumSolution(QList<Point_PC *> points);

    float getDistance();
    float *getIJK();

    static PS_PlaneSegment *detectPlane(QList<Point_PC*> points, PS_InputParameter param);
    static int checkPointsInPlane(PS_PlaneSegment *myPlane, QList<Point_PC*> myPoints, PS_InputParameter param, int toleranceFactor); //check wether a point is in the given plane
    static void sortOut(PS_PlaneSegment *myPlane, PS_InputParameter param);
    static void mergePlanes(const QList<PS_PlaneSegment *> &detectedPlanes, QList<PS_PlaneSegment *> &mergedPlanes, PS_InputParameter param);

private:

    //fitting helpers (static to not have to instanciate every time)
    static OiMat u;
    static OiMat v;
    static OiVec d;
    static OiVec n0;

    //current plane state pointer to access special plane attributes
    PlaneState *myPlaneState;
};

#endif // PS_PLANESEGMENT_H
