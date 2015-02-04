#ifndef PI_OIEXCHANGEINTERFACE_H
#define PI_OIEXCHANGEINTERFACE_H

#include <QStringList>
#include <QObject>

#include "oiexchangeemitter.h"
#include "oiexchangeobject.h"
#include "pluginmetadata.h"

/*!
 * \brief The oiExchange Interface must be implemented for a new kind of data import/export.
 * You define which and how features can be imported/exported by external data. After you have
 * implemented your import/export class you have to add it to the static class oidataexchanger.
 * For an example you can take a look at the oiexchangeascii class.
 */
class OiExchangeInterface : public QObject
{
    Q_OBJECT

public:
    explicit OiExchangeInterface(QObject *parent = 0) : QObject(parent){ this->nominalCoordSys = NULL; }
    virtual ~OiExchangeInterface(){}

    /*!
     * \brief getMetaData
     * \return
     */
    virtual PluginMetaData* getMetaData() const = 0;

    /*!
     * \brief importOiData
     * import data here by filling a projectData exchange object.
     * \param projectData
     * \return
     */
    virtual bool importOiData(OiExchangeObject &projectData) = 0;

    /*!
     * \brief exportOiData
     *  export data here by filling a projectData exchange object.
     * \param projectData
     * \return
     */
    virtual bool exportOiData(OiExchangeObject &projectData) = 0;


    /*!
     * \brief getSupportedGeometries
     * define here which geometry types can be imported or exported by your class
     * \return
     */
    virtual QList<Configuration::GeometryTypes> getSupportedGeometries() = 0;

    /*!
     * \brief setExportObservations
     * true if only the observations of the selected geometries shall be exported
     * \param exportObservations
     */
    void setExportObservations(bool exportObservations){
        this->exportObservations = exportObservations;
    }

    /*!
     * \brief setUnits
     * Set the units used for im- or export
     * \param units
     */
    /*void setUnits(QMap<OiUnitConverter::DimensionType, OiUnitConverter::UnitType> units){
        this->units = units;
    }*/

    /*!
     * \brief setNominalSystem
     * \param nominalCoordSys
     */
    void setNominalSystem(CoordinateSystem* nominalCoordSys){
        this->nominalCoordSys = nominalCoordSys;
    }

    /*!
     * \brief setGroup
     * \param groupName
     */
    void setGroup(QString groupName){
        this->groupName = groupName;
    }

    /*!
     * \brief addGeometry
     * Add an imported geometry to the given reference nominal system
     * \param geom
     * \param projectData
     * \return
     */
    bool addGeometry(FeatureWrapper *geom, OiExchangeObject &projectData){
        if(this->nominalCoordSys == NULL || geom == NULL || geom->getGeometry() == NULL){
            return false;
        }
        geom->getGeometry()->setNominalSystem(this->nominalCoordSys);
        projectData.features.append(geom);
        return true;
    }

    /*!
     * \brief writeToConsole
     * Writes a message to console
     * \param msg
     */
    void writeToConsole(QString msg) const{
        this->myExchangeEmitter.emitSendString(msg);
    }

signals:

    /*!
     * \brief updateProgress
     * Send signal frequently to inform OpenIndy about the status of the import/export (0 <= progress <= 100)
     * \param progress
     * \param msg
     */
    void updateProgress(int progress, QString msg);

protected:
    bool exportObservations; //true if only the observations of the selected geometries shall be exported
    //QMap<OiUnitConverter::DimensionType, OiUnitConverter::UnitType> units; //units used for im- or export
    CoordinateSystem* nominalCoordSys; //nominal coordinate system the imported geometries are referenced to
    QString groupName; //group that the imported nominals shall be added to

private:
    OiExchangeEmitter myExchangeEmitter;

};

#define OiExchange_iidd "de.openIndy.Plugin.OiExchange.v001"

#endif // PI_OIEXCHANGEINTERFACE_H

