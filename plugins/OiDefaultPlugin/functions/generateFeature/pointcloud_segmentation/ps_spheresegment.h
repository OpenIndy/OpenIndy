#ifndef PS_SPHERESEGMENT_H
#define PS_SPHERESEGMENT_H

#include <QList>
#include <QtMath>

#include "oivec.h"
#include "oimat.h"

#include "pointcloud.h"

#include "ps_pointcloud.h"
#include "ps_generalmath.h"
#include "ps_shapesegment.h"

//! \brief sphere specific attributes
struct SphereState : ShapeState{

    SphereState() : ShapeState(){
        this->radius = 0.0;
        this->xyz[0] = 0.0;
        this->xyz[1] = 0.0;
        this->xyz[2] = 0.0;
    }

    ShapeState& operator=(const ShapeState &copy){

        //call base class assignment operator
        ShapeState::operator =(copy);

        //try to get pointer to derived struct (SphereState)
        const SphereState *mySphereState = dynamic_cast<const SphereState*>(&copy);

        if(mySphereState){ //if cast is valid copy the attributes
            this->radius = mySphereState->radius;
            this->xyz[0] = mySphereState->xyz[0];
            this->xyz[1] = mySphereState->xyz[1];
            this->xyz[2] = mySphereState->xyz[2];
        }else{ //else set default values
            this->radius = 0.0;
            this->xyz[0] = 0.0;
            this->xyz[1] = 0.0;
            this->xyz[2] = 0.0;
        }

        return *this;

    }

    float radius; //radius of the sphere
    float xyz[3]; //center of the sphere

};

//! \brief used to find spheres in unstructured pointclouds
class PS_SphereSegment : public PS_ShapeSegment
{

public:
    PS_SphereSegment();
    ~PS_SphereSegment();

    void fit();
    void fitBySample(unsigned int numPoints);

    void minimumSolution(QList<Point_PC *> points);

    float getRadius();
    float *getXYZ();

    void setApproximation(float radius, float x, float y, float z);

    static PS_SphereSegment *detectSphere(QList<Point_PC*> points, PS_InputParameter param);
    static int checkPointsInSphere(PS_SphereSegment *mySphere, QList<Point_PC*> myPoints, PS_InputParameter param, int toleranceFactor); //check wether a point is in the given sphere
    static void sortOut(PS_SphereSegment *mySphere, PS_InputParameter param);
    static void mergeSpheres(const QList<PS_SphereSegment *> &detectedSpheres, QList<PS_SphereSegment *> &mergedSpheres, PS_InputParameter param);
    static void verifySpheres(const QList<PS_SphereSegment *> &detectedSpheres, QList<PS_SphereSegment *> &verifiedSpheres, PS_InputParameter param);

private:
    //fitting helpers (static to not have to instanciate every time)

    //minimum solution of a sphere
    static OiMat min_A;
    static OiMat min_AReplaced;
    static OiVec min_C;
    static OiVec min_X;

    //fitting of a sphere
    static OiMat fitting_N;
    static OiVec fitting_n;
    static OiMat fitting_Q;
    static OiVec fitting_a;
    static OiVec fitting_x;

    //sphere verification
    static OiMat verify_u;
    static OiMat verify_v;
    static OiVec verify_d;
    static OiVec verify_xyz;

    //current sphere state pointer to access special sphere attributes
    SphereState *mySphereState;

};

#endif // PS_SPHERESEGMENT_H
