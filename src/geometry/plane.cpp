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

bool Plane::toOpenIndyXML(QXmlStreamWriter &stream){

    //---------------common geometry attributes--------------
    stream.writeStartElement("geometry");
    stream.writeAttribute("id", QString::number(this->id));
    stream.writeAttribute("name", this->name);
    stream.writeAttribute("type", Configuration::sPoint);
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

            stream.writeStartElement("spatialDirection");
            stream.writeAttribute("i", QString::number(this->ijk.getAt(0)));
            stream.writeAttribute("j", QString::number(this->ijk.getAt(1)));
            stream.writeAttribute("k", QString::number(this->ijk.getAt(2)));
            stream.writeEndElement();

            stream.writeStartElement("standardDeviation");
            stream.writeAttribute("value", QString::number(this->myStatistic.stdev));
            stream.writeEndElement();
    }

   this->writeGeometryAttributes(stream);

   stream.writeEndElement();

    return true;

}

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

    xml.readNext();

    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                xml.name() == "geometry")) {
            if(xml.tokenType() == QXmlStreamReader::StartElement) {
                /* We've found first name. */
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


    return dependencies;
}

bool Plane::saveSimulationData()
{
    OiVec tmpXyz(3);

    tmpXyz.setAt(0,this->xyz.getAt(0));
    tmpXyz.setAt(1,this->xyz.getAt(1));
    tmpXyz.setAt(2,this->xyz.getAt(2));

    OiVec tmpIjk(3);

    tmpIjk.setAt(0,this->ijk.getAt(0));
    tmpIjk.setAt(1,this->ijk.getAt(1));
    tmpIjk.setAt(2,this->ijk.getAt(2));

    this->myStatistic.simulationData.xyz.append(tmpXyz);
    this->myStatistic.simulationData.ijk.append(tmpIjk);

    return true;
}

QString Plane::getDisplayX() const{
    return QString::number(this->xyz.getAt(0)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString Plane::getDisplayY() const{
    return QString::number(this->xyz.getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString Plane::getDisplayZ() const{
    return QString::number(this->xyz.getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString Plane::getDisplayI() const{
    return QString::number(this->ijk.getAt(0),'f',6);
}

QString Plane::getDisplayJ() const{
    return QString::number(this->ijk.getAt(1),'f',6);
}

QString Plane::getDisplayK() const{
    return QString::number(this->ijk.getAt(2),'f',6);
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
