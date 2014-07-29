#ifndef PS_POINTCLOUD_H
#define PS_POINTCLOUD_H

#include <QList>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <fstream>
#include <iostream>
#include <limits>
#include <QThread>
#include <QObject>
#include <vector>

#include <string>
#include <sstream>

#include <ctime>

#include "pointcloud.h"

#include "ps_octree.h"

class PS_PlaneSegment;
class PS_SphereSegment;
class PS_CylinderSegment;

//! special input parameter only for plane detection
struct PlaneParameter{
    bool detectPlanes; //true if planes shall be detected, false if not
    unsigned int minPoints; //minimum number of points that a plane has to contain to be detected
    float maxDistance; //maximum orthogonal distance of a point from an estimated plane so that the point is still associated with the plane
};

//! special input parameter only for sphere detection
struct SphereParameter{
    bool detectSpheres; //true if spheres shall be detected, false if not
    unsigned int minPoints; //minimum number of points that a sphere has to contain to be detected
    float maxDistance; //maximum orthogonal distance of a point from an estimated sphere so that the point is still associated with the sphere
    float minRadius; //minimum radius of detected spheres
    float maxRadius; //maximum radius of detected spheres
};

//! special input parameter only for cylinder detection
struct CylinderParameter{
    bool detectCylinders; //true if cylinders shall be detected, false if not
    unsigned int minPoints; //minimum number of points that a cylinder has to contain to be detected
    float maxDistance; //maximum orthogonal distance of a point from an estimated cylinder so that the point is still associated with the cylinder
    float minRadius; //minimum radius of detected spheres
    float maxRadius; //maximum radius of detected spheres
};

//! input parameter for pointcloud segmentation
struct PS_InputParameter{
    unsigned int leafSize; //maximum number of points in one leaf-voxel
    float outlierPercentage; //estimated percentage of outlier points in a leaf voxel (between 0.0 and 1.0)
    unsigned int fitSampleSize; //number of points used to fit a shape (fitBySample)
    bool forceProximity; //true if only voxel of nearly the same size shall be merged, false if the size does not matter
    PlaneParameter planeParams; //special plane parameter
    SphereParameter sphereParams; //special sphere parameter
    CylinderParameter cylinderParams; //special cylinder parameter
};

using namespace std;

class PS_PointCloud // : public QObject
{

    //Q_OBJECT
public:
    PS_PointCloud();

    bool loadPointCloud(QString fileName);
    bool setUpOctree(PS_InputParameter param);
    bool detectShapes(PS_InputParameter param);

/*signals:
    void parseLine(QString line);*/

private:
    clock_t c1;

    vector<Point_PC *> *myPoints;
    BoundingBox_PC myBoundingBox;
    unsigned long num_points;

    PS_Octree *myOctree;

    QList<PS_PlaneSegment *> detectedPlanes;
    QList<PS_SphereSegment *> detectedSpheres;
    QList<PS_CylinderSegment *> detectedCylinders;

    void considerNeighbourNodes(PS_Node *n, PS_InputParameter param, PS_PlaneSegment *p);
    void considerNeighbourNodes(PS_Node *n, PS_InputParameter param, PS_SphereSegment *s);
    void considerNeighbourNodes(PS_Node *n, PS_InputParameter param, PS_CylinderSegment *c);

    void mergeNode(PS_Node *n, PS_InputParameter param, PS_PlaneSegment *p);
    void mergeNode(PS_Node *n, PS_InputParameter param, PS_SphereSegment *s);
    void mergeNode(PS_Node *n, PS_InputParameter param, PS_CylinderSegment *c);

    static QList<PS_Node *> mergedNodes; //save all merged nodes temporarily to be able to set them as unmerged in each iteration

};

#endif // PS_POINTCLOUD_H
