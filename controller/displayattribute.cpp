#include "displayattribute.h"

/*!
 * \brief DisplayAttribute constructor
 */
DisplayAttribute::DisplayAttribute()
{
    attrName = "";
    displayState = false;
}

/*!
 * \brief DisplayAttribute constructor
 * \param name
 * \param state
 */
DisplayAttribute::DisplayAttribute(QString name, bool state)
{
    this->attrName = name;
    this->displayState = state;
}
