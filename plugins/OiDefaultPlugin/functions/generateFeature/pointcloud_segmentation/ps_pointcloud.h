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
#include <QDateTime>

#include <string>
#include <sstream>

#include <ctime>

#include "ps_octree.h"
#include "ps_point_pc.h"

class PS_PlaneSegment;
class PS_SphereSegment;
class PS_CylinderSegment;

struct PS_BoundingBox_PC{
    float min[3];
    float max[3];
};

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
    float fitSampleSize; //percentage of points of a shape used to fit it by sample
    bool finalFit; //true if all detected shapes shall be fit at the end using all points
    PlaneParameter planeParams; //special plane parameter
    SphereParameter sphereParams; //special sphere parameter
    CylinderParameter cylinderParams; //special cylinder parameter
};

using namespace std;

class PS_PointCloud : public QObject
{

    Q_OBJECT
public:
    PS_PointCloud(QObject *parent = NULL);
    PS_PointCloud (const PS_PointCloud &copy);
    PS_PointCloud &operator=(const PS_PointCloud &copy);

    bool loadPointCloud(QString fileName);
    bool setCloud(vector<PS_Point_PC *> *myPoints, PS_BoundingBox_PC bbox, unsigned long numPoints);
    bool setUpOctree(PS_InputParameter param);
    bool detectShapes(PS_InputParameter param);

    const QList<PS_PlaneSegment *> &getDetectedPlanes();
    const QList<PS_SphereSegment *> &getDetectedSpheres();
    const QList<PS_CylinderSegment *> &getDetectedCylinders();

signals:
    void parseLine(QString line);
    void updateStatus(QString msg, int status);

private:
    clock_t c1;
    vector<PS_Point_PC *> *myPoints;

    PS_BoundingBox_PC myBoundingBox;
    unsigned long num_points;

    PS_Octree *myOctree;

    QList<PS_PlaneSegment *> detectedPlanes;
    QList<PS_SphereSegment *> detectedSpheres;
    QList<PS_CylinderSegment *> detectedCylinders;

    void considerNeighbourNodes(PS_Node *n, const PS_InputParameter &param, PS_PlaneSegment *p, QList<PS_Point_PC *> &unmergedPoints);
    void considerNeighbourNodes(PS_Node *n, const PS_InputParameter &param, PS_SphereSegment *s, QList<PS_Point_PC *> &unmergedPoints);
    void considerNeighbourNodes(PS_Node *n, const PS_InputParameter &param, PS_CylinderSegment *c, QList<PS_Point_PC *> &unmergedPoints);

    bool mergeNode(PS_Node *n, const PS_InputParameter &param, PS_PlaneSegment *p, QList<PS_Point_PC *> &unmergedPoints);
    bool mergeNode(PS_Node *n, const PS_InputParameter &param, PS_SphereSegment *s, QList<PS_Point_PC *> &unmergedPoints);
    bool mergeNode(PS_Node *n, const PS_InputParameter &param, PS_CylinderSegment *c, QList<PS_Point_PC *> &unmergedPoints);

    void acceptShapeCandidates(PS_PlaneSegment *p, PS_SphereSegment *s, PS_CylinderSegment *c, unsigned long &numUsedPoints, const PS_InputParameter &param);

    void printOutput(QString filePath, double processingTime, PS_InputParameter param);

    static QList<PS_Node *> mergedNodes; //save all merged nodes temporarily to be able to set them as unmerged in each iteration

    QString filePath;

};

#endif // PS_POINTCLOUD_H
