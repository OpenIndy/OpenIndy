#include "pointcloud.h"

#include "featurewrapper.h"

/*
#include "function.h"
#include "featurewrapper.h"

MeasurementConfig PointCloud::defaultMeasurementConfig;

PointCloud::PointCloud(bool isNominal, QObject *parent) : Geometry(isNominal, parent), xyz(4)
{
    //this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

PointCloud::PointCloud(const PointCloud &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->xyz = copy.xyz;
    this->isSolved = copy.isSolved;
}

PointCloud::~PointCloud()
{
}

const QList<QPointer<Point_PC> > &PointCloud::getPointCloudPoints() const{
    return this->myPoints;
}

void PointCloud::addPointCloudPoint(const QPointer<Point_PC> &myPoint){
    this->myPoints.append(myPoint);
}

const OiVec &PointCloud::getMainFocus() const{
    return this->xyz;
}

void PointCloud::setMainFocus(OiVec mainFocus){
    this->xyz = mainFocus;
}

const BoundingBox_PC &PointCloud::getBoundingBox() const{
    return this->bbox;
}

void PointCloud::setBoundingBox(BoundingBox_PC bbox){
    this->bbox = bbox;
}

unsigned long PointCloud::getPointCount() const{
    return this->myPoints.size();
}

bool PointCloud::addSegment(const QPointer<FeatureWrapper> &segment){

    if(segment == NULL || segment->getGeometry() == NULL){
        return false;
    }

    foreach(FeatureWrapper *detectedSegment, this->detectedSegments){
        if(detectedSegment != NULL && detectedSegment->getFeature() != NULL
                && detectedSegment->getFeature()->getFeatureName().compare(segment->getFeature()->getFeatureName()) == 0){
            return false;
        }
    }

    segment->getGeometry()->setNominalSystem(this->myNominalCoordSys);

    this->detectedSegments.append(segment);

    emit this->pcSegmentAdded(segment);

}

void PointCloud::clearSegmentation(){
   this->detectedSegments.clear();
}

void PointCloud::recalc(){

    if(this->functionList.size() > 0){

        bool solved = true;
        foreach(Function *f, this->functionList){

            //execute the function if it exists and if the last function was executed successfully
            if(f != NULL && solved == true){
                solved = f->exec(*this);
            }

        }
        this->setIsSolved(solved);

    }else if(this->isNominal == false){

        this->setIsSolved(false);

    }

}

QDomElement PointCloud::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement pointCloud = Geometry::toOpenIndyXML(xmlDoc);

    if(pointCloud.isNull()){
        return pointCloud;
    }

    pointCloud.setAttribute("type", getGeometryTypeName(ePointCloudGeometry));
}

bool PointCloud::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){



    }

    return result;

}

bool PointCloud::saveSimulationData()
{
    return false;
}

QString PointCloud::getDisplayX(bool showDiff) const{
    return "";
    //return QString::number(this->xyz.getAt(0)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
}

QString PointCloud::getDisplayY(bool showDiff) const{
    return "";
    //return QString::number(this->xyz.getAt(1)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
}

QString PointCloud::getDisplayZ(bool showDiff) const{
    return "";
    //return QString::number(this->xyz.getAt(2)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
}
*/

/*!
 * \brief PointCloud::PointCloud
 * \param isNominal
 * \param parent
 */
PointCloud::PointCloud(bool isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPointCloud(this);
    }

}

/*!
 * \brief PointCloud::PointCloud
 * \param copy
 */
PointCloud::PointCloud(const PointCloud &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPointCloud(this);
    }

    this->setPosition(copy.mainFocus);
    this->setBoundingBox(copy.bbox);

}

/*!
 * \brief PointCloud::operator =
 * \param copy
 * \return
 */
PointCloud &PointCloud::operator=(const PointCloud &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPointCloud(this);
    }

    this->setPosition(copy.mainFocus);
    this->setBoundingBox(copy.bbox);

    return *this;

}

/*!
 * \brief PointCloud::~PointCloud
 */
PointCloud::~PointCloud(){

}

/*!
 * \brief PointCloud::getPointCount
 * \return
 */
unsigned long PointCloud::getPointCount() const{
    return this->points.size();
}

/*!
 * \brief PointCloud::addSegment
 * \param segment
 * \return
 */
bool PointCloud::addSegment(const QPointer<FeatureWrapper> &segment){

    //check segment
    if(segment.isNull() || segment->getGeometry().isNull()){
        return false;
    }

    //check if a segment with that name already exists
    foreach(const QPointer<FeatureWrapper> &detectedSegment, this->detectedSegments){

        if(!detectedSegment.isNull() && !detectedSegment->getFeature().isNull()
                && detectedSegment->getFeature()->getFeatureName().compare(segment->getGeometry()->getFeatureName()) == 0){
            return false;
        }

    }

    segment->getGeometry()->setNominalSystem(this->nominalSystem);

    this->detectedSegments.append(segment);

    emit this->pcSegmentAdded(segment);

}

/*!
 * \brief PointCloud::clearSegmentation
 */
void PointCloud::clearSegmentation(){
    this->detectedSegments.clear();
}

/*!
 * \brief PointCloud::hasPosition
 * \return
 */
bool PointCloud::hasPosition() const{
    return true;
}

/*!
 * \brief PointCloud::getPointCloudPoints
 * Returns a list of all point cloud points
 * \return
 */
const QList<QPointer<Point_PC> > &PointCloud::getPointCloudPoints() const{
    return this->points;
}

/*!
 * \brief PointCloud::addPointCloudPoint
 * \param point
 */
void PointCloud::addPointCloudPoint(const QPointer<Point_PC> &point){
    this->points.append(point);
}

/*!
 * \brief PointCloud::getPosition
 * Returns the main focus of the point cloud points
 * \return
 */
const Position &PointCloud::getPosition() const{
    return this->mainFocus;
}

/*!
 * \brief PointCloud::setPosition
 * \param mainFocus
 */
void PointCloud::setPosition(const Position &mainFocus){
    this->mainFocus = mainFocus;
}

/*!
 * \brief PointCloud::getBoundingBox
 * Returns the bounding box of the point cloud
 * \return
 */
const BoundingBox_PC &PointCloud::getBoundingBox() const{
    return this->bbox;
}

/*!
 * \brief PointCloud::setBoundingBox
 * \param bbox
 */
void PointCloud::setBoundingBox(const BoundingBox_PC &bbox){
    this->bbox = bbox;
}

/*!
 * \brief PointCloud::recalc
 */
void PointCloud::recalc(){

    Geometry::recalc();

}

/*!
 * \brief PointCloud::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement PointCloud::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement pointCloud = Geometry::toOpenIndyXML(xmlDoc);

    if(pointCloud.isNull()){
        return pointCloud;
    }

    pointCloud.setAttribute("type", getGeometryTypeName(ePointCloudGeometry));

}

/*!
 * \brief PointCloud::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool PointCloud::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){



    }

    return result;

}

/*!
 * \brief PointCloud::getDisplayX
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString PointCloud::getDisplayX(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->mainFocus.getVector().getAt(0), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief PointCloud::getDisplayY
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString PointCloud::getDisplayY(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->mainFocus.getVector().getAt(1), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief PointCloud::getDisplayZ
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString PointCloud::getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertFromDefault(this->mainFocus.getVector().getAt(2), type), 'f', digits);
    }
    return QString("");
}
