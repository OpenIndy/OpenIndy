#ifndef OIDATAIMPORTER_H
#define OIDATAIMPORTER_H

#include <QString>
#include <QStringList>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include"function.h"

#include "point.h"
#include "featurewrapper.h"

class OiDataImporter
{
public:
    OiDataImporter();


    static bool saveToXML(QList<FeatureWrapper*> features, QFile &fileToSave,int idActiveCoord);
    static bool loadFromXML(QList<FeatureWrapper*> features, QFile *fileToLoad);
    static Observation* parseObservation(QXmlStreamReader& xml);
    static Reading* parseReading(QXmlStreamReader& xml);
    static FeatureWrapper* parseStation(QXmlStreamReader& xml);
    static FeatureWrapper* parseGeometry(QXmlStreamReader& xml);
    static FeatureWrapper* parseCoordinatesystem(QXmlStreamReader& xml);
    static FeatureWrapper* parseTrafoPara(QXmlStreamReader& xml);
    static void parseFunction(QXmlStreamReader& xml, FeatureWrapper *feature);

};

#endif // OIDATAIMPORTER_H
