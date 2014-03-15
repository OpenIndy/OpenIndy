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

