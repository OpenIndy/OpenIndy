#include "sortlistbyname.h"

SortListByName::SortListByName()
{
}

/*!
 * \brief SortListByName::addRefPoint
 * \param p
 */
void SortListByName::addRefPoint(Point p){
    if(p.name != ""){
        if(this->point.contains(p.name)){ //if there is yet a loc or ref point with this name available
            QMap<QString, LocRefBool>::Iterator idx = this->point.find(p.name);
            if(idx != this->point.end()){
                LocRefBool checkAvailable = idx.value();
                if(checkAvailable.ref == false){ //if there is no ref point with the same name
                    this->refPoints.append(p);
                    checkAvailable.ref = true;
                    checkAvailable.refIndex = this->refPoints.size()-1;
                    this->point.insert(p.name, checkAvailable);
                }
            }
        }else{ //if there is not a point with this name available
            this->refPoints.append(p);
            LocRefBool checkAvailable;
            checkAvailable.ref = true;
            checkAvailable.loc = false;
            checkAvailable.refIndex = this->refPoints.size()-1;
            checkAvailable.locIndex = -1;
            this->point.insert(p.name, checkAvailable);
        }
    }
}

/*!
 * \brief SortListByName::addLocPoint
 * \param p
 */
void SortListByName::addLocPoint(Point p){
    if(p.name != ""){
        if(this->point.contains(p.name)){ //if there is yet a loc or ref point with this name available
            QMap<QString, LocRefBool>::Iterator idx = this->point.find(p.name);
            if(idx != this->point.end()){
                LocRefBool checkAvailable = idx.value();
                if(checkAvailable.loc == false){ //if there is no loc point with the same name
                    this->locPoints.append(p);
                    checkAvailable.loc = true;
                    checkAvailable.locIndex = this->locPoints.size()-1;
                    this->point.insert(p.name, checkAvailable);
                }
            }
        }else{ //if there is not a point with this name available
            this->locPoints.append(p);
            LocRefBool checkAvailable;
            checkAvailable.loc = true;
            checkAvailable.ref = false;
            checkAvailable.locIndex = this->locPoints.size()-1;
            checkAvailable.refIndex = -1;
            this->point.insert(p.name, checkAvailable);
        }
    }
}

/*!
 * \brief SortListByName::getRefPoints
 * \return
 */
QList<Point> SortListByName::getRefPoints(){
    QList<Point> result;
    QList<QString> keys = this->point.keys();
    foreach(QString key, keys){ //all entries in map
        QMap<QString, LocRefBool>::Iterator idx = this->point.find(key);
        if(idx != this->point.end()){
            LocRefBool checkAvailable = idx.value();
            if(checkAvailable.loc == true && checkAvailable.ref == true
                    && this->locPoints.size() > checkAvailable.locIndex && checkAvailable.locIndex >= 0
                    && this->refPoints.size() > checkAvailable.refIndex && checkAvailable.refIndex >= 0){ //if loc and ref point are available
                //add ref point at refIndex
                result.append(this->refPoints.at(checkAvailable.refIndex));
            }
        }
    }
    return result;
}

/*!
 * \brief SortListByName::getLocPoints
 * \return
 */
QList<Point> SortListByName::getLocPoints(){
    QList<Point> result;
    QList<QString> keys = this->point.keys();
    foreach(QString key, keys){ //all entries in map
        QMap<QString, LocRefBool>::Iterator idx = this->point.find(key);
        if(idx != this->point.end()){
            LocRefBool checkAvailable = idx.value();
            if(checkAvailable.loc == true && checkAvailable.ref == true
                    && this->locPoints.size() > checkAvailable.locIndex && checkAvailable.locIndex >= 0
                    && this->refPoints.size() > checkAvailable.refIndex && checkAvailable.refIndex >= 0){ //if loc and ref point are available
                //add loc point at locIndex
                result.append(this->locPoints.at(checkAvailable.locIndex));
            }
        }
    }
    return result;
}

/*!
 * \brief SortListByName::addRefLine
 * \param l
 */
void SortListByName::addRefLine(Line l){
    if(l.name != ""){
        if(this->line.contains(l.name)){ //if there is yet a loc or ref line with this name available
            QMap<QString, LocRefBool>::Iterator idx = this->line.find(l.name);
            if(idx != this->line.end()){
                LocRefBool checkAvailable = idx.value();
                if(checkAvailable.ref == false){ //if there is no ref line with the same name
                    this->refLines.append(l);
                    checkAvailable.ref = true;
                    checkAvailable.refIndex = this->refLines.size()-1;
                    this->line.insert(l.name, checkAvailable);
                }
            }
        }else{ //if there is not a line with this name available
            this->refLines.append(l);
            LocRefBool checkAvailable;
            checkAvailable.ref = true;
            checkAvailable.loc = false;
            checkAvailable.refIndex = this->refLines.size()-1;
            checkAvailable.locIndex = -1;
            this->line.insert(l.name, checkAvailable);
        }
    }
}

/*!
 * \brief SortListByName::addLocLine
 * \param l
 */
void SortListByName::addLocLine(Line l){
    if(l.name != ""){
        if(this->line.contains(l.name)){ //if there is yet a loc or ref line with this name available
            QMap<QString, LocRefBool>::Iterator idx = this->line.find(l.name);
            if(idx != this->line.end()){
                LocRefBool checkAvailable = idx.value();
                if(checkAvailable.loc == false){ //if there is no loc line with the same name
                    this->locLines.append(l);
                    checkAvailable.loc = true;
                    checkAvailable.locIndex = this->locLines.size()-1;
                    this->line.insert(l.name, checkAvailable);
                }
            }
        }else{ //if there is not a line with this name available
            this->locLines.append(l);
            LocRefBool checkAvailable;
            checkAvailable.loc = true;
            checkAvailable.ref = false;
            checkAvailable.locIndex = this->locLines.size()-1;
            checkAvailable.refIndex = -1;
            this->line.insert(l.name, checkAvailable);
        }
    }
}

/*!
 * \brief SortListByName::getRefLines
 * \return
 */
QList<Line> SortListByName::getRefLines(){
    QList<Line> result;
    QList<QString> keys = this->line.keys();
    foreach(QString key, keys){ //all entries in map
        QMap<QString, LocRefBool>::Iterator idx = this->line.find(key);
        if(idx != this->line.end()){
            LocRefBool checkAvailable = idx.value();
            if(checkAvailable.loc == true && checkAvailable.ref == true
                    && this->locLines.size() > checkAvailable.locIndex && checkAvailable.locIndex >= 0
                    && this->refLines.size() > checkAvailable.refIndex && checkAvailable.refIndex >= 0){ //if loc and ref line are available
                //add ref line at refIndex
                result.append(this->refLines.at(checkAvailable.refIndex));
            }
        }
    }
    return result;
}

/*!
 * \brief SortListByName::getLocLines
 * \return
 */
QList<Line> SortListByName::getLocLines(){
    QList<Line> result;
    QList<QString> keys = this->line.keys();
    foreach(QString key, keys){ //all entries in map
        QMap<QString, LocRefBool>::Iterator idx = this->line.find(key);
        if(idx != this->line.end()){
            LocRefBool checkAvailable = idx.value();
            if(checkAvailable.loc == true && checkAvailable.ref == true
                    && this->locLines.size() > checkAvailable.locIndex && checkAvailable.locIndex >= 0
                    && this->refLines.size() > checkAvailable.refIndex && checkAvailable.refIndex >= 0){ //if loc and ref line are available
                //add loc line at locIndex
                result.append(this->locLines.at(checkAvailable.locIndex));
            }
        }
    }
    return result;
}

/*!
 * \brief SortListByName::addRefPlane
 * \param p
 */
void SortListByName::addRefPlane(Plane p){
    if(p.name != ""){
        if(this->plane.contains(p.name)){ //if there is yet a loc or ref plane with this name available
            QMap<QString, LocRefBool>::Iterator idx = this->plane.find(p.name);
            if(idx != this->plane.end()){
                LocRefBool checkAvailable = idx.value();
                if(checkAvailable.ref == false){ //if there is no ref plane with the same name
                    this->refPlanes.append(p);
                    checkAvailable.ref = true;
                    checkAvailable.refIndex = this->refPlanes.size()-1;
                    this->plane.insert(p.name, checkAvailable);
                }
            }
        }else{ //if there is not a plane with this name available
            this->refPlanes.append(p);
            LocRefBool checkAvailable;
            checkAvailable.ref = true;
            checkAvailable.loc = false;
            checkAvailable.refIndex = this->refPlanes.size()-1;
            checkAvailable.locIndex = -1;
            this->plane.insert(p.name, checkAvailable);
        }
    }
}

/*!
 * \brief SortListByName::addLocPlane
 * \param p
 */
void SortListByName::addLocPlane(Plane p){
    if(p.name != ""){
        if(this->plane.contains(p.name)){ //if there is yet a loc or ref plane with this name available
            QMap<QString, LocRefBool>::Iterator idx = this->plane.find(p.name);
            if(idx != this->plane.end()){
                LocRefBool checkAvailable = idx.value();
                if(checkAvailable.loc == false){ //if there is no loc plane with the same name
                    this->locPlanes.append(p);
                    checkAvailable.loc = true;
                    checkAvailable.locIndex = this->locPlanes.size()-1;
                    this->plane.insert(p.name, checkAvailable);
                }
            }
        }else{ //if there is not a plane with this name available
            this->locPlanes.append(p);
            LocRefBool checkAvailable;
            checkAvailable.loc = true;
            checkAvailable.ref = false;
            checkAvailable.locIndex = this->locPlanes.size()-1;
            checkAvailable.refIndex = -1;
            this->plane.insert(p.name, checkAvailable);
        }
    }
}

/*!
 * \brief SortListByName::getRefPlanes
 * \return
 */
QList<Plane> SortListByName::getRefPlanes(){
    QList<Plane> result;
    QList<QString> keys = this->plane.keys();
    foreach(QString key, keys){ //all entries in map
        QMap<QString, LocRefBool>::Iterator idx = this->plane.find(key);
        if(idx != this->plane.end()){
            LocRefBool checkAvailable = idx.value();
            if(checkAvailable.loc == true && checkAvailable.ref == true
                    && this->locPlanes.size() > checkAvailable.locIndex && checkAvailable.locIndex >= 0
                    && this->refPlanes.size() > checkAvailable.refIndex && checkAvailable.refIndex >= 0){ //if loc and ref plane are available
                //add ref plane at refIndex
                result.append(this->refPlanes.at(checkAvailable.refIndex));
            }
        }
    }
    return result;
}

/*!
 * \brief SortListByName::getLocPlanes
 * \return
 */
QList<Plane> SortListByName::getLocPlanes(){
    QList<Plane> result;
    QList<QString> keys = this->plane.keys();
    foreach(QString key, keys){ //all entries in map
        QMap<QString, LocRefBool>::Iterator idx = this->plane.find(key);
        if(idx != this->plane.end()){
            LocRefBool checkAvailable = idx.value();
            if(checkAvailable.loc == true && checkAvailable.ref == true
                    && this->locPlanes.size() > checkAvailable.locIndex && checkAvailable.locIndex >= 0
                    && this->refPlanes.size() > checkAvailable.refIndex && checkAvailable.refIndex >= 0){ //if loc and ref plane are available
                //add loc plane at locIndex
                result.append(this->locPlanes.at(checkAvailable.locIndex));
            }
        }
    }
    return result;
}

/*!
 * \brief SortListByName::addRefSphere
 * \param s
 */
void SortListByName::addRefSphere(Sphere s){
    if(s.name != ""){
        if(this->sphere.contains(s.name)){ //if there is yet a loc or ref sphere with this name available
            QMap<QString, LocRefBool>::Iterator idx = this->sphere.find(s.name);
            if(idx != this->sphere.end()){
                LocRefBool checkAvailable = idx.value();
                if(checkAvailable.ref == false){ //if there is no ref sphere with the same name
                    this->refSpheres.append(s);
                    checkAvailable.ref = true;
                    checkAvailable.refIndex = this->refSpheres.size()-1;
                    this->sphere.insert(s.name, checkAvailable);
                }
            }
        }else{ //if there is not a sphere with this name available
            this->refSpheres.append(s);
            LocRefBool checkAvailable;
            checkAvailable.ref = true;
            checkAvailable.loc = false;
            checkAvailable.refIndex = this->refSpheres.size()-1;
            checkAvailable.locIndex = -1;
            this->sphere.insert(s.name, checkAvailable);
        }
    }
}

/*!
 * \brief SortListByName::addLocSphere
 * \param s
 */
void SortListByName::addLocSphere(Sphere s){
    if(s.name != ""){
        if(this->sphere.contains(s.name)){ //if there is yet a loc or ref sphere with this name available
            QMap<QString, LocRefBool>::Iterator idx = this->sphere.find(s.name);
            if(idx != this->sphere.end()){
                LocRefBool checkAvailable = idx.value();
                if(checkAvailable.loc == false){ //if there is no loc sphere with the same name
                    this->locSpheres.append(s);
                    checkAvailable.loc = true;
                    checkAvailable.locIndex = this->locSpheres.size()-1;
                    this->sphere.insert(s.name, checkAvailable);
                }
            }
        }else{ //if there is not a sphere with this name available
            this->locSpheres.append(s);
            LocRefBool checkAvailable;
            checkAvailable.loc = true;
            checkAvailable.ref = false;
            checkAvailable.locIndex = this->locSpheres.size()-1;
            checkAvailable.refIndex = -1;
            this->sphere.insert(s.name, checkAvailable);
        }
    }
}

/*!
 * \brief SortListByName::getRefSpheres
 * \return
 */
QList<Sphere> SortListByName::getRefSpheres(){
    QList<Sphere> result;
    QList<QString> keys = this->sphere.keys();
    foreach(QString key, keys){ //all entries in map
        QMap<QString, LocRefBool>::Iterator idx = this->sphere.find(key);
        if(idx != this->sphere.end()){
            LocRefBool checkAvailable = idx.value();
            if(checkAvailable.loc == true && checkAvailable.ref == true
                    && this->locSpheres.size() > checkAvailable.locIndex && checkAvailable.locIndex >= 0
                    && this->refSpheres.size() > checkAvailable.refIndex && checkAvailable.refIndex >= 0){ //if loc and ref sphere are available
                //add ref sphere at refIndex
                result.append(this->refSpheres.at(checkAvailable.refIndex));
            }
        }
    }
    return result;
}

/*!
 * \brief SortListByName::getLocSpheres
 * \return
 */
QList<Sphere> SortListByName::getLocSpheres(){
    QList<Sphere> result;
    QList<QString> keys = this->sphere.keys();
    foreach(QString key, keys){ //all entries in map
        QMap<QString, LocRefBool>::Iterator idx = this->sphere.find(key);
        if(idx != this->sphere.end()){
            LocRefBool checkAvailable = idx.value();
            if(checkAvailable.loc == true && checkAvailable.ref == true
                    && this->locSpheres.size() > checkAvailable.locIndex && checkAvailable.locIndex >= 0
                    && this->refSpheres.size() > checkAvailable.refIndex && checkAvailable.refIndex >= 0){ //if loc and ref sphere are available
                //add loc sphere at locIndex
                result.append(this->locSpheres.at(checkAvailable.locIndex));
            }
        }
    }
    return result;
}

/*!
 * \brief SortListByName::addRefScalarEntityDistance
 * \param s
 */
void SortListByName::addRefScalarEntityDistance(ScalarEntityDistance s){
    if(s.name != ""){
        if(this->scalarEntityDistance.contains(s.name)){ //if there is yet a loc or ref scalarEntityDistance with this name available
            QMap<QString, LocRefBool>::Iterator idx = this->scalarEntityDistance.find(s.name);
            if(idx != this->scalarEntityDistance.end()){
                LocRefBool checkAvailable = idx.value();
                if(checkAvailable.ref == false){ //if there is no ref scalarEntityDistance with the same name
                    this->refScalarEntityDistances.append(s);
                    checkAvailable.ref = true;
                    checkAvailable.refIndex = this->refScalarEntityDistances.size()-1;
                    this->scalarEntityDistance.insert(s.name, checkAvailable);
                }
            }
        }else{ //if there is not a scalarEntityDistance with this name available
            this->refScalarEntityDistances.append(s);
            LocRefBool checkAvailable;
            checkAvailable.ref = true;
            checkAvailable.loc = false;
            checkAvailable.refIndex = this->refScalarEntityDistances.size()-1;
            checkAvailable.locIndex = -1;
            this->scalarEntityDistance.insert(s.name, checkAvailable);
        }
    }
}

/*!
 * \brief SortListByName::addLocScalarEntityDistance
 * \param s
 */
void SortListByName::addLocScalarEntityDistance(ScalarEntityDistance s){
    if(s.name != ""){
        if(this->scalarEntityDistance.contains(s.name)){ //if there is yet a loc or ref scalarEntityDistance with this name available
            QMap<QString, LocRefBool>::Iterator idx = this->sphere.find(s.name);
            if(idx != this->scalarEntityDistance.end()){
                LocRefBool checkAvailable = idx.value();
                if(checkAvailable.loc == false){ //if there is no loc scalarEntityDistance with the same name
                    this->locScalarEntityDistances.append(s);
                    checkAvailable.loc = true;
                    checkAvailable.locIndex = this->locScalarEntityDistances.size()-1;
                    this->scalarEntityDistance.insert(s.name, checkAvailable);
                }
            }
        }else{ //if there is not a scalarEntityDistance with this name available
            this->locScalarEntityDistances.append(s);
            LocRefBool checkAvailable;
            checkAvailable.loc = true;
            checkAvailable.ref = false;
            checkAvailable.locIndex = this->locScalarEntityDistances.size()-1;
            checkAvailable.refIndex = -1;
            this->scalarEntityDistance.insert(s.name, checkAvailable);
        }
    }
}

/*!
 * \brief SortListByName::getRefScalarEntityDistances
 * \return
 */
QList<ScalarEntityDistance> SortListByName::getRefScalarEntityDistances(){
    QList<ScalarEntityDistance> result;
    QList<QString> keys = this->scalarEntityDistance.keys();
    foreach(QString key, keys){ //all entries in map
        QMap<QString, LocRefBool>::Iterator idx = this->scalarEntityDistance.find(key);
        if(idx != this->scalarEntityDistance.end()){
            LocRefBool checkAvailable = idx.value();
            if(checkAvailable.loc == true && checkAvailable.ref == true
                    && this->locScalarEntityDistances.size() > checkAvailable.locIndex && checkAvailable.locIndex >= 0
                    && this->refScalarEntityDistances.size() > checkAvailable.refIndex && checkAvailable.refIndex >= 0){ //if loc and ref sphere are available
                //add ref scalarEntityDistance at refIndex
                result.append(this->refScalarEntityDistances.at(checkAvailable.refIndex));
            }
        }
    }
    return result;
}

/*!
 * \brief SortListByName::getLocScalarEntityDistances
 * \return
 */
QList<ScalarEntityDistance> SortListByName::getLocScalarEntityDistances(){
    QList<ScalarEntityDistance> result;
    QList<QString> keys = this->scalarEntityDistance.keys();
    foreach(QString key, keys){ //all entries in map
        QMap<QString, LocRefBool>::Iterator idx = this->scalarEntityDistance.find(key);
        if(idx != this->scalarEntityDistance.end()){
            LocRefBool checkAvailable = idx.value();
            if(checkAvailable.loc == true && checkAvailable.ref == true
                    && this->locScalarEntityDistances.size() > checkAvailable.locIndex && checkAvailable.locIndex >= 0
                    && this->refScalarEntityDistances.size() > checkAvailable.refIndex && checkAvailable.refIndex >= 0){ //if loc and ref sphere are available
                //add loc scalarEntityDistance at locIndex
                result.append(this->locScalarEntityDistances.at(checkAvailable.locIndex));
            }
        }
    }
    return result;
}

/*!
 * \brief SortListByName::addRefScalarEntityAngle
 * \param s
 */
void SortListByName::addRefScalarEntityAngle(ScalarEntityAngle s){
    if(s.name != ""){
        if(this->scalarEntityAngle.contains(s.name)){ //if there is yet a loc or ref scalarEntityAngle with this name available
            QMap<QString, LocRefBool>::Iterator idx = this->scalarEntityAngle.find(s.name);
            if(idx != this->scalarEntityAngle.end()){
                LocRefBool checkAvailable = idx.value();
                if(checkAvailable.ref == false){ //if there is no ref scalarEntityAngle with the same name
                    this->refScalarEntityAngles.append(s);
                    checkAvailable.ref = true;
                    checkAvailable.refIndex = this->refScalarEntityAngles.size()-1;
                    this->scalarEntityAngle.insert(s.name, checkAvailable);
                }
            }
        }else{ //if there is not a scalarEntityAngle with this name available
            this->refScalarEntityAngles.append(s);
            LocRefBool checkAvailable;
            checkAvailable.ref = true;
            checkAvailable.loc = false;
            checkAvailable.refIndex = this->refScalarEntityAngles.size()-1;
            checkAvailable.locIndex = -1;
            this->scalarEntityAngle.insert(s.name, checkAvailable);
        }
    }
}

/*!
 * \brief SortListByName::addLocScalarEntityAngle
 * \param s
 */
void SortListByName::addLocScalarEntityAngle(ScalarEntityAngle s){
    if(s.name != ""){
        if(this->scalarEntityAngle.contains(s.name)){ //if there is yet a loc or ref scalarEntityAngle with this name available
            QMap<QString, LocRefBool>::Iterator idx = this->scalarEntityAngle.find(s.name);
            if(idx != this->scalarEntityAngle.end()){
                LocRefBool checkAvailable = idx.value();
                if(checkAvailable.loc == false){ //if there is no loc scalarEntityAngle with the same name
                    this->locScalarEntityAngles.append(s);
                    checkAvailable.loc = true;
                    checkAvailable.locIndex = this->locScalarEntityAngles.size()-1;
                    this->scalarEntityAngle.insert(s.name, checkAvailable);
                }
            }
        }else{ //if there is not a scalarEntityAngle with this name available
            this->locScalarEntityAngles.append(s);
            LocRefBool checkAvailable;
            checkAvailable.loc = true;
            checkAvailable.ref = false;
            checkAvailable.locIndex = this->locScalarEntityAngles.size()-1;
            checkAvailable.refIndex = -1;
            this->scalarEntityAngle.insert(s.name, checkAvailable);
        }
    }
}

/*!
 * \brief SortListByName::getRefScalarEntityAngles
 * \return
 */
QList<ScalarEntityAngle> SortListByName::getRefScalarEntityAngles(){
    QList<ScalarEntityAngle> result;
    QList<QString> keys = this->scalarEntityAngle.keys();
    foreach(QString key, keys){ //all entries in map
        QMap<QString, LocRefBool>::Iterator idx = this->scalarEntityAngle.find(key);
        if(idx != this->scalarEntityAngle.end()){
            LocRefBool checkAvailable = idx.value();
            if(checkAvailable.loc == true && checkAvailable.ref == true
                    && this->locScalarEntityAngles.size() > checkAvailable.locIndex && checkAvailable.locIndex >= 0
                    && this->refScalarEntityAngles.size() > checkAvailable.refIndex && checkAvailable.refIndex >= 0){ //if loc and ref sphere are available
                //add ref scalarEntityAngle at refIndex
                result.append(this->refScalarEntityAngles.at(checkAvailable.refIndex));
            }
        }
    }
    return result;
}

/*!
 * \brief SortListByName::getLocScalarEntityAngles
 * \return
 */
QList<ScalarEntityAngle> SortListByName::getLocScalarEntityAngles(){
    QList<ScalarEntityAngle> result;
    QList<QString> keys = this->scalarEntityAngle.keys();
    foreach(QString key, keys){ //all entries in map
        QMap<QString, LocRefBool>::Iterator idx = this->scalarEntityAngle.find(key);
        if(idx != this->scalarEntityAngle.end()){
            LocRefBool checkAvailable = idx.value();
            if(checkAvailable.loc == true && checkAvailable.ref == true
                    && this->locScalarEntityAngles.size() > checkAvailable.locIndex && checkAvailable.locIndex >= 0
                    && this->refScalarEntityAngles.size() > checkAvailable.refIndex && checkAvailable.refIndex >= 0){ //if loc and ref sphere are available
                //add loc scalarEntityAngle at locIndex
                result.append(this->locScalarEntityAngles.at(checkAvailable.locIndex));
            }
        }
    }
    return result;
}
