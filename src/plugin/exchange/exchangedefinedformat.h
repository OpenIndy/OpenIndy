#ifndef EXCHANGEDEFINEDFORMAT_H
#define EXCHANGEDEFINEDFORMAT_H

#include "exchangeinterface.h"

/*!
 * \brief The OiExchangeDefinedFormat class
 * Interface that shall be used for files with a special standardized format (like *.obj or *.pts etc.)
 */
class ExchangeDefinedFormat : public ExchangeInterface
{
    Q_OBJECT

public:
    explicit ExchangeDefinedFormat(QObject *parent = 0);

    virtual ~ExchangeDefinedFormat();

    //#########################################
    //methods to get or set further information
    //#########################################

    const QStringList &getSupportedFileExtensions() const;

    const QList<ElementTypes> &getUsedElements() const;
    void setUsedElements(const QList<ElementTypes> &usedElementTypes);

protected:

    //###########################
    //input and output parameters
    //###########################

    QList<ElementTypes> usedElementTypes; //types of elements that shall be im- or exported

    QStringList supportedFileExtensions;

};

#define ExchangeDefinedFormat_iidd "de.openIndy.plugin.exchange.exchangeDefinedFormat.v001"

#endif // EXCHANGEDEFINEDFORMAT_H
