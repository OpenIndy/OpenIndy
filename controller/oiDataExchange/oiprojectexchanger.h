#ifndef OIPROJECTEXCHANGER_H
#define OIPROJECTEXCHANGER_H

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QDateTime>
#include <QtAlgorithms>

#include "function.h"
#include "systemdbmanager.h"
#include "pluginloader.h"

#include "projectrestorer.h"


#include "oiprojectdata.h"
#include "console.h"
#include "configuration.h"
#include "oirequesthandler.h"

#include <QtXml>

/*!
 * \brief The OiProjectExchanger class
 * This class provides methods to save and load OpenIndy projects and different configs
 */
class OiProjectExchanger
{
    friend class OiRequestHandler;
private:
    static QDomDocument saveProject();
    static bool loadProject(QDomDocument project);

    static void addActiveStationAndSystem(QDomElement &parent);

};

#endif // OIPROJECTEXCHANGER_H
