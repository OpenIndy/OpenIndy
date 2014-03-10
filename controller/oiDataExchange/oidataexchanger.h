#ifndef OIDATAEXCHANGER_H
#define OIDATAEXCHANGER_H

#include <QString>
#include <QStringList>

#include "oiexchangeascii.h"

/*!
 * \brief The oiDataExchanger class
 * static class to manage all the different kinds of data import defined by
 * the oiExchangeInterface. You have to add your oiexchange class here.
 */
class oiDataExchanger
{

public:

    oiDataExchanger();

    /*!
     * \brief importData
     *  static import function
     * \param exchangeData
     * \param format
     * \return
     */
    static bool importData(oiExchangeObject &exchangeData, QString format);

    /*!
     * \brief exportData
     *  static export function
     * \param exchangeData
     * \param format
     * \return
     */
    static bool exportData(oiExchangeObject &exchangeData, QString format);

    /*!
     * \brief getOutputFormats
     *  add a string tag for your oiexchange class here
     * \return
     */
    static QStringList getOutputFormats();

    /*!
     * \brief getOutputFormats
     *  add the supported element types of your oiexchange class here
     * \return
     */
    static QList<Configuration::ElementTypes> getSupportedElements(QString format);


private:

    /*!
     * \brief exchangeData
     * add the import and export methode of your oiexchange class here
     * \param exchangeData
     * \param format
     * \param import
     * \return
     */
    static bool exchangeData(oiExchangeObject &exchangeData,QString format,bool import);

};

#endif // OIDATAEXCHANGER_H
