#ifndef PI_OIEXCHANGESPECIALFORMAT_H
#define PI_OIEXCHANGESPECIALFORMAT_H

#include "pi_oiexchangeinterface.h"

/*!
 * \brief The OiExchangeSpecialFormat class
 * Interface that shall be used for files with a special standardized format (like *.obj or *.pts etc.)
 */
class OiExchangeSpecialFormat : public OiExchangeInterface
{
    Q_OBJECT

public:
    explicit OiExchangeSpecialFormat(QObject *parent = 0) : OiExchangeInterface(parent){}
    virtual ~OiExchangeSpecialFormat(){}

    /*!
     * \brief setUsedElements
     * Specify the types of elements that shall be im- or exported
     * \param usedElementTypes
     */
    void setUsedElements(QList<Configuration::ElementTypes> usedElementTypes){
        this->usedElementTypes = usedElementTypes;
    }

private:
    QList<Configuration::ElementTypes> usedElementTypes; //types of elements that shall be im- or exported
};

#define OiExchangeSpecialFormat_iidd "de.openIndy.Plugin.OiExchangeSpecialFormat.v001"

#endif // PI_OIEXCHANGESPECIALFORMAT_H
