#include "exchangedefinedformat.h"

/*!
 * \brief ExchangeDefinedFormat::ExchangeDefinedFormat
 * \param parent
 */
ExchangeDefinedFormat::ExchangeDefinedFormat(QObject *parent) : ExchangeInterface(parent){

}

/*!
 * \brief ExchangeDefinedFormat::~ExchangeDefinedFormat
 */
ExchangeDefinedFormat::~ExchangeDefinedFormat(){

}

/*!
 * \brief ExchangeDefinedFormat::getSupportedFileExtensions
 * \return
 */
const QStringList &ExchangeDefinedFormat::getSupportedFileExtensions() const{
    return this->supportedFileExtensions;
}

/*!
 * \brief ExchangeDefinedFormat::getUsedElements
 * \return
 */
const QList<ElementTypes> &ExchangeDefinedFormat::getUsedElements() const{
    return this->usedElementTypes;
}

/*!
 * \brief ExchangeDefinedFormat::setUsedElements
 * Specify the types of elements that shall be im- or exported
 * \param usedElementTypes
 */
void ExchangeDefinedFormat::setUsedElements(const QList<ElementTypes> &usedElementTypes){
    this->usedElementTypes = usedElementTypes;
}
