#ifndef OIEXCHANGEINTERFACE_H
#define OIEXCHANGEINTERFACE_H

#include <QStringList>
#include <QObject>

#include "oiexchangeobject.h"

/*!
 * \brief The oiExchange Interface must be implemented for a new kind of data import/export.
 * You define which and how features can be imported/exported by external data. After you have
 * implemented your import/export class you have to add it to the static class oidataexchanger.
 * For an example you can take a look at the oiexchangeascii class.
 */
class oiExchangeInterface : public QObject
{
    Q_OBJECT

public:
    explicit oiExchangeInterface(QObject *parent = 0){}
    virtual ~oiExchangeInterface(){}

    /*!
     * \brief importOiData
     * import data here by filling a projectData exchange Object.
     * \param projectData
     * \return
     */
    virtual bool importOiData(oiExchangeObject &projectData) = 0;

    /*!
     * \brief exportOiData
     *  export data here by filling a projectData exchange Object.
     * \param projectData
     * \return
     */
    virtual bool exportOiData(oiExchangeObject &projectData) = 0;


    /*!
     * \brief getSupportedElements
     *  define here which element types can be imported by your class
     * \return
     */
    virtual QList<Configuration::ElementTypes> getSupportedElements() = 0;

    /*!
     * \brief getElementDescription
     * \return
     */
    virtual QStringList getElementDescription(Configuration::ElementTypes) = 0;

    /*!
     * \brief getSupportedDistanceUnits
     * \param format
     * \return
     */
    virtual QList<UnitConverter::unitType> getSupportedDistanceUnits() = 0;

    /*!
     * \brief getSupportedAngleUnits
     * \return
     */
    virtual QList<UnitConverter::unitType> getSupportedAngleUnits() = 0;

    /*!
     * \brief getSupportedTemperatureUnits
     * \return
     */
    virtual QList<UnitConverter::unitType> getSupportedTemperatureUnits() = 0;

signals:
    void updateProgress(int progress, QString msg); //progress (0-100)

};


#endif // OIEXCHANGEINTERFACE_H

