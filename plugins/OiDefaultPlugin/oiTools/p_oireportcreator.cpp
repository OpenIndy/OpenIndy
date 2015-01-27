#include "p_oireportcreator.h"

/*!
 * \brief BestFitLine::getMetaData
 * \return
 */
PluginMetaData* OiReportCreator::getMetaData() const{
    PluginMetaData* metaData = new PluginMetaData();
    metaData->name = "Report Creator";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "br";
    metaData->description = QString("%1")
            .arg("Tool for creating Reports");
    metaData->iid = "de.openIndy.Plugin.OiTool.v001";
    return metaData;
}

void OiReportCreator::customXMLRequest(OiRequestResponse *request)
{

}

void OiReportCreator::watchWindowKeyPressed(Qt::Key)
{

}

