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
     * isDefined -> becomes true as soon as the first function of a feature has been executed, which defines the feature
     * isSolved -> is true as long as there isn't any function which cannot be successfully executed
     */
    bool isDefined = false;
    foreach(Function *f, this->functionList){
        if(!isDefined){
            this->isSolved = f->exec(*this);
            isDefined = true;
        }else if(this->isSolved){
            this->isSolved = f->exec(*this);
        }
    }
    //if no function is set this feature cannot be solved and its coordinates are reset
    if(this->functionList.size() == 0 && this->isNominal == false){
        this->isSolved = false;
        this->xyz = OiVec(4);
    }
}

bool PointCloud::toOpenIndyXML(QXmlStreamWriter &stream){

    return false;
}

ElementDependencies PointCloud::fromOpenIndyXML(QXmlStreamReader &xml){
    ElementDependencies dependencies;

    return dependencies;
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
