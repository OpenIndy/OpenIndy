#ifndef PI_OIEXCHANGEDEFINEDFORMAT_H
#define PI_OIEXCHANGEDEFINEDFORMAT_H

#include "oiexchangeinterface.h"

/*!
 * \brief The OiExchangeDefinedFormat class
 * Interface that shall be used for files with a special standardized format (like *.obj or *.pts etc.)
 */
class OiExchangeDefinedFormat : public OiExchangeInterface
{
    Q_OBJECT

public:
    explicit OiExchangeDefinedFormat(QObject *parent = 0) : OiExchangeInterface(parent){}
    virtual ~OiExchangeDefinedFormat(){}

    /*!
     * \brief getSupportedFileExtensions
     * \return
     */
    virtual QStringList getSupportedFileExtensions() const = 0;

    /*!
     * \brief setUsedElements
     * Specify the types of elements that shall be im- or exported
     * \param usedElementTypes
     */
    void setUsedElements(QList<ElementTypes> usedElementTypes){
        this->usedElementTypes = usedElementTypes;
    }

protected:
    QList<ElementTypes> usedElementTypes; //types of elements that shall be im- or exported
};

#define OiExchangeDefinedFormat_iidd "de.openIndy.Plugin.OiExchange.OiExchangeDefinedFormat.v001"

#endif // PI_OIEXCHANGEDEFINEDFORMAT_H
