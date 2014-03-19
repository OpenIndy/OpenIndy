#include "trafoparam.h"

#include "coordinatesystem.h"
#include "statistic.h"
#include "function.h"

TrafoParam::TrafoParam() : homogenMatrix(4, 4), translation(3), rotation(3), scale(3), quaternion(4)
{
    this->id = Configuration::generateID();
    this->isUpdated = false;
}

TrafoParam::~TrafoParam(){

}

/*!
 * \brief TrafoParam::recalc
 */
void TrafoParam::recalc(){
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
}
/*!
 * \brief set up the homogenious matrix for transformation.
 * This function is called, if the user sets some values for transformation by hand via the transformation parameter
 * input dialog.
 */
void TrafoParam::generateHomogenMatrix()
{
    OiMat tmpTranslation(4,4);
    OiMat tmpRotation(4,4);
    OiMat tmpScale(4,4);

    tmpTranslation.setAt(0,0,1.0);
    tmpTranslation.setAt(0,1,0.0);
    tmpTranslation.setAt(0,2,0.0);
    tmpTranslation.setAt(0,3,this->translation.getAt(0));
    tmpTranslation.setAt(1,0,0.0);
    tmpTranslation.setAt(1,1,1.0);
    tmpTranslation.setAt(1,2,0.0);
    tmpTranslation.setAt(1,3,this->translation.getAt(1));
    tmpTranslation.setAt(2,0,0.0);
    tmpTranslation.setAt(2,1,0.0);
    tmpTranslation.setAt(2,2,1.0);
    tmpTranslation.setAt(2,3,this->translation.getAt(2));
    tmpTranslation.setAt(3,0,0.0);
    tmpTranslation.setAt(3,1,0.0);
    tmpTranslation.setAt(3,2,0.0);
    tmpTranslation.setAt(3,3,1.0);

    tmpScale.setAt(0,0,this->scale.getAt(0));
    tmpScale.setAt(0,1,0.0);
    tmpScale.setAt(0,2,0.0);
    tmpScale.setAt(0,3,0.0);
    tmpScale.setAt(1,0,0.0);
    tmpScale.setAt(1,1,this->scale.getAt(1));
    tmpScale.setAt(1,2,0.0);
    tmpScale.setAt(1,3,0.0);
    tmpScale.setAt(2,0,0.0);
    tmpScale.setAt(2,1,0.0);
    tmpScale.setAt(2,2,this->scale.getAt(2));
    tmpScale.setAt(2,3,0.0);
    tmpScale.setAt(3,0,0.0);
    tmpScale.setAt(3,1,0.0);
    tmpScale.setAt(3,2,0.0);
    tmpScale.setAt(3,3,1.0);

    tmpRotation.setAt(0,0,qCos(this->rotation.getAt(1))*qCos(this->rotation.getAt(2)));
    tmpRotation.setAt(0,1,qCos(this->rotation.getAt(0))*qSin(this->rotation.getAt(2))+qSin(this->rotation.getAt(0))*qSin(this->rotation.getAt(1))*qCos(this->rotation.getAt(2)));
    tmpRotation.setAt(0,2,qSin(this->rotation.getAt(0))*qSin(this->rotation.getAt(2))-qCos(this->rotation.getAt(0))*qSin(this->rotation.getAt(1))*qCos(this->rotation.getAt(2)));
    tmpRotation.setAt(0,3,0.0);
    tmpRotation.setAt(1,0,-qCos(this->rotation.getAt(1))*qSin(this->rotation.getAt(2)));
    tmpRotation.setAt(1,1,qCos(this->rotation.getAt(0))*qCos(this->rotation.getAt(2))-qSin(this->rotation.getAt(0))*qSin(this->rotation.getAt(1))*qSin(this->rotation.getAt(2)));
    tmpRotation.setAt(1,2,qSin(this->rotation.getAt(0))*qCos(this->rotation.getAt(2))+qCos(this->rotation.getAt(0))*qSin(this->rotation.getAt(1))*qSin(this->rotation.getAt(2)));
    tmpRotation.setAt(1,3,0.0);
    tmpRotation.setAt(2,0,qSin(this->rotation.getAt(1)));
    tmpRotation.setAt(2,1,-qSin(this->rotation.getAt(0))*qCos(this->rotation.getAt(2)));
    tmpRotation.setAt(2,2,qCos(this->rotation.getAt(0))*qCos(this->rotation.getAt(1)));
    tmpRotation.setAt(2,3,0.0);
    tmpRotation.setAt(3,0,0.0);
    tmpRotation.setAt(3,1,0.0);
    tmpRotation.setAt(3,2,0.0);
    tmpRotation.setAt(3,3,1.0);

    this->homogenMatrix = tmpTranslation*tmpScale*tmpRotation;
}

bool TrafoParam::toOpenIndyXML(QXmlStreamWriter &stream){

    stream.writeStartElement("transformationsparameter");
    stream.writeAttribute("id", QString::number(this->id));
    stream.writeAttribute("name", this->name);
    stream.writeAttribute("solved", QString::number(this->isSolved));
    stream.writeAttribute("tx", QString::number(this->translation.getAt(0)));
    stream.writeAttribute("ty", QString::number(this->translation.getAt(1)));
    stream.writeAttribute("tz", QString::number(this->translation.getAt(2)));
    stream.writeAttribute("rx", QString::number(this->rotation.getAt(0)));
    stream.writeAttribute("ry", QString::number(this->rotation.getAt(1)));
    stream.writeAttribute("rz", QString::number(this->rotation.getAt(2)));
    stream.writeAttribute("mx", QString::number(this->scale.getAt(0)));
    stream.writeAttribute("my", QString::number(this->scale.getAt(1)));
    stream.writeAttribute("mz", QString::number(this->scale.getAt(2)));


    stream.writeStartElement("from");
    stream.writeAttribute("type", "coordinatesystem");
    stream.writeAttribute("ref", QString::number(this->from->id));
    stream.writeEndElement();

    stream.writeStartElement("to");
    stream.writeAttribute("type", "coordinatesystem");
    stream.writeAttribute("ref", QString::number(this->to->id));
    stream.writeEndElement();


    this->writeFeatureAttributes(stream);


    stream.writeEndElement();

    return true;
}

ElementDependencies TrafoParam::fromOpenIndyXML(QXmlStreamReader &xml){

    ElementDependencies dependencies;


    QXmlStreamAttributes attributes = xml.attributes();

    if(attributes.hasAttribute("name")){
        this->name = attributes.value("name").toString();
    }
    if(attributes.hasAttribute("id")) {
        this->id = attributes.value("id").toInt();
        dependencies.elementID = this->id;
    }
    if(attributes.hasAttribute("tx")) {
        this->translation.setAt(0,attributes.value("tx").toDouble());
    }
    if(attributes.hasAttribute("ty")) {
        this->translation.setAt(1,attributes.value("ty").toDouble());
    }
    if(attributes.hasAttribute("tz")) {
        this->translation.setAt(2,attributes.value("tz").toDouble());
    }
    if(attributes.hasAttribute("rx")) {
        this->rotation.setAt(0,attributes.value("rx").toDouble());
    }
    if(attributes.hasAttribute("ry")) {
        this->rotation.setAt(1,attributes.value("ry").toDouble());
    }
    if(attributes.hasAttribute("rz")) {
        this->rotation.setAt(2,attributes.value("rz").toDouble());
    }
    if(attributes.hasAttribute("mx")) {
        this->scale.setAt(0,attributes.value("mx").toDouble());
    }
    if(attributes.hasAttribute("my")) {
        this->scale.setAt(1,attributes.value("my").toDouble());
    }
    if(attributes.hasAttribute("mz")) {
        this->scale.setAt(2,attributes.value("mz").toDouble());
    }

    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named transformationparameter.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "transformationsparameter")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found first name. */

            if(xml.name() == "from") {


                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "from")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes fromAttributes = xml.attributes();

                        if(fromAttributes.hasAttribute("ref")){

                        }

                    }

                    xml.readNext();
                }


            }

            if(xml.name() == "to") {


                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "to")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes toAttributes = xml.attributes();

                        if(toAttributes.hasAttribute("ref")){
                            CoordinateSystem *tmpCoord = new CoordinateSystem();
                            tmpCoord->id = toAttributes.value("ref").toInt();
                            this->to = tmpCoord;
                        }

                    }

                    xml.readNext();
                }


            }

            if(xml.name() == "member"){

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "member")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes memberAttributes = xml.attributes();

                        if(memberAttributes.hasAttribute("type")){

                        this->readFeatureAttributes(xml,dependencies);

                        }
                    }
                    /* ...and next... */
                    xml.readNext();
                }

            }


            if(xml.name() == "function"){

                this->readFunction(xml,dependencies);

            }


        }
        /* ...and next... */
        xml.readNext();
    }



    return dependencies;
}

QString TrafoParam::getDisplayStartSystem() const{
    return this->from->name;
}

QString TrafoParam::getDisplayDestinationSystem() const{
    return this->to->name;
}

QString TrafoParam::getDisplayTranslationX() const
{
    return QString::number(this->translation.getAt(0)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString TrafoParam::getDisplayTranslationY() const
{
    return QString::number(this->translation.getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString TrafoParam::getDisplayTranslationZ() const
{
    return QString::number(this->translation.getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString TrafoParam::getDisplayRotationX() const
{
    return QString::number(this->rotation.getAt(0)*UnitConverter::getAngleMultiplier(),'f',UnitConverter::angleDigits);
}

QString TrafoParam::getDisplayRotationY() const
{
    return QString::number(this->rotation.getAt(1)*UnitConverter::getAngleMultiplier(),'f',UnitConverter::angleDigits);
}

QString TrafoParam::getDisplayRotationZ() const
{
    return QString::number(this->rotation.getAt(2)*UnitConverter::getAngleMultiplier(),'f',UnitConverter::angleDigits);
}

QString TrafoParam::getDisplayScaleX() const
{
    return QString::number(this->scale.getAt(0),'f',6);
}

QString TrafoParam::getDisplayScaleY() const
{
    return QString::number(this->scale.getAt(1),'f',6);
}

QString TrafoParam::getDisplayScaleZ() const
{
    return QString::number(this->scale.getAt(2),'f',6);
}
