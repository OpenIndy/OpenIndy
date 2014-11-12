#include "pointcloud.h"

#include "function.h"
#include "featurewrapper.h"

PointCloud::PointCloud(bool isNominal, QObject *parent) : Geometry(isNominal, parent), xyz(4)
{
    this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

/*!
 * \brief PointCloud::PointCloud
 * \param copy
 */
PointCloud::PointCloud(const PointCloud &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->xyz = copy.xyz;
    this->isSolved = copy.isSolved;
}

PointCloud::~PointCloud()
{
}

/*!
 * \brief PointCloud::getPointCloudPoints
 * \return
 */
const QList<Point_PC *> &PointCloud::getPointCloudPoints() const{
    return this->myPoints;
}

/*!
 * \brief PointCloud::addPointCloudPoint
 * \param myPoint
 */
void PointCloud::addPointCloudPoint(Point_PC *myPoint){
    this->myPoints.append(myPoint);
}

/*!
 * \brief PointCloud::getMainFocus
 * \return
 */
const OiVec &PointCloud::getMainFocus() const{
    return this->xyz;
}

/*!
 * \brief PointCloud::setMainFocus
 * \param mainFocus
 */
void PointCloud::setMainFocus(OiVec mainFocus){
    this->xyz = mainFocus;
}

/*!
 * \brief PointCloud::getBoundingBox
 * \return
 */
const BoundingBox_PC &PointCloud::getBoundingBox() const{
    return this->bbox;
}

/*!
 * \brief PointCloud::setBoundingBox
 * \param bbox
 */
void PointCloud::setBoundingBox(BoundingBox_PC bbox){
    this->bbox = bbox;
}

/*!
 * \brief PointCloud::getPointCount
 * \return
 */
unsigned long PointCloud::getPointCount() const{
    return this->myPoints.size();
}

/*!
 * \brief PointCloud::addSegment
 * Adds a new segment to the pointcloud and sends it to OiFeatureState
 * \param segment
 * \return
 */
bool PointCloud::addSegment(FeatureWrapper *segment){

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

/*!
 * \brief PointCloud::clearSegmentation
 */
void PointCloud::clearSegmentation(){
   this->detectedSegments.clear();
}

//TODO pointcloud bei function adden

/*!
 * \brief PointCloud::recalc
 * Execute alls functions in the specified order
 */
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

    pointCloud.setAttribute("type", Configuration::sPointCloud);
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

bool PointCloud::saveSimulationData()
{
    return false;
}

//TODO change functions with unitconverter and String format

QString PointCloud::getDisplayX() const{
    return QString::number(this->xyz.getAt(0)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString PointCloud::getDisplayY() const{
    return QString::number(this->xyz.getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString PointCloud::getDisplayZ() const{
    return QString::number(this->xyz.getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}
