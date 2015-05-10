#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include <QObject>
#include <QtXml>

#include "geometry.h"
#include "position.h"

class FeatureWrapper;

/*!
 * \brief The Point_PC class
 */
class Point_PC{
    Point_PC(){ this->isUsed = false; }

    float xyz[3];
    bool isUsed;
};

/*!
 * \brief The BoundingBox_PC class
 */
class BoundingBox_PC{
    float min[3];
    float max[3];
};

/*!
 * \brief The PointCloud class
 */
class PointCloud : public Geometry
{
    Q_OBJECT

public:
    PointCloud(bool isNominal, QObject *parent = 0);

    PointCloud(const PointCloud &copy, QObject *parent = 0);

    PointCloud &operator=(const PointCloud &copy);

    ~PointCloud();

    //###########################
    //general point cloud methods
    //###########################

    unsigned long getPointCount() const;

    bool addSegment(const QPointer<FeatureWrapper> &segment);
    void clearSegmentation();

    //###################################################################
    //check wether this geometry has a direction, a position and a radius
    //###################################################################

    bool hasPosition() const;

    //#################################
    //get or set point cloud parameters
    //#################################

    const QList<QPointer<Point_PC> > &getPointCloudPoints() const;
    void addPointCloudPoint(const QPointer<Point_PC> &point);

    const Position &getPosition() const;
    void setPosition(const Position &mainFocus);

    const BoundingBox_PC &getBoundingBox() const;
    void setBoundingBox(const BoundingBox_PC &bbox);

    //###########################
    //reexecute the function list
    //###########################

    void recalc();

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

    //###############
    //display methods
    //###############

    QString getDisplayX(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    QString getDisplayY(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    QString getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff = false) const;

signals:
    void pcSegmentAdded(const QPointer<FeatureWrapper> &segment);

private:

    //######################
    //point cloud attributes
    //######################

    QList<QPointer<Point_PC> > points; //all points of the pointcloud
    Position mainFocus; //centroid of the pointcloud
    BoundingBox_PC bbox; //bounding box of the pointcloud

    QList<QPointer<FeatureWrapper> > detectedSegments; //geometry-segments that were detected in the pointcloud

};

#endif // POINTCLOUD_H
