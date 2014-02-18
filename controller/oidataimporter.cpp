#include "oidataimporter.h"

OiDataImporter::OiDataImporter()
{
}



bool OiDataImporter::saveToXML(QList<FeatureWrapper*> features, QFile &fileToSave, int idActiveCoord){


    if(fileToSave.open(QIODevice::WriteOnly | QIODevice::Text)){

        QList<Observation*> observations;
        QList<FeatureWrapper*> geometries;
        QList<FeatureWrapper*> stations;
        QList<FeatureWrapper*> coordSystems;
        QList<FeatureWrapper*> trafoParams;

        //get current date and time
        QDateTime dateTime = QDateTime::currentDateTime();
        QString dateTimeString = dateTime.toString(Qt::ISODate);

        QFileInfo fileInfo(fileToSave.fileName());
        QString filename(fileInfo.fileName());

        QXmlStreamWriter stream(&fileToSave);

        stream.setAutoFormatting(true);
        stream.writeStartDocument();

        stream.writeStartElement("oiProjectData");
        stream.writeAttribute("name", filename);
        stream.writeAttribute("date", dateTimeString);
        stream.writeAttribute("idcount", QString::number(Configuration::idCount));

        stream.writeStartElement("member");
        stream.writeAttribute("type", "activeCoordinatesystem");
        stream.writeAttribute("ref", QString::number(idActiveCoord));
        stream.writeEndElement();

        for(int i = 0; i<features.size();i++){

         if(features.at(i)->getGeometry() != NULL ){

            geometries.append(features.at(i));

         }else if(features.at(i)->getCoordinateSystem() != NULL){

            coordSystems.append(features.at(i));

          }else if (features.at(i)->getStation() != NULL){

             stations.append(features.at(i));

          }else if(features.at(i)->getTrafoParam() != NULL){

             trafoParams.append(features.at(i));

           }
        }

        //##############################save stations##############################
        for(int i = 0; i<stations.size();i++){
            stream.writeStartElement("station");
            stream.writeAttribute("id", QString::number(stations.at(i)->getStation()->id));
            stream.writeAttribute("name", stations.at(i)->getStation()->name);

            if(stations.at(i)->getStation()->instrument != NULL){
                stream.writeStartElement("sensor");
                stream.writeAttribute("name",stations.at(i)->getStation()->instrument->getMetaData()->name);
                stream.writeAttribute("path", stations.at(i)->getStation()->instrument->getMetaData()->pluginName);
                stream.writeEndElement();
            }

            if(stations.at(i)->getStation()->position != NULL){

                stream.writeStartElement("member");
                stream.writeAttribute("type", "position");
                stream.writeAttribute("ref", QString::number(stations.at(i)->getStation()->position->id));
                stream.writeEndElement();

                FeatureWrapper* tp = new FeatureWrapper();
                tp->setPoint(stations.at(i)->getStation()->position);

                if(!geometries.contains(tp)){
                    geometries.append(tp);
                }


            }


            if(stations.at(i)->getStation()->coordSys != NULL){
                stream.writeStartElement("member");
                stream.writeAttribute("type", "coordinatesystem");
                stream.writeAttribute("ref", QString::number(stations.at(i)->getStation()->coordSys->id));
                stream.writeEndElement();

                FeatureWrapper* tp = new FeatureWrapper();
                tp->setCoordinateSystem(stations.at(i)->getStation()->coordSys);

                if(!coordSystems.contains(tp)){
                    coordSystems.append(tp);
                }

            }

            for(int k =0;k<stations.at(i)->getStation()->functionList.size();k++){
                stream.writeStartElement("function");
                stream.writeAttribute("name", stations.at(i)->getStation()->functionList.at(k)->getMetaData()->name);
                stream.writeAttribute("id", QString::number(stations.at(i)->getStation()->functionList.at(k)->getId()));
                stream.writeAttribute("executionIndex", QString::number(k));

                QMapIterator<int, QList<InputFeature> > j(stations.at(i)->getStation()->functionList.at(k)->getFeatureOrder());
                while (j.hasNext()) {
                    j.next();

                    for(int m = 0; m < j.value().size();m++){
                        stream.writeStartElement("inputElement");
                        stream.writeAttribute("index", QString::number(j.key()));
                        int neededFeatureId = j.value().at(m).id;
                        stream.writeAttribute("ref", QString::number(neededFeatureId));
                        stream.writeEndElement();
                    }


                }
                stream.writeEndElement();
            }

            stream.writeEndElement();
        }

        //##############################save coordinatesystems##############################
        for(int i = 0; i<coordSystems.size();i++){

            stream.writeStartElement("coordinatesystem");
            stream.writeAttribute("id", QString::number(coordSystems.at(i)->getCoordinateSystem()->id));
            stream.writeAttribute("name", coordSystems.at(i)->getCoordinateSystem()->name);
            stream.writeAttribute("solved", QString::number(coordSystems.at(i)->getCoordinateSystem()->isSolved));

            for(int k =0;k<coordSystems.at(i)->getCoordinateSystem()->usedFor.size();k++){
                stream.writeStartElement("member");
                stream.writeAttribute("type", "usedForFeature");
                stream.writeAttribute("ref", QString::number(features.at(i)->getCoordinateSystem()->usedFor.at(k)->getFeature()->id));
                stream.writeEndElement();
            }

                for(int k =0;k<coordSystems.at(i)->getCoordinateSystem()->observations.size();k++){
                    stream.writeStartElement("member");
                    stream.writeAttribute("type", "observation");
                    stream.writeAttribute("ref", QString::number(coordSystems.at(i)->getCoordinateSystem()->observations.at(k)->id));
                    stream.writeEndElement();

                    if(!observations.contains(coordSystems.at(i)->getCoordinateSystem()->observations.at(k))){
                        observations.append(coordSystems.at(i)->getCoordinateSystem()->observations.at(k));
                    }
                }



                for(int k =0;k<coordSystems.at(i)->getCoordinateSystem()->nominals.size();k++){
                    stream.writeStartElement("member");
                    stream.writeAttribute("type", "nominal");
                    stream.writeAttribute("ref", QString::number(coordSystems.at(i)->getCoordinateSystem()->nominals.at(k)->id));
                    stream.writeEndElement();
                }

                for(int k =0;k<coordSystems.at(i)->getCoordinateSystem()->trafoParams.size();k++){
                    stream.writeStartElement("member");
                    stream.writeAttribute("type", "transformationParameter");
                    stream.writeAttribute("ref", QString::number(coordSystems.at(i)->getCoordinateSystem()->trafoParams.at(k)->id));

                    FeatureWrapper* tp = new FeatureWrapper();
                    tp->setTrafoParam(coordSystems.at(i)->getCoordinateSystem()->trafoParams.at(k));

                    if(!trafoParams.contains(tp)){
                        trafoParams.append(tp);
                    }

                    stream.writeEndElement();
                }

                for(int k =0;k<coordSystems.at(i)->getCoordinateSystem()->functionList.size();k++){
                    stream.writeStartElement("function");
                    stream.writeAttribute("name", coordSystems.at(i)->getCoordinateSystem()->functionList.at(k)->getMetaData()->name);
                    stream.writeAttribute("id", QString::number(coordSystems.at(i)->getCoordinateSystem()->functionList.at(k)->getId()));
                    stream.writeEndElement();
                }

                for(int k =0;k<coordSystems.at(i)->getCoordinateSystem()->functionList.size();k++){
                    stream.writeStartElement("function");
                    stream.writeAttribute("name", coordSystems.at(i)->getCoordinateSystem()->functionList.at(k)->getMetaData()->name);
                    stream.writeAttribute("path", coordSystems.at(i)->getCoordinateSystem()->functionList.at(k)->getMetaData()->pluginName);
                    stream.writeAttribute("executionIndex", QString::number(k));

                    QMapIterator<int, QList<InputFeature> > j(coordSystems.at(i)->getCoordinateSystem()->functionList.at(k)->getFeatureOrder());
                    while (j.hasNext()) {
                        j.next();

                        for(int m = 0; m < j.value().size();m++){
                            stream.writeStartElement("inputElement");
                            stream.writeAttribute("index", QString::number(j.key()));
                            int neededFeatureId = j.value().at(m).id;
                            stream.writeAttribute("ref", QString::number(neededFeatureId));
                            stream.writeEndElement();
                        }


                    }

                    stream.writeEndElement();
                }

                stream.writeEndElement();


        }

        //##############################save trafoParameter############################
        for(int i = 0; i<trafoParams.size();i++){

            stream.writeStartElement("transformationsparameter");
            stream.writeAttribute("id", QString::number(trafoParams.at(i)->getTrafoParam()->id));
            stream.writeAttribute("name", trafoParams.at(i)->getTrafoParam()->name);
            stream.writeAttribute("solved", QString::number(trafoParams.at(i)->getTrafoParam()->isSolved));
            stream.writeAttribute("tx", QString::number(trafoParams.at(i)->getTrafoParam()->translation.getAt(0)));
            stream.writeAttribute("ty", QString::number(trafoParams.at(i)->getTrafoParam()->translation.getAt(1)));
            stream.writeAttribute("tz", QString::number(trafoParams.at(i)->getTrafoParam()->translation.getAt(2)));
            stream.writeAttribute("rx", QString::number(trafoParams.at(i)->getTrafoParam()->rotation.getAt(0)));
            stream.writeAttribute("ry", QString::number(trafoParams.at(i)->getTrafoParam()->rotation.getAt(1)));
            stream.writeAttribute("rz", QString::number(trafoParams.at(i)->getTrafoParam()->rotation.getAt(2)));
            stream.writeAttribute("mx", QString::number(trafoParams.at(i)->getTrafoParam()->scale.getAt(0)));
            stream.writeAttribute("my", QString::number(trafoParams.at(i)->getTrafoParam()->scale.getAt(1)));
            stream.writeAttribute("mz", QString::number(trafoParams.at(i)->getTrafoParam()->scale.getAt(2)));

            for(int k =0;k<trafoParams.at(i)->getTrafoParam()->usedFor.size();k++){
                stream.writeStartElement("member");
                stream.writeAttribute("type", "usedForFeature");
                stream.writeAttribute("ref", QString::number(trafoParams.at(i)->getTrafoParam()->usedFor.at(k)->getFeature()->id));
                stream.writeEndElement();
            }

                for(int k =0;k<trafoParams.at(i)->getTrafoParam()->functionList.size();k++){
                    stream.writeStartElement("function");
                    stream.writeAttribute("name", trafoParams.at(i)->getTrafoParam()->functionList.at(k)->getMetaData()->name);
                    stream.writeAttribute("id", QString::number(trafoParams.at(i)->getTrafoParam()->functionList.at(k)->getId()));
                    stream.writeEndElement();
                }

                for(int k =0;k<trafoParams.at(i)->getTrafoParam()->functionList.size();k++){
                    stream.writeStartElement("function");
                    stream.writeAttribute("name", trafoParams.at(i)->getTrafoParam()->functionList.at(k)->getMetaData()->name);
                    stream.writeAttribute("path", trafoParams.at(i)->getTrafoParam()->functionList.at(k)->getMetaData()->pluginName);
                    stream.writeAttribute("executionIndex", QString::number(k));

                    QMapIterator<int, QList<InputFeature> > j(trafoParams.at(i)->getTrafoParam()->functionList.at(k)->getFeatureOrder());
                    while (j.hasNext()) {
                        j.next();

                        for(int m = 0; m < j.value().size();m++){
                            stream.writeStartElement("inputElement");
                            stream.writeAttribute("index", QString::number(j.key()));
                            int neededFeatureId = j.value().at(m).id;
                            stream.writeAttribute("ref", QString::number(neededFeatureId));
                            stream.writeEndElement();
                        }


                    }

                    stream.writeEndElement();
                }

                stream.writeEndElement();


        }

        //##############################save geometries##############################
        for(int i = 0; i<geometries.size();i++){
            stream.writeStartElement("geometry");
            stream.writeAttribute("id", QString::number(geometries.at(i)->getGeometry()->id));
            stream.writeAttribute("name", geometries.at(i)->getGeometry()->name);
            stream.writeAttribute("type", geometries.at(i)->returnFeatureType());
            stream.writeAttribute("nominal",QString::number(geometries.at(i)->getGeometry()->isNominal));
            stream.writeAttribute("common",QString::number(geometries.at(i)->getGeometry()->isCommon));
            stream.writeAttribute("solved", QString::number(geometries.at(i)->getGeometry()->isSolved));


            if(geometries.at(i)->getGeometry()->isSolved || geometries.at(i)->getGeometry()->isNominal){
                if(geometries.at(i)->getPoint() != NULL){
                    stream.writeStartElement("coordinates");
                    stream.writeAttribute("x", QString::number(geometries.at(i)->getPoint()->xyz.getAt(0)));
                    stream.writeAttribute("y", QString::number(geometries.at(i)->getPoint()->xyz.getAt(1)));
                    stream.writeAttribute("z", QString::number(geometries.at(i)->getPoint()->xyz.getAt(2)));
                    stream.writeEndElement();

                    stream.writeStartElement("standardDeviation");
                    stream.writeAttribute("value", QString::number(geometries.at(i)->getPoint()->myStatistic.stdev));
                    stream.writeEndElement();
                }else if(geometries.at(i)->getLine() != NULL){
                    stream.writeStartElement("coordinates");
                    stream.writeAttribute("x", QString::number(geometries.at(i)->getLine()->xyz.getAt(0)));
                    stream.writeAttribute("y", QString::number(geometries.at(i)->getLine()->xyz.getAt(1)));
                    stream.writeAttribute("z", QString::number(geometries.at(i)->getLine()->xyz.getAt(2)));
                    stream.writeEndElement();

                    stream.writeStartElement("spatialDirection");
                    stream.writeAttribute("i", QString::number(geometries.at(i)->getLine()->ijk.getAt(0)));
                    stream.writeAttribute("j", QString::number(geometries.at(i)->getLine()->ijk.getAt(1)));
                    stream.writeAttribute("k", QString::number(geometries.at(i)->getLine()->ijk.getAt(2)));
                    stream.writeEndElement();

                    stream.writeStartElement("standardDeviation");
                    stream.writeAttribute("value", QString::number(geometries.at(i)->getLine()->myStatistic.stdev));
                    stream.writeEndElement();
                }else if(geometries.at(i)->getPlane() != NULL){
                    stream.writeStartElement("coordinates");
                    stream.writeAttribute("x", QString::number(geometries.at(i)->getPlane()->xyz.getAt(0)));
                    stream.writeAttribute("y", QString::number(geometries.at(i)->getPlane()->xyz.getAt(1)));
                    stream.writeAttribute("z", QString::number(geometries.at(i)->getPlane()->xyz.getAt(2)));
                    stream.writeEndElement();

                    stream.writeStartElement("spatialDirection");
                    stream.writeAttribute("i", QString::number(geometries.at(i)->getPlane()->ijk.getAt(0)));
                    stream.writeAttribute("j", QString::number(geometries.at(i)->getPlane()->ijk.getAt(1)));
                    stream.writeAttribute("k", QString::number(geometries.at(i)->getPlane()->ijk.getAt(2)));
                    stream.writeEndElement();

                    stream.writeStartElement("standardDeviation");
                    stream.writeAttribute("value", QString::number(geometries.at(i)->getPlane()->myStatistic.stdev));
                    stream.writeEndElement();

                }else if(geometries.at(i)->getSphere() != NULL){

                    stream.writeStartElement("coordinates");
                    stream.writeAttribute("x", QString::number(geometries.at(i)->getSphere()->xyz.getAt(0)));
                    stream.writeAttribute("y", QString::number(geometries.at(i)->getSphere()->xyz.getAt(1)));
                    stream.writeAttribute("z", QString::number(geometries.at(i)->getSphere()->xyz.getAt(2)));
                    stream.writeEndElement();

                    stream.writeStartElement("radius");
                    stream.writeAttribute("value", QString::number(geometries.at(i)->getSphere()->radius));
                    stream.writeEndElement();

                    stream.writeStartElement("standardDeviation");
                    stream.writeAttribute("value", QString::number(geometries.at(i)->getSphere()->myStatistic.stdev));
                    stream.writeEndElement();

                }else if (geometries.at(i)->getScalarEntityAngle() != NULL){

                    stream.writeStartElement("angle");
                    stream.writeAttribute("value", QString::number(geometries.at(i)->getScalarEntityAngle()->getAngle()));
                    stream.writeEndElement();

                    stream.writeStartElement("standardDeviation");
                    stream.writeAttribute("value", QString::number(geometries.at(i)->getScalarEntityAngle()->myStatistic.stdev));
                    stream.writeEndElement();

                }else if (geometries.at(i)->getScalarEntityDistance() != NULL){

                    stream.writeStartElement("distance");
                    stream.writeAttribute("value", QString::number(geometries.at(i)->getScalarEntityDistance()->getDistance()));
                    stream.writeEndElement();

                    stream.writeStartElement("standardDeviation");
                    stream.writeAttribute("value", QString::number(geometries.at(i)->getScalarEntityDistance()->myStatistic.stdev));
                    stream.writeEndElement();

                }
            }

            for(int k =0;k<geometries.at(i)->getGeometry()->usedFor.size();k++){
                stream.writeStartElement("member");
                stream.writeAttribute("type", "usedForFeature");
                stream.writeAttribute("ref", QString::number(geometries.at(i)->getGeometry()->usedFor.at(k)->getFeature()->id));
                stream.writeEndElement();
            }

            for(int k =0;k<geometries.at(i)->getGeometry()->myObservations.size();k++){
                stream.writeStartElement("member");
                stream.writeAttribute("type", "observation");
                stream.writeAttribute("ref", QString::number(geometries.at(i)->getGeometry()->myObservations.at(k)->id));
                stream.writeEndElement();
                if(!observations.contains(geometries.at(i)->getGeometry()->myObservations.at(k))){
                    observations.append(geometries.at(i)->getGeometry()->myObservations.at(k));
                }
             }

            if(geometries.at(i)->getGeometry()->myNominalCoordSys != NULL){
                stream.writeStartElement("member");
                stream.writeAttribute("type", "coordinatesystem");
                stream.writeAttribute("ref", QString::number(geometries.at(i)->getGeometry()->myNominalCoordSys->id));
                stream.writeEndElement();
            }

            for(int k =0;k<geometries.at(i)->getGeometry()->nominals.size();k++){
                stream.writeStartElement("member");
                stream.writeAttribute("type", "nominalGeometry");
                stream.writeAttribute("ref", QString::number(geometries.at(i)->getGeometry()->nominals.at(k)->id));
                stream.writeEndElement();
            }

            if(!geometries.at(i)->getGeometry()->isNominal){
                stream.writeStartElement("measurementconfig");
                stream.writeAttribute("name", geometries.at(i)->getGeometry()->mConfig.name);
                stream.writeEndElement();
            }

            for(int k =0;k<geometries.at(i)->getGeometry()->functionList.size();k++){
                stream.writeStartElement("function");
                stream.writeAttribute("name", geometries.at(i)->getGeometry()->functionList.at(k)->getMetaData()->name);
                stream.writeAttribute("path", geometries.at(i)->getGeometry()->functionList.at(k)->getMetaData()->pluginName);
                stream.writeAttribute("executionIndex", QString::number(k));

                QMapIterator<int, QList<InputFeature> > j(geometries.at(i)->getGeometry()->functionList.at(k)->getFeatureOrder());
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

            stream.writeEndElement();
        }

        //##############################save observations##############################
        for (int i = 0; i < observations.size();i++){

            stream.writeStartElement("observation");
            stream.writeAttribute("id", QString::number(observations.at(i)->id));
            stream.writeAttribute("x", QString::number(observations.at(i)->myXyz.getAt(0)));
            stream.writeAttribute("y", QString::number(observations.at(i)->myXyz.getAt(1)));
            stream.writeAttribute("z", QString::number(observations.at(i)->myXyz.getAt(2)));
            stream.writeAttribute("isValid", QString::number(observations.at(i)->isValid));
            stream.writeAttribute("sigmaX", QString::number(observations.at(i)->sigmaXyz.getAt(0)));
            stream.writeAttribute("sigmaY", QString::number(observations.at(i)->sigmaXyz.getAt(1)));
            stream.writeAttribute("sigmaZ", QString::number(observations.at(i)->sigmaXyz.getAt(2)));


            /*if(observations.at(i)->myStation != NULL){
                stream.writeStartElement("member");
                stream.writeAttribute("type", "station");
                stream.writeAttribute("ref", QString::number(observations.at(i)->myStation->id));
                stream.writeEndElement();
            }*/

            if(observations.at(i)->myReading != NULL){
                stream.writeStartElement("reading");
                stream.writeAttribute("id",QString::number(observations.at(i)->myReading->id));
                stream.writeAttribute("type",QString::number(observations.at(i)->myReading->typeofReading));

                QString measuredAtTime = observations.at(i)->myReading->measuredAt.toString(Qt::ISODate);

                switch(observations.at(i)->myReading->typeofReading){
                    case(Configuration::ePolar) :{

                    //get current date and time


                        stream.writeStartElement("measurement");
                        stream.writeAttribute("time",measuredAtTime);
                        stream.writeAttribute("type","azimuth");
                        stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rPolar.azimuth));
                        stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rPolar.sigmaAzimuth));
                        stream.writeEndElement();

                        stream.writeStartElement("measurement");
                        stream.writeAttribute("time",measuredAtTime);
                        stream.writeAttribute("type","zenith");
                        stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rPolar.zenith));
                        stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rPolar.sigmaZenith));
                        stream.writeEndElement();

                        stream.writeStartElement("measurement");
                        stream.writeAttribute("time",measuredAtTime);
                        stream.writeAttribute("type","distance");
                        stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rPolar.distance));
                        stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rPolar.sigmaDistance));
                        stream.writeEndElement();


                        break;
                    }
                    case(Configuration::eDistance) :{

                        stream.writeStartElement("measurement");
                        stream.writeAttribute("time",measuredAtTime);
                        stream.writeAttribute("type","distance");
                        stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rDistance.distance));
                        stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rDistance.sigmaDistance));
                        stream.writeEndElement();

                        break;
                    }
                    case(Configuration::eDirection) :{

                        stream.writeStartElement("measurement");
                        stream.writeAttribute("time",measuredAtTime);
                        stream.writeAttribute("type","azimuth");
                        stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rDirection.azimuth));
                        stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rDirection.sigmaAzimuth));
                        stream.writeEndElement();

                        stream.writeStartElement("measurement");
                        stream.writeAttribute("time",measuredAtTime);
                        stream.writeAttribute("type","zenith");
                        stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rDirection.zenith));
                        stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rDirection.sigmaZenith));
                        stream.writeEndElement();

                        break;
                    }
                    case(Configuration::eCartesian) :{

                        stream.writeStartElement("measurement");
                        stream.writeAttribute("time",measuredAtTime);
                        stream.writeAttribute("type","x");
                        stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rCartesian.xyz.getAt(0)));
                        stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rCartesian.sigmaXyz.getAt(0)));
                        stream.writeEndElement();

                        stream.writeStartElement("measurement");
                        stream.writeAttribute("time",measuredAtTime);
                        stream.writeAttribute("type","y");
                        stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rCartesian.xyz.getAt(1)));
                        stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rCartesian.sigmaXyz.getAt(1)));
                        stream.writeEndElement();

                        stream.writeStartElement("measurement");
                        stream.writeAttribute("time",measuredAtTime);
                        stream.writeAttribute("type","z");
                        stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rCartesian.xyz.getAt(2)));
                        stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rCartesian.sigmaXyz.getAt(2)));
                        stream.writeEndElement();


                        break;
                    }
                    case(Configuration::eLevel) :{

                        stream.writeStartElement("measurement");
                        stream.writeAttribute("time",measuredAtTime);
                        stream.writeAttribute("type","angleXZ");
                        stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rLevel.angleXZ));
                        stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rLevel.sigmaAngleXZ));
                        stream.writeEndElement();

                        stream.writeStartElement("measurement");
                        stream.writeAttribute("time",measuredAtTime);
                        stream.writeAttribute("type","angleYZ");
                        stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rLevel.angleYZ));
                        stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rLevel.sigmaAngleYZ));
                        stream.writeEndElement();

                        stream.writeStartElement("measurement");
                        stream.writeAttribute("time",measuredAtTime);
                        stream.writeAttribute("type","diffXY");
                        stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rLevel.diffXY));
                        stream.writeAttribute("sigma","-");
                        stream.writeEndElement();

                        stream.writeStartElement("measurement");
                        stream.writeAttribute("time",measuredAtTime);
                        stream.writeAttribute("type","diffXZ");
                        stream.writeAttribute("value",QString::number(observations.at(i)->myReading->rLevel.diffXZ));
                        stream.writeAttribute("sigma","-");
                        stream.writeEndElement();


                        break;
                }
                case(Configuration::eUndefined) :{

                    QMapIterator<QString, double > j(observations.at(i)->myReading->rUndefined.values);
                    while (j.hasNext()) {
                        j.next();

                        stream.writeStartElement("measurement");
                        stream.writeAttribute("time",measuredAtTime);
                        stream.writeAttribute("type",j.key());
                        stream.writeAttribute("value",QString::number(j.value()));
                        stream.writeAttribute("sigma",QString::number(observations.at(i)->myReading->rUndefined.sigmaValues.value(j.key())));
                        stream.writeEndElement();

                    }

                        break;
                }
                }

                stream.writeEndElement();
            }



            /*for(int k =0;k<observations.at(i)->myTargetGeometries.size();k++){
                stream.writeStartElement("member");
                stream.writeAttribute("type", "geometry");
                stream.writeAttribute("ref", QString::number(observations.at(i)->myTargetGeometries.at(k)->id));
                stream.writeEndElement();
            }*/


            stream.writeEndElement();

        }

        stream.writeEndElement(); // bookmark
        stream.writeEndDocument();
      }else{
       return false;
     }

     return true;
 }

bool OiDataImporter::loadFromXML(QList<FeatureWrapper*> features, QFile *fileToLoad){

    if (!fileToLoad->open(QIODevice::ReadOnly | QIODevice::Text)) {

        return false;
    }

    QList<Observation*> observations;
    QList<FeatureWrapper*> stations;
    QList<FeatureWrapper*> coordSystems;
    QList<FeatureWrapper*> trafoParams;

    QXmlStreamReader xml(fileToLoad);

    while(!xml.atEnd() &&
            !xml.hasError()) {

        QXmlStreamReader::TokenType token = xml.readNext();

        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if(token == QXmlStreamReader::StartElement) {

            qDebug() << xml.name();

            if(xml.name() == "observation") {
                Observation *o = OiDataImporter::parseObservation(xml);
                observations.append(o);
            }

            if(xml.name() == "oiProjectData"){

            }

            if(xml.name() == "station"){

            }

            if(xml.name() == "coordinatesystem"){

            }

            if(xml.name() == "geometry"){

            }

            if(xml.name() == "transformationparameter"){

            }
        }
    }

    if(xml.hasError()) {

        Console::addLine(QString("xml not valid: " + xml.errorString()));
        return false;
    }

     return true;
 }

Observation* OiDataImporter::parseObservation(QXmlStreamReader& xml) {

    Observation* obs = new Observation(NULL,NULL);


    QXmlStreamAttributes attributes = xml.attributes();

    if(attributes.hasAttribute("id")) {
        obs->id = attributes.value("id").toInt();
    }
    if(attributes.hasAttribute("x")){
        obs->myOriginalXyz.setAt(0,attributes.value("x").toDouble());
    }
    if(attributes.hasAttribute("y")){
        obs->myOriginalXyz.setAt(1,attributes.value("y").toDouble());
    }
    if(attributes.hasAttribute("z")){
        obs->myOriginalXyz.setAt(2,attributes.value("z").toDouble());
    }
    if(attributes.hasAttribute("isValid")){
        obs->isValid = attributes.value("isVaild").toInt();
    }
    if(attributes.hasAttribute("sigmaX")){
        obs->sigmaXyz.setAt(0,attributes.value("sigmaX").toDouble());
    }
    if(attributes.hasAttribute("sigmaY")){
        obs->sigmaXyz.setAt(1,attributes.value("sigmaY").toDouble());
    }
    if(attributes.hasAttribute("sigmaZ")){
        obs->sigmaXyz.setAt(2,attributes.value("sigmaZ").toDouble());
    }
    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named person.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "observation")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found first name. */
            if(xml.name() == "reading") {
                Reading *r = OiDataImporter::parseReading(xml);
                r->obs = obs;
                obs->myReading = r;
            }

        }
        /* ...and next... */
        xml.readNext();
    }
    return obs;
}

Reading* OiDataImporter::parseReading(QXmlStreamReader& xml){

    Reading* r = new Reading();


    QXmlStreamAttributes attributes = xml.attributes();

    if(attributes.hasAttribute("id")) {
        r->id = attributes.value("id").toInt();
    }
    if(attributes.hasAttribute("type")){
       r->typeofReading = (Configuration::ReadingTypes) attributes.value("type").toInt();
    }
    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named person.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "reading")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found first name. */

            switch(r->typeofReading){
                case(Configuration::ePolar) :{

                if(xml.name() == "measurement") {
                    QXmlStreamAttributes attributes = xml.attributes();

                            if(attributes.hasAttribute("time")) {
                                r->measuredAt = QDateTime::fromString(attributes.value("time").toString());
                            }
                            if(attributes.hasAttribute("type")){

                                if(attributes.value("type").toString() == "azimuth"){

                                    if(attributes.hasAttribute("value")){
                                        r->rPolar.azimuth = attributes.value("value").toDouble();
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                        r->rPolar.sigmaAzimuth = attributes.value("sigma").toDouble();
                                    }

                                }
                                if(attributes.value("type").toString() == "zenith"){

                                    if(attributes.hasAttribute("value")){
                                        r->rPolar.zenith = attributes.value("value").toDouble();
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                        r->rPolar.sigmaZenith = attributes.value("sigma").toDouble();
                                    }

                                }
                                if(attributes.value("type").toString() == "distance"){

                                    if(attributes.hasAttribute("value")){
                                        r->rPolar.distance = attributes.value("value").toDouble();
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                        r->rPolar.sigmaDistance = attributes.value("sigma").toDouble();
                                    }

                                }

                            }
                        }

                    break;
                }
                case(Configuration::eDistance) :{


                if(xml.name() == "measurement") {
                    QXmlStreamAttributes attributes = xml.attributes();

                            if(attributes.hasAttribute("time")) {
                                r->measuredAt = QDateTime::fromString(attributes.value("time").toString());
                            }
                            if(attributes.hasAttribute("type")){

                                if(attributes.value("type").toString() == "distance"){

                                    if(attributes.hasAttribute("value")){
                                        r->rDistance.distance = attributes.value("value").toDouble();
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                        r->rDistance.sigmaDistance = attributes.value("sigma").toDouble();
                                    }
                                }
                            }
                     }


                    break;
                }
                case(Configuration::eDirection) :{

                if(xml.name() == "measurement") {
                    QXmlStreamAttributes attributes = xml.attributes();

                            if(attributes.hasAttribute("time")) {
                                r->measuredAt = QDateTime::fromString(attributes.value("time").toString());
                            }
                            if(attributes.hasAttribute("type")){

                                if(attributes.value("type").toString() == "azimuth"){

                                    if(attributes.hasAttribute("value")){
                                        r->rDirection.azimuth = attributes.value("value").toDouble();
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                        r->rDirection.sigmaAzimuth = attributes.value("sigma").toDouble();
                                    }

                                }
                                if(attributes.value("type").toString() == "zenith"){

                                    if(attributes.hasAttribute("value")){
                                        r->rDirection.zenith = attributes.value("value").toDouble();
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                        r->rDirection.sigmaZenith = attributes.value("sigma").toDouble();
                                    }

                                }

                            }
                        }

                    break;
                }
                case(Configuration::eCartesian) :{

                if(xml.name() == "measurement") {
                    QXmlStreamAttributes attributes = xml.attributes();

                            if(attributes.hasAttribute("time")) {
                                r->measuredAt = QDateTime::fromString(attributes.value("time").toString());
                            }
                            if(attributes.hasAttribute("type")){

                                if(attributes.value("type").toString() == "x"){

                                    if(attributes.hasAttribute("value")){
                                        r->rCartesian.xyz.setAt(0,attributes.value("value").toDouble());
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                       r->rCartesian.sigmaXyz.setAt(0,attributes.value("sigma").toDouble());
                                    }

                                }
                                if(attributes.value("type").toString() == "y"){

                                    if(attributes.hasAttribute("value")){
                                        r->rCartesian.xyz.setAt(1,attributes.value("value").toDouble());
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                       r->rCartesian.sigmaXyz.setAt(1,attributes.value("sigma").toDouble());
                                    }

                                }
                                if(attributes.value("type").toString() == "z"){

                                    if(attributes.hasAttribute("value")){
                                        r->rCartesian.xyz.setAt(1,attributes.value("value").toDouble());
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                       r->rCartesian.sigmaXyz.setAt(1,attributes.value("sigma").toDouble());
                                    }

                                }

                            }
                        }

                    break;
                }
                case(Configuration::eLevel) :{

                if(xml.name() == "measurement") {
                    QXmlStreamAttributes attributes = xml.attributes();

                            if(attributes.hasAttribute("time")) {
                               r->measuredAt = QDateTime::fromString(attributes.value("time").toString());
                            }
                            if(attributes.hasAttribute("type")){

                                if(attributes.value("type").toString() == "angleXZ"){

                                    if(attributes.hasAttribute("value")){
                                        r->rLevel.angleXZ = attributes.value("value").toDouble();
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                        r->rLevel.sigmaAngleXZ = attributes.value("sigma").toDouble();
                                    }

                                }
                                if(attributes.value("type").toString() == "angleYZ"){

                                    if(attributes.hasAttribute("value")){
                                        r->rLevel.angleYZ = attributes.value("value").toDouble();
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                        r->rLevel.sigmaAngleYZ = attributes.value("sigma").toDouble();
                                    }

                                }
                                if(attributes.value("type").toString() == "diffXY"){

                                    if(attributes.hasAttribute("value")){
                                        r->rLevel.diffXY = attributes.value("value").toDouble();
                                    }
                                    if(attributes.hasAttribute("sigma")){

                                    }

                                }
                                if(attributes.value("type").toString() == "diffXZ"){

                                    if(attributes.hasAttribute("value")){
                                        r->rLevel.diffXZ = attributes.value("value").toDouble();
                                    }
                                    if(attributes.hasAttribute("sigma")){

                                    }

                                }

                            }
                        }


                    break;
            }
            case(Configuration::eUndefined) :{

                if(xml.name() == "measurement") {
                    QXmlStreamAttributes attributes = xml.attributes();

                            if(attributes.hasAttribute("time")) {

                                r->measuredAt = QDateTime::fromString(attributes.value("time").toString());
                            }
                            if(attributes.hasAttribute("type")){

                                    if(attributes.hasAttribute("value")){
                                        r->rUndefined.values.insert(attributes.value("type").toString(),attributes.value("value").toDouble());
                                    }
                                    if(attributes.hasAttribute("sigma")){
                                        r->rUndefined.sigmaValues.insert(attributes.value("type").toString(),attributes.value("sigma").toDouble());
                                    }

                             }
                     }

                    break;
            }
            }


        }
        /* ...and next... */
        xml.readNext();
    }

    if(xml.hasError()) {

        Console::addLine(QString("xml not valid: " + xml.errorString()));
        return r;
    }

    return r;

}

FeatureWrapper* OiDataImporter::parseStation(QXmlStreamReader& xml){

    FeatureWrapper* fw = new FeatureWrapper();
    Station *s = NULL;
    fw->setStation(s);

    QXmlStreamAttributes attributes = xml.attributes();

    if(attributes.hasAttribute("name")){
        s = new Station(attributes.value("name").toString());
    }
    if(attributes.hasAttribute("id")) {
        s->id = attributes.value("id").toInt();
    }

    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named person.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "station")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found first name. */
            if(xml.name() == "sensor") {

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "sensor")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QString sensorName;
                        QString sensorPlugin;

                        QXmlStreamAttributes sensorAttributes = xml.attributes();

                        if(sensorAttributes.hasAttribute("name")){
                            sensorName = sensorAttributes.value("name").toString();
                        }
                        if(attributes.hasAttribute("path")) {
                            sensorPlugin = sensorAttributes.value("path").toString();
                        }

                        //TODO *sensor = database methode (QString name,QString plugin)

                    }
                    /* ...and next... */
                    xml.readNext();
                }
            }

            if(xml.name() == "member"){

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "member")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes memberAttributes = xml.attributes();

                        if(memberAttributes.hasAttribute("type")){

                            if (memberAttributes.value("type") == "position"){

                                if(memberAttributes.hasAttribute("ref")){
                                    s->position->id = memberAttributes.value("ref").toInt();
                                }

                            }

                            if (memberAttributes.value("type") == "coordinatesystem"){

                                if(memberAttributes.hasAttribute("ref")){
                                    s->coordSys->id = memberAttributes.value("ref").toInt();
                                }
                            }
                        }

                    }
                    /* ...and next... */
                    xml.readNext();
                }

            }

            if(xml.name() == "function"){

                OiDataImporter::parseFunction(xml,fw);

            }


        }
        /* ...and next... */
        xml.readNext();
    }
    return fw;
}


FeatureWrapper* OiDataImporter::parseGeometry(QXmlStreamReader& xml){

    FeatureWrapper* fw = new FeatureWrapper();

    QXmlStreamAttributes attributes = xml.attributes();

    if(attributes.hasAttribute("type")){
        if(attributes.value("type").toString() == Configuration::sPlane){
            Plane *pl = new Plane();

            if(attributes.hasAttribute("name")){
                pl->name = attributes.value("name").toString();
            }
            if(attributes.hasAttribute("id")) {
                pl->id = attributes.value("id").toInt();
            }
            if(attributes.hasAttribute("nominal")) {
                pl->isNominal = attributes.value("nominal").toInt();
            }
            if(attributes.hasAttribute("common")) {
                pl->isCommon = attributes.value("common").toInt();
            }
            if(attributes.hasAttribute("solved")) {
                pl->isSolved= attributes.value("solved").toInt();
            }

            fw->setPlane(pl);
        }else if(attributes.value("type").toString() == Configuration::sPoint){
            Point *p = new Point();

            if(attributes.hasAttribute("name")){
                p->name = attributes.value("name").toString();
            }
            if(attributes.hasAttribute("id")) {
                p->id = attributes.value("id").toInt();
            }
            if(attributes.hasAttribute("nominal")) {
                p->isNominal = attributes.value("nominal").toInt();
            }
            if(attributes.hasAttribute("common")) {
                p->isCommon = attributes.value("common").toInt();
            }
            if(attributes.hasAttribute("solved")) {
                p->isSolved= attributes.value("solved").toInt();
            }

            fw->setPoint(p);

        }else if(attributes.value("type").toString() == Configuration::sLine){

            Line *l = new Line();

            if(attributes.hasAttribute("name")){
                l->name = attributes.value("name").toString();
            }
            if(attributes.hasAttribute("id")) {
                l->id = attributes.value("id").toInt();
            }
            if(attributes.hasAttribute("nominal")) {
                l->isNominal = attributes.value("nominal").toInt();
            }
            if(attributes.hasAttribute("common")) {
                l->isCommon = attributes.value("common").toInt();
            }
            if(attributes.hasAttribute("solved")) {
                l->isSolved= attributes.value("solved").toInt();
            }

            fw->setLine(l);

        }else if(attributes.value("type").toString() == Configuration::sSphere){

            Sphere *s = new Sphere();

            if(attributes.hasAttribute("name")){
                s->name = attributes.value("name").toString();
            }
            if(attributes.hasAttribute("id")) {
                s->id = attributes.value("id").toInt();
            }
            if(attributes.hasAttribute("nominal")) {
                s->isNominal = attributes.value("nominal").toInt();
            }
            if(attributes.hasAttribute("common")) {
                s->isCommon = attributes.value("common").toInt();
            }
            if(attributes.hasAttribute("solved")) {
                s->isSolved= attributes.value("solved").toInt();
            }

            fw->setSphere(s);

        }else if(attributes.value("type").toString() == Configuration::sEntitiyAngle){

            ScalarEntityAngle *sa = new ScalarEntityAngle();

            if(attributes.hasAttribute("name")){
                sa->name = attributes.value("name").toString();
            }
            if(attributes.hasAttribute("id")) {
                sa->id = attributes.value("id").toInt();
            }
            if(attributes.hasAttribute("nominal")) {
                sa->isNominal = attributes.value("nominal").toInt();
            }
            if(attributes.hasAttribute("common")) {
                sa->isCommon = attributes.value("common").toInt();
            }
            if(attributes.hasAttribute("solved")) {
                sa->isSolved= attributes.value("solved").toInt();
            }

            fw->setScalarEntityAngle(sa);

        }else if(attributes.value("type").toString() == Configuration::sEntityDistance){

            ScalarEntityDistance *sd = new ScalarEntityDistance();

            if(attributes.hasAttribute("name")){
                sd->name = attributes.value("name").toString();
            }
            if(attributes.hasAttribute("id")) {
                sd->id = attributes.value("id").toInt();
            }
            if(attributes.hasAttribute("nominal")) {
                sd->isNominal = attributes.value("nominal").toInt();
            }
            if(attributes.hasAttribute("common")) {
                sd->isCommon = attributes.value("common").toInt();
            }
            if(attributes.hasAttribute("solved")) {
                sd->isSolved= attributes.value("solved").toInt();
            }

            fw->setScalarEntityDistance(sd);

        }

    }


    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named person.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "geometry")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found first name. */
            if(xml.name() == "coordinates") {

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "coordinates")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes coordinatesAttributes = xml.attributes();

                        if(fw->getPoint() != NULL){

                            if(coordinatesAttributes.hasAttribute("x")){
                               fw->getPoint()->xyz.setAt(0,coordinatesAttributes.value("x").toDouble());
                            }

                            if(coordinatesAttributes.hasAttribute("y")){
                               fw->getPoint()->xyz.setAt(1,coordinatesAttributes.value("y").toDouble());
                            }

                            if(coordinatesAttributes.hasAttribute("z")){
                               fw->getPoint()->xyz.setAt(2,coordinatesAttributes.value("z").toDouble());
                            }

                        }else if(fw->getPlane() != NULL){

                            if(coordinatesAttributes.hasAttribute("x")){
                               fw->getPlane()->xyz.setAt(0,coordinatesAttributes.value("x").toDouble());
                            }

                            if(coordinatesAttributes.hasAttribute("y")){
                               fw->getPlane()->xyz.setAt(1,coordinatesAttributes.value("y").toDouble());
                            }

                            if(coordinatesAttributes.hasAttribute("z")){
                               fw->getPlane()->xyz.setAt(2,coordinatesAttributes.value("z").toDouble());
                            }

                        }else if(fw->getLine() != NULL){

                            if(coordinatesAttributes.hasAttribute("x")){
                               fw->getLine()->xyz.setAt(0,coordinatesAttributes.value("x").toDouble());
                            }

                            if(coordinatesAttributes.hasAttribute("y")){
                               fw->getLine()->xyz.setAt(1,coordinatesAttributes.value("y").toDouble());
                            }

                            if(coordinatesAttributes.hasAttribute("z")){
                               fw->getLine()->xyz.setAt(2,coordinatesAttributes.value("z").toDouble());
                            }

                        }else if(fw->getSphere() != NULL){

                            if(coordinatesAttributes.hasAttribute("x")){
                               fw->getSphere()->xyz.setAt(0,coordinatesAttributes.value("x").toDouble());
                            }

                            if(coordinatesAttributes.hasAttribute("y")){
                               fw->getSphere()->xyz.setAt(1,coordinatesAttributes.value("y").toDouble());
                            }

                            if(coordinatesAttributes.hasAttribute("z")){
                               fw->getSphere()->xyz.setAt(2,coordinatesAttributes.value("z").toDouble());
                            }

                        }


                    }
                    /* ...and next... */
                    xml.readNext();
                }
            }

            if(xml.name() == "spatialDirection") {

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "spatialDirection")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes spatialDirectionAttributes = xml.attributes();

                       if(fw->getPlane() != NULL){

                            if(spatialDirectionAttributes.hasAttribute("i")){
                               fw->getPlane()->ijk.setAt(0,spatialDirectionAttributes.value("i").toDouble());
                            }

                            if(spatialDirectionAttributes.hasAttribute("j")){
                               fw->getPlane()->ijk.setAt(1,spatialDirectionAttributes.value("j").toDouble());
                            }

                            if(spatialDirectionAttributes.hasAttribute("k")){
                               fw->getPlane()->ijk.setAt(2,spatialDirectionAttributes.value("k").toDouble());
                            }

                        }else if(fw->getLine() != NULL){

                            if(spatialDirectionAttributes.hasAttribute("i")){
                               fw->getLine()->ijk.setAt(0,spatialDirectionAttributes.value("i").toDouble());
                            }

                            if(spatialDirectionAttributes.hasAttribute("j")){
                               fw->getLine()->ijk.setAt(1,spatialDirectionAttributes.value("j").toDouble());
                            }

                            if(spatialDirectionAttributes.hasAttribute("k")){
                               fw->getLine()->ijk.setAt(2,spatialDirectionAttributes.value("k").toDouble());
                            }

                         }
                    }
                    xml.readNext();
                }
            }

            if(xml.name() == "radius") {

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "radius")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes radiusAttributes = xml.attributes();

                       if(fw->getSphere() != NULL){

                            if(radiusAttributes.hasAttribute("value")){
                               fw->getSphere()->radius = radiusAttributes.value("value").toDouble();
                            }

                         }
                     }
                 xml.readNext();
                }
            }

            if(xml.name() == "standardDeviation") {

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "standardDeviation")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes sigmaAttributes = xml.attributes();

                       if(fw->getGeometry() != NULL){

                            if(sigmaAttributes.hasAttribute("value")){
                               fw->getGeometry()->myStatistic.stdev = sigmaAttributes.value("value").toDouble();
                            }

                         }
                     }
                xml.readNext();
                }
            }

            if(xml.name() == "angle") {

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "angle")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes angleAttributes = xml.attributes();

                       if(fw->getScalarEntityAngle() != NULL){

                            if(angleAttributes.hasAttribute("value")){
                               fw->getScalarEntityAngle()->setAngle(angleAttributes.value("value").toDouble());
                            }

                         }
                     }
                xml.readNext();
                }
            }

            if(xml.name() == "distance") {

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "distance")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes distanceAttributes = xml.attributes();

                       if(fw->getScalarEntityAngle() != NULL){

                            if(distanceAttributes.hasAttribute("value")){
                               fw->getScalarEntityDistance()->setDistance(distanceAttributes.value("value").toDouble());
                            }

                         }
                     }
                xml.readNext();
                }
            }

            if(xml.name() == "measurementconfig") {

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "measurementconfig")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes measurementconfigAttributes = xml.attributes();

                       if(!fw->getGeometry()->isNominal){

                            if(measurementconfigAttributes.hasAttribute("name")){
                              //TODO MeasurementConfig setzten - datenbank abfrage
                            }

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

                            if (memberAttributes.value("type") == "observation"){

                                if(memberAttributes.hasAttribute("ref")){
                                    Observation *tmpObs = new Observation(NULL,NULL);
                                    tmpObs->id = memberAttributes.value("ref").toInt();
                                    fw->getGeometry()->myObservations.append(tmpObs);
                                }

                            }

                            if (memberAttributes.value("type") == "coordinatesystem"){

                                if(memberAttributes.hasAttribute("ref")){
                                    CoordinateSystem* tmpCoordSys = new CoordinateSystem();
                                    tmpCoordSys->id = memberAttributes.value("ref").toInt();
                                    fw->getGeometry()->myNominalCoordSys = tmpCoordSys;
                                }
                            }

                            if (memberAttributes.value("type") == "usedForFeature"){

                                if(memberAttributes.hasAttribute("ref")){
                                    FeatureWrapper *tmpFeature = new FeatureWrapper();
                                    Point *p = new Point();
                                    p->id = memberAttributes.value("ref").toInt();
                                    tmpFeature->setPoint(p);
                                    fw->getGeometry()->usedFor.append(tmpFeature);
                                }
                            }

                            if (memberAttributes.value("type") == "nominalGeometry"){

                                if(memberAttributes.hasAttribute("ref")){
                                    Geometry *tmpGeom= new Point();
                                    tmpGeom->id = memberAttributes.value("ref").toInt();
                                    fw->getGeometry()->nominals.append(tmpGeom);
                                }
                            }

                        }

                    }
                    /* ...and next... */
                    xml.readNext();
                }
            }

            if(xml.name() == "function"){

                OiDataImporter::parseFunction(xml,fw);

            }


        }
        /* ...and next... */
        xml.readNext();
    }

    return fw;

}

FeatureWrapper* OiDataImporter::parseCoordinatesystem(QXmlStreamReader& xml){

    FeatureWrapper* fw = new FeatureWrapper();
    Station *s;
    fw->setStation(s);

    QXmlStreamAttributes attributes = xml.attributes();

    if(attributes.hasAttribute("name")){
        s = new Station(attributes.value("name").toString());
    }
    if(attributes.hasAttribute("id")) {
        s->id = attributes.value("id").toInt();
    }

    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named person.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "station")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found first name. */
            if(xml.name() == "sensor") {

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "sensor")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {
                        /* We've found first name. */


                    }
                    /* ...and next... */
                    xml.readNext();
                }
            }

            if(xml.name() == "member"){

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "member")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes memberAttributes = xml.attributes();

                        if(memberAttributes.hasAttribute("type")){

                            if (memberAttributes.value("type") == "position"){

                                if(memberAttributes.hasAttribute("ref")){

                                }

                            }

                            if (memberAttributes.value("type") == "coordinatesystem"){

                                if(memberAttributes.hasAttribute("ref")){

                                }
                            }
                        }



                    }
                    /* ...and next... */
                    xml.readNext();
                }

            }

            if(xml.name() == "function"){

                OiDataImporter::parseFunction(xml,fw);

            }


        }
        /* ...and next... */
        xml.readNext();
    }
    return fw;

}

FeatureWrapper* OiDataImporter::parseTrafoPara(QXmlStreamReader& xml){

    FeatureWrapper* fw = new FeatureWrapper();
    Station *s;
    fw->setStation(s);

    QXmlStreamAttributes attributes = xml.attributes();

    if(attributes.hasAttribute("name")){
        s = new Station(attributes.value("name").toString());
    }
    if(attributes.hasAttribute("id")) {
        s->id = attributes.value("id").toInt();
    }

    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named person.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "station")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found first name. */
            if(xml.name() == "sensor") {

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "sensor")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {
                        /* We've found first name. */


                    }
                    /* ...and next... */
                    xml.readNext();
                }
            }

            if(xml.name() == "member"){

                while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
                        xml.name() == "member")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {

                        QXmlStreamAttributes memberAttributes = xml.attributes();

                        if(memberAttributes.hasAttribute("type")){

                            if (memberAttributes.value("type") == "position"){

                                if(memberAttributes.hasAttribute("ref")){

                                }

                            }

                            if (memberAttributes.value("type") == "coordinatesystem"){

                                if(memberAttributes.hasAttribute("ref")){

                                }
                            }
                        }



                    }
                    /* ...and next... */
                    xml.readNext();
                }

            }

            if(xml.name() == "function"){

                OiDataImporter::parseFunction(xml,fw);

            }


        }
        /* ...and next... */
        xml.readNext();
    }
    return fw;

}

void OiDataImporter::parseFunction(QXmlStreamReader& xml, FeatureWrapper *feature){



}
