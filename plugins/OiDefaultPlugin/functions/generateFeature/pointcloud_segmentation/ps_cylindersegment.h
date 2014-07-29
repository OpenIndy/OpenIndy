#ifndef PS_CYLINDERSEGMENT_H
#define PS_CYLINDERSEGMENT_H

#include <QList>
#include <QtMath>
#include <string>
#include <limits>

#include "oivec.h"
#include "oimat.h"

#include "pointcloud.h"

#include "ps_pointcloud.h"
#include "ps_generalmath.h"
#include "ps_shapesegment.h"

//! \brief cylinder specific attributes
struct CylinderState : ShapeState{

    CylinderState() : ShapeState(){
        this->radius = 0.0;
        this->alpha = 0.0;
        this->beta = 0.0;
        this->xyz[0] = 0.0;
        this->xyz[1] = 0.0;
    }

    ShapeState& operator=(const ShapeState &copy){

        //call base class assignment operator
        ShapeState::operator =(copy);

        //try to get pointer to derived struct (CylinderState)
        const CylinderState *myCylinderState = dynamic_cast<const CylinderState*>(&copy);

        if(myCylinderState){ //if cast is valid copy the attributes
            this->radius = myCylinderState->radius;
            this->alpha = myCylinderState->alpha;
            this->beta = myCylinderState->beta;
            this->xyz[0] = myCylinderState->xyz[0];
            this->xyz[1] = myCylinderState->xyz[1];
        }else{ //else set default values
            this->radius = 0.0;
            this->alpha = 0.0;
            this->beta = 0.0;
            this->xyz[0] = 0.0;
            this->xyz[1] = 0.0;
        }

        return *this;

    }

    float radius; //radius of the cylinder
    float alpha; //rotation around x axis
    float beta; //rotation around y axis
    float xyz[2]; //point on the cylinder axis after rotation (z = 0)

};

//! \brief used to find spheres in unstructured pointclouds
class PS_CylinderSegment : public PS_ShapeSegment
{

public:
    PS_CylinderSegment();
    ~PS_CylinderSegment();

    void fit();
    void fitBySample(unsigned int numPoints);

    void minimumSolution(QList<Point_PC *> points);

    float getRadius();
    float *getXYZ();
    float getAlpha();
    float getBeta();

    void setApproximation(float alpha, float beta, float radius, float x, float y); //set values by hand

    static PS_CylinderSegment *detectCylinder(QList<Point_PC*> points, PS_InputParameter param);
    static int checkPointsInCylinder(PS_CylinderSegment *myCylinder, QList<Point_PC*> myPoints, PS_InputParameter param, int toleranceFactor); //check wether a point is in the given cylinder
    static void sortOut(PS_CylinderSegment *myCylinder, PS_InputParameter param);
    static void mergeCylinders(const QList<PS_CylinderSegment *> &detectedCylinders, QList<PS_CylinderSegment *> &mergedCylinders, PS_InputParameter param);

private:

    double getCorrespondingCos(double a);
    double getCorrespondingSin(double a);
    bool compareAngles(double a, double b);

    //static helpers (to not have to instanciate every time)
    static OiMat Ralpha;
    static OiMat Rbeta;
    static OiMat Rall;
    static OiVec x_m_n;
    static OiVec n0;
    static OiVec a;
    static OiVec b;
    static OiVec X0; //approximation of unknowns (r, X0, Y0, alpha, beta)
    static OiVec x; //corrections of unknowns
    static OiMat H; //used in minimum solution
    static OiMat u;
    static OiVec d;
    static OiMat v;

    //current cylinder state pointer to access special cylinder attributes
    CylinderState *myCylinderState;

};

#endif // PS_CYLINDERSEGMENT_H
