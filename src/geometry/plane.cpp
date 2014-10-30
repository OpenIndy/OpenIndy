#include "plane.h"

#include "function.h"

Plane::Plane(bool isNominal, QObject *parent) : Geometry(isNominal, parent), xyz(4), ijk(4)
{
    this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

/*!
 * \brief Plane::Plane
 * \param copy
 */
Plane::Plane(const Plane &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->xyz = copy.xyz;
    this->ijk = copy.ijk;
    this->isSolved = copy.isSolved;
}

Plane::~Plane(){

}

/*!
 * \brief Plane::getXYZ returns xyz vector
 * \return
 */
OiVec Plane::getXYZ() const
{
    return this->xyz;
}

/*!
 * \brief Plane::getIJK returns ijk vector
 * \return
 */
OiVec Plane::getIJK() const
{
    return this->ijk;
}

/*!
 * \brief Plane::recalc
 * Execute all functions in the specified order
 */
void Plane::recalc(){

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

        this->xyz = OiVec(4);
        this->ijk = OiVec(4);
        this->setIsSolved(false);

    }

}

/*!
 * \brief Plane::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Plane::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement plane = Geometry::toOpenIndyXML(xmlDoc);

    if(plane.isNull()){
        return plane;
    }

    plane.setAttribute("type", Configuration::sPlane);

    //add normal vector
    QDomElement ijk = xmlDoc.createElement("spatialDirection");
    if(this->ijk.getSize() >= 3 && this->getIsSolved()){
        ijk.setAttribute("i", this->ijk.getAt(0));
        ijk.setAttribute("j", this->ijk.getAt(1));
        ijk.setAttribute("k", this->ijk.getAt(2));
    }else{
        ijk.setAttribute("i", 0.0);
        ijk.setAttribute("j", 0.0);
        ijk.setAttribute("k", 0.0);
    }

    return plane;

}

/*!
 * \brief Plane::fromOpenIndyXML
 * \param xml
 * \return
 */
ElementDependencies Plane::fromOpenIndyXML(QXmlStreamReader &xml){
    ElementDependencies dependencies;

    QXmlStreamAttributes attributes = xml.attributes();

    if(attributes.hasAttribute("name")){
        this->name = attributes.value("name").toString();
    }
    if(attributes.hasAttribute("id")) {
        this->id = attributes.value("id").toInt();
        dependencies.elementID = this->id;
    }
    if(attributes.hasAttribute("nominal")) {
        this->isNominal = attributes.value("nominal").toInt();
    }
    if(attributes.hasAttribute("common")) {
        this->isCommon = attributes.value("common").toInt();
    }
    if(attributes.hasAttribute("solved")) {
        this->isSolved= attributes.value("solved").toInt();
    }
    if(attributes.hasAttribute("group")) {
        this->group= attributes.value("group").toString();
    }
    if(attributes.hasAttribute("comment")) {
        this->comment= attributes.value("comment").toString();
    }

    xml.readNext();

    while( !xml.atEnd() && xml.name().compare("geometry") != 0 ){

        if(xml.tokenType() == QXmlStreamReader::StartElement) {

            if(xml.name() == "coordinates"){

                QXmlStreamAttributes coordinatesAttributes = xml.attributes();

                if(coordinatesAttributes.hasAttribute("x")){
                  this->xyz.setAt(0,coordinatesAttributes.value("x").toDouble());
                }
                if(coordinatesAttributes.hasAttribute("y")){
                  this->xyz.setAt(1,coordinatesAttributes.value("y").toDouble());
                }
                if(coordinatesAttributes.hasAttribute("z")){
                  this->xyz.setAt(2,coordinatesAttributes.value("z").toDouble());
                }

                xml.readNext();

            }else if(xml.name() == "spatialDirection"){

                QXmlStreamAttributes spatialDirectionAttributes = xml.attributes();

                if(spatialDirectionAttributes.hasAttribute("i")){
                   this->ijk.setAt(0,spatialDirectionAttributes.value("i").toDouble());
                }
                if(spatialDirectionAttributes.hasAttribute("j")){
                   this->ijk.setAt(1,spatialDirectionAttributes.value("j").toDouble());
                }
                if(spatialDirectionAttributes.hasAttribute("k")){
                   this->ijk.setAt(2,spatialDirectionAttributes.value("k").toDouble());
                }
                xml.readNext();

            }else{
                this->readGeometryAttributes(xml,dependencies);
                xml.readNext();
            }

        }else{
            xml.readNext();
        }

    }

    return dependencies;




    /*while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                xml.name() == "geometry")) {
            if(xml.tokenType() == QXmlStreamReader::StartElement) {

                if(xml.name() == "coordinates") {

                        if(xml.tokenType() == QXmlStreamReader::StartElement) {

                            QXmlStreamAttributes coordinatesAttributes = xml.attributes();

                                if(coordinatesAttributes.hasAttribute("x")){
                                  this->xyz.setAt(0,coordinatesAttributes.value("x").toDouble());
                                }

                                if(coordinatesAttributes.hasAttribute("y")){
                                  this->xyz.setAt(1,coordinatesAttributes.value("y").toDouble());
                                }

                                if(coordinatesAttributes.hasAttribute("z")){
                                  this->xyz.setAt(2,coordinatesAttributes.value("z").toDouble());
                                }
                         }

                }else if(xml.name() == "spatialDirection"){

                        if(xml.tokenType() == QXmlStreamReader::StartElement) {

                            QXmlStreamAttributes spatialDirectionAttributes = xml.attributes();

                            if(spatialDirectionAttributes.hasAttribute("i")){
                               this->ijk.setAt(0,spatialDirectionAttributes.value("i").toDouble());
                            }

                            if(spatialDirectionAttributes.hasAttribute("j")){
                               this->ijk.setAt(1,spatialDirectionAttributes.value("j").toDouble());
                            }

                            if(spatialDirectionAttributes.hasAttribute("k")){
                               this->ijk.setAt(2,spatialDirectionAttributes.value("k").toDouble());
                            }
                        }

                }else{
                         this->readGeometryAttributes(xml,dependencies);
                }

            }
            xml.readNext();
        }


    return dependencies;*/
}

bool Plane::saveSimulationData()
{
    this->mySimulationData.addXYZ(this->xyz.getAt(0),this->xyz.getAt(1),this->xyz.getAt(2));

    this->mySimulationData.addIJK(this->ijk.getAt(0),this->ijk.getAt(1),this->ijk.getAt(2));
    return true;
}

QString Plane::getDisplayX() const{

    QString value = QString::number(this->xyz.getAt(0)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);

    if(!this->isNominal){
        if(this->isSolved){
           foreach(Geometry *g, this->nominals){
               if(g != NULL && g->getIsSolved()){

                   QString diff = QString::number((this->xyz.getAt(0)-g->getXYZ().getAt(0))*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                   return QString(value + " (" + diff + ")");
               }
           }
       }
    }

    return value;

}

QString Plane::getDisplayY() const{

    QString value = QString::number(this->xyz.getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);

    if(!this->isNominal){
        if(this->isSolved){
           foreach(Geometry *g, this->nominals){
               if(g != NULL && g->getIsSolved()){

                   QString diff = QString::number((this->xyz.getAt(1)-g->getXYZ().getAt(1))*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                   return QString(value + " (" + diff + ")");
               }
           }
       }
    }

    return value;
}

QString Plane::getDisplayZ() const{
    QString value = QString::number(this->xyz.getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);

    if(!this->isNominal){
        if(this->isSolved){
           foreach(Geometry *g, this->nominals){
               if(g != NULL && g->getIsSolved()){

                   QString diff = QString::number((this->xyz.getAt(2)-g->getXYZ().getAt(2))*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                   return QString(value + " (" + diff + ")");
               }
           }
       }
    }

    return value;
}

QString Plane::getDisplayI() const{
    QString value = QString::number(this->ijk.getAt(0),'f',6);

    if(!this->isNominal){
        if(this->isSolved){
            foreach (Geometry *g, this->nominals) {
                if(g != NULL && g->getIsSolved()){

                    QString diff = QString::number((this->ijk.getAt(0)-g->getIJK().getAt(0)),'f',6);
                    return QString(value + " (" + diff + ")");
                }
            }
        }
    }

    return value;
}

QString Plane::getDisplayJ() const{
    QString value = QString::number(this->ijk.getAt(1),'f',6);

    if(!this->isNominal){
        if(this->isSolved){
            foreach (Geometry *g, this->nominals) {
                if(g != NULL && g->getIsSolved()){

                    QString diff = QString::number((this->ijk.getAt(1)-g->getIJK().getAt(1)),'f',6);
                    return QString(value + " (" + diff + ")");
                }
            }
        }
    }

    return value;
}

QString Plane::getDisplayK() const{
    QString value = QString::number(this->ijk.getAt(2),'f',6);

    if(!this->isNominal){
        if(this->isSolved){
            foreach (Geometry *g, this->nominals) {
                if(g != NULL && g->getIsSolved()){

                    QString diff = QString::number((this->ijk.getAt(2)-g->getIJK().getAt(2)),'f',6);
                    return QString(value + " (" + diff + ")");
                }
            }
        }
    }

    return value;
}

QString Plane::getDisplayIsCommon() const{
    return QString(isCommon?"true":"false");
}

QString Plane::getDisplayIsNominal() const{
    return QString(isNominal?"true":"false");
}

/*QString Plane::getDisplayObs() const{
    return QString::number(this->myObservations.size());
}*/

QString Plane::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString Plane::getDisplayMConfig() const{
    return this->mConfig.name;
}

QString Plane::getDisplayStdDev() const{

    if(this->myStatistic.isValid){
        return QString::number(this->myStatistic.stdev*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
    }else{
        return "-/-";
    }
}
