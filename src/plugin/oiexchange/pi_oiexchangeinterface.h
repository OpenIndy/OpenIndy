#ifndef PI_OIEXCHANGEINTERFACE_H
#define PI_OIEXCHANGEINTERFACE_H

#include <QStringList>
#include <QObject>

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
    explicit OiExchangeInterface(QObject *parent = 0) : QObject(parent){}
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

signals:

    /*!
     * \brief updateProgress
     * Send signal frequently to inform OpenIndy about the status of the import/export (0 <= progress <= 100)
     * \param progress
     * \param msg
     */
    void updateProgress(int progress, QString msg);

};

#define OiExchange_iidd "de.openIndy.Plugin.OiExchange.v001"

#endif // PI_OIEXCHANGEINTERFACE_H

