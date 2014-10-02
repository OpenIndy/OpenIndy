#include "ps_shapesegment.h"

PS_ShapeSegment::PS_ShapeSegment()
{
}

PS_ShapeSegment::~PS_ShapeSegment()
{
}

/*!
 * \brief ShapeSegment::writeToObj
 * Write the points of the shape to an *.obj-file
 * \param filePath
 * \return
 */
bool PS_ShapeSegment::writeToObj(const QString &filePath){

    QFile file( filePath );
    if ( file.open(QIODevice::ReadWrite | QIODevice::Truncate) ){

        QTextStream streama( &file );
        for(int i = 0; i < this->getPoints().size(); i++){
            streama << "v " << QString::number(this->getPoints().at(i)->xyz[0])
                    << " " << QString::number(this->getPoints().at(i)->xyz[1])
                    << " " << QString::number(this->getPoints().at(i)->xyz[2]) << endl;
        }

        qDebug() << "writing finished";
        return true;

    }

    return false;

}

/*!
 * \brief ShapeSegment::writeToPts
 * Write the points of the shape to an *.pts-file
 * \param filePath
 * \return
 */
bool PS_ShapeSegment::writeToPts(const QString &filePath){

    int r = 0;
    int g = 0;
    int b = 0;
    std::random_device rd;
    int seed = rd();

    std::default_random_engine generator( seed );
    uniform_int_distribution<int> distribution(0, 255);
    r = distribution(generator);
    g = distribution(generator);
    b = distribution(generator);

    QFile file( filePath );
    if ( file.open(QIODevice::ReadWrite | QIODevice::Truncate) ){

        QTextStream streama( &file );
        for(int i = 0; i < this->getPoints().size(); i++){
            streama << QString::number(this->getPoints().at(i)->xyz[0])
                    << " " << QString::number(this->getPoints().at(i)->xyz[1])
                    << " " << QString::number(this->getPoints().at(i)->xyz[2])
                    << " " << r << " " << g << " " << b << endl;
        }
        return true;

    }

    return false;

}

/*!
 * \brief ShapeSegment::addPoint
 * Adds a point to the shape
 * \param p
 */
void PS_ShapeSegment::addPoint(PS_Point_PC *p){
    this->myState->myPoints.append(p);
}

/*!
 * \brief ShapeSegment::removePoint
 * Removes the point at the specified index from the shape
 * \param index
 */
void PS_ShapeSegment::removePoint(const int &index){
    this->myState->myPoints.removeAt(index);
}

/*!
 * \brief PS_ShapeSegment::removeUsedPoints
 * Remove all points which are already used by another shape
 */
void PS_ShapeSegment::removeUsedPoints(){
    QList<PS_Point_PC *> unusedPoints;
    foreach(PS_Point_PC *p, this->myState->myPoints){
        if(!p->isUsed){
            unusedPoints.append(p);
        }
    }
    this->myState->myPoints = unusedPoints;
}

/*!
 * \brief ShapeSegment::removeAllPoints
 * Removes all points from the shape
 */
void PS_ShapeSegment::removeAllPoints(){
    this->myState->myPoints.clear();
}

/*!
 * \brief ShapeSegment::saveCurrentState
 * Saves the current shape-state to be able to fall back to that state later
 */
void PS_ShapeSegment::saveCurrentState(){
    if(this->myOldState != NULL){
        *this->myOldState = *this->myState;
    }
}

/*!
 * \brief ShapeSegment::fallBack
 * Fall back to the previously saved state
 */
void PS_ShapeSegment::fallBack(){
    if(this->myOldState != NULL){
        *this->myState = *this->myOldState;
    }
}

/*!
 * \brief ShapeSegment::setIsValid
 * Sets the state of the shape to isValid
 * \param isValid
 */
void PS_ShapeSegment::setIsValid(const bool &isValid){
    this->myState->isValid = isValid;
}

/*!
 * \brief PS_ShapeSegment::addUsedNode
 * \param n
 */
void PS_ShapeSegment::addUsedNode(PS_Node *n){
    this->usedNodes.append(n);
}
