#include "p_oiexchangepts.h"

/*!
 * \brief OiExchangePts::getMetaData
 * \return
 */
PluginMetaData *OiExchangePts::getMetaData() const{

    PluginMetaData* metaData = new PluginMetaData();

    metaData->name = "OiExchangePts";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "br";
    metaData->description = QString("%1")
            .arg("Read *.pts files.");
    metaData->iid = "de.openIndy.Plugin.OiExchange.OiExchangeDefinedFormat.v001";

    return metaData;

}

/*!
 * \brief OiExchangePts::importOiData
 * \param projectData
 * \return
 */
bool OiExchangePts::importOiData(OiExchangeObject &projectData){

    return true;

}

/*!
 * \brief OiExchangePts::exportOiData
 * \param projectData
 * \return
 */
bool OiExchangePts::exportOiData(OiExchangeObject &projectData){

    return true;

}

/*!
 * \brief OiExchangePts::getSupportedGeometries
 * \return
 */
QList<Configuration::GeometryTypes> OiExchangePts::getSupportedGeometries(){

    QList<Configuration::GeometryTypes> supportedGeometries;

    //add supported geometries
    supportedGeometries.append(Configuration::ePointCloudGeometry);

    return supportedGeometries;

}

/*!
 * \brief OiExchangePts::getSupportedFileExtensions
 * \return
 */
QStringList OiExchangePts::getSupportedFileExtensions() const{

    QStringList supportedFileExtensions;

    //add supported file extensions
    supportedFileExtensions.append("*.pts");

    return supportedFileExtensions;

}
