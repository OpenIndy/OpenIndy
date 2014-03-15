#include "pointcloud.h"
#include "function.h"

PointCloud::PointCloud() : xyz(4)
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
PointCloud::PointCloud(const PointCloud &copy){
    this->id = copy.id;
    this->name = copy.name;
    this->xyz = copy.xyz;
    this->isSolved = copy.isSolved;
}

PointCloud::~PointCloud()
{
}

//TODO pointcloud bei function adden

/*!
 * \brief PointCloud::recalc
 * Execute alls functions in the specified order
 */
void PointCloud::recalc(){
   /*
    foreach(Function *f, this->functionList){
        f->exec(*this);
    }
    */
}

bool PointCloud::toOpenIndyXML(QXmlStreamWriter &stream){

    return false;
}

bool PointCloud::fromOpenIndyXML(QXmlStreamReader &xml){

    return false;
}

//TODO change functions with unitconverter and String format

QString PointCloud::getX(){
    return QString::number(this->xyz.getAt(0));
}

QString PointCloud::getY(){
    return QString::number(this->xyz.getAt(1));
}

QString PointCloud::getZ(){
    return QString::number(this->xyz.getAt(2));
}

QString PointCloud::getIsCommon(){
    return QString(isCommon?"true":"false");
}

QString PointCloud::getIsNominal(){
    return QString(isNominal?"true":"false");
}
