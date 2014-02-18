#include "oiprojectdata.h"

oiProjectData::oiProjectData(QObject *parent) :
    QObject(parent)
{


    device = NULL;
    activeCoordSystem = NULL;
    activeStation = NULL;

}
