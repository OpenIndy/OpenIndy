#include "feature.h"

#include "function.h"
#include "featurewrapper.h"

Feature::~Feature(){

}

/*!
 * \brief Feature::addFunction
 * \param f
 */
void Feature::addFunction(Function *f){
    if(f != NULL){
        this->functionList.append(f);
    }
}


bool Feature::writeFeatureAttributes(QXmlStreamWriter &stream){

    for(int k =0;k<this ->usedFor.size();k++){
        stream.writeStartElement("member");
        stream.writeAttribute("type", "usedForFeature");
        stream.writeAttribute("ref", QString::number(this->usedFor.at(k)->getFeature()->id));
        stream.writeEndElement();
    }

    for(int k =0;k<this->previouslyNeeded.size();k++){
        stream.writeStartElement("member");
        stream.writeAttribute("type", "previouslyNeeded");
        stream.writeAttribute("ref", QString::number(this->previouslyNeeded.at(k)->getFeature()->id));
        stream.writeEndElement();
    }

    for(int k =0;k<this->functionList.size();k++){
        stream.writeStartElement("function");
        stream.writeAttribute("name", this->functionList.at(k)->getMetaData()->name);
        stream.writeAttribute("plugin", this->functionList.at(k)->getMetaData()->pluginName);
        stream.writeAttribute("executionIndex", QString::number(k));

        QMapIterator<int, QList<InputFeature> > j(this->functionList.at(k)->getFeatureOrder());
        while (j.hasNext()) {
            j.next();

            for(int m = 0; m < j.value().size();m++){
                stream.writeStartElement("inputElement");
                stream.writeAttribute("index", QString::number(j.key()));
                int neededFeatureId = j.value().at(m).id;
                stream.writeAttribute("type", QString::number( j.value().at(m).typeOfElement));
                stream.writeAttribute("ref", QString::number(neededFeatureId));
                stream.writeEndElement();
            }


        }

        stream.writeEndElement();
    }

    return true;

}

bool Feature::readFeatureAttributes(QXmlStreamReader &xml, ElementDependencies dependencies){

    if(xml.name() == "member"){

            if(xml.tokenType() == QXmlStreamReader::StartElement) {

                QXmlStreamAttributes memberAttributes = xml.attributes();

                if(memberAttributes.hasAttribute("type")){

                    if (memberAttributes.value("type") == "usedForFeature"){

                        if(memberAttributes.hasAttribute("ref")){
                            dependencies.addFeatureID(memberAttributes.value("ref").toInt(),"usedForFeature");
                        }
                    }

                    if (memberAttributes.value("type") == "previouslyNeeded"){

                        if(memberAttributes.hasAttribute("ref")){
                            dependencies.addFeatureID(memberAttributes.value("ref").toInt(),"previouslyNeeded");
                        }
                    }
                }

            }

    return true;
    }
}

bool Feature::readFunction(QXmlStreamReader &xml, ElementDependencies d){

    functionInfo f;

    if(xml.name() == "function"){

        QXmlStreamAttributes attributes = xml.attributes();

        if(attributes.hasAttribute("name")){
            f.name= attributes.value("name").toString();
        }
        if(attributes.hasAttribute("plugin")) {
            f.plugin = attributes.value("id").toInt();
        }
        if(attributes.hasAttribute("executionIndex")) {
            f.executionIndex = attributes.value("executionIndex").toInt();
        }


        while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                xml.name() == "function")) {
            if(xml.tokenType() == QXmlStreamReader::StartElement) {

                if(xml.name() == "inputElement"){

                    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                            xml.name() == "inputElement")) {
                        if(xml.tokenType() == QXmlStreamReader::StartElement) {

                            QXmlStreamAttributes memberAttributes = xml.attributes();

                                    featureIndex idx;

                                    if(memberAttributes.hasAttribute("index")){
                                       idx.idx = memberAttributes.value("index").toInt();
                                    }

                                    if(memberAttributes.hasAttribute("type") && memberAttributes.hasAttribute("ref")){

                                        int elementId = memberAttributes.value("ref").toInt();
                                        idx.typeOfElement =  memberAttributes.value("type").toInt();

                                        f.neededElements.insert(elementId,idx);
                                    }

                        }
                        /* ...and next... */
                        xml.readNext();
                    }
                }
            }
            /* ...and next... */
            xml.readNext();
        }

    d.addFunctionInfo(f);
    return true;
    }

}


