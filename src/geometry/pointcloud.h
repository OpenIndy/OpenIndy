#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include "geometry.h"
#include "oiemitter.h"
#include "oivec.h"

class FeatureWrapper;

struct Point_PC{
    Point_PC(){ this->isUsed = false; }

    float xyz[3];
    bool isUsed;
};

struct BoundingBox_PC{
    float min[3];
    float max[3];
};

class PointCloud : public Geometry
{
    Q_OBJECT
public:
    PointCloud(bool isNominal, QObject *parent = 0);
    PointCloud(const PointCloud &copy);
    ~PointCloud();

    const QList<Point_PC *> &getPointCloudPoints() const;
    void addPointCloudPoint(Point_PC *myPoint);

    const OiVec &getMainFocus() const;
    void setMainFocus(OiVec mainFocus);

    const BoundingBox_PC &getBoundingBox() const;
    void setBoundingBox(BoundingBox_PC bbox);

    unsigned long getPointCount() const;

    bool addSegment(FeatureWrapper *segment);
    void clearSegmentation();

    void recalc();

    bool toOpenIndyXML(QXmlStreamWriter& stream);
    ElementDependencies fromOpenIndyXML(QXmlStreamReader& xml);

    bool saveSimulationData();

    QString getDisplayX() const;
    QString getDisplayY() const;
    QString getDisplayZ() const;

signals:
    void pcSegmentAdded(FeatureWrapper *segment);

private:
    QList<Point_PC*> myPoints; //all points of the pointcloud
    OiVec xyz; //centroid of the pointcloud
    BoundingBox_PC bbox; //bounding box of the pointcloud

    QList<FeatureWrapper*> detectedSegments; //geometry-segments that were detected in the pointcloud
};

#endif // POINTCLOUD_H
