#include "sphere.h"

#include "function.h"

Sphere::Sphere(bool isNominal, QObject *parent) : Geometry(isNominal, parent), xyz(4), radius(0.0)
{
    this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

/*!
 * \brief Sphere::Sphere
 * \param copy
 */
Sphere::Sphere(const Sphere &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->xyz = copy.xyz;
    this->radius = copy.radius;
    this->isSolved = copy.isSolved;
}

Sphere::~Sphere(){

}

/*!
 * \brief Sphere::getXYZ returns the xyz vector
 * \return
 */
OiVec Sphere::getXYZ() const
{
    return this->xyz;
}

double Sphere::getRadius() const
{
    return this->radius;
}

/*!
 * \brief Sphere::recalc
 * Execute alls functions in the specified order
 */
void Sphere::recalc(){

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
        this->radius = 0.0;
        this->setIsSolved(false);

    }

}

bool Sphere::toOpenIndyXML(QXmlStreamWriter &stream){

    //---------------common geometry attributes--------------
    stream.writeStartElement("geometry");
    stream.writeAttribute("id", QString::number(this->id));
    stream.writeAttribute("name", this->name);
    stream.writeAttribute("type", Configuration::sSphere);
    stream.writeAttribute("nominal",QString::number(this->isNominal));
    stream.writeAttribute("common",QString::number(this->isCommon));
    stream.writeAttribute("solved", QString::number(this->isSolved));


    //---------------specific geometry attributes--------------
    if(this->isSolved || this->isNominal){
            stream.writeStartElement("coordinates");
            stream.writeAttribute("x", QString::number(this->xyz.getAt(0)));
            stream.writeAttribute("y", QString::number(this->xyz.getAt(1)));
            stream.writeAttribute("z", QString::number(this->xyz.getAt(2)));
            stream.writeEndElement();

            stream.writeStartElement("radius");
            stream.writeAttribute("value", QString::number(this->radius));
            stream.writeEndElement();

            stream.writeStartElement("standardDeviation");
            stream.writeAttribute("value", QString::number(this->myStatistic.stdev));
            stream.writeEndElement();
    }

   this->writeGeometryAttributes(stream);

   stream.writeEndElement();

    return true;

}

/*!
 * \brief Sphere::fromOpenIndyXML
 * \param xml
 * \return
 */
ElementDependencies Sphere::fromOpenIndyXML(QXmlStreamReader &xml){
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

            }else{
                this->readGeometryAttributes(xml,dependencies);
                xml.readNext();
            }

        }else if(xml.name() == "radius"){

            if(xml.tokenType() == QXmlStreamReader::StartElement) {

                QXmlStreamAttributes radiusAttributes = xml.attributes();

                if(radiusAttributes.hasAttribute("value")){
                   this->radius = radiusAttributes.value("value").toDouble();
                }
            }
            xml.readNext();

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

                }else if(xml.name() == "radius"){

                        if(xml.tokenType() == QXmlStreamReader::StartElement) {

                                QXmlStreamAttributes radiusAttributes = xml.attributes();

                                if(radiusAttributes.hasAttribute("value")){
                                   this->radius = radiusAttributes.value("value").toDouble();
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

bool Sphere::saveSimulationData()
{


    this->mySimulationData.addXYZ(this->xyz.getAt(0),this->xyz.getAt(1),this->xyz.getAt(2));

    this->mySimulationData.addRadius(this->radius);

    return true;
}

QString Sphere::getDisplayX() const{

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

QString Sphere::getDisplayY() const{

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

QString Sphere::getDisplayZ() const{
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

QString Sphere::getDisplayRadius() const{

    QString value = QString::number(this->radius*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);

    if(!this->isNominal){
        if(this->isSolved){
            foreach (Geometry *g, this->nominals) {
                if(g != NULL && g->getIsSolved()){

                    QString diff = QString::number((this->radius - g->getRadius())*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                    return QString(value + " (" + diff + ")" );
                }
            }
        }
    }

    return value;
}

QString Sphere::getDisplayIsCommon() const{
    return QString(isCommon?"true":"false");
}

QString Sphere::getDisplayIsNominal() const{
    return QString(isNominal?"true":"false");
}
/*QString Sphere::getDisplayObs() const{
    return QString::number(this->myObservations.size());
}*/

QString Sphere::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString Sphere::getDisplayMConfig() const{
    return this->mConfig.name;
}

QString Sphere::getDisplayStdDev() const{

    if(this->myStatistic.isValid){
        return QString::number(this->myStatistic.stdev*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
    }else{
        return "-/-";
    }
}
