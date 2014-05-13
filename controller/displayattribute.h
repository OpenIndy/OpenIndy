#ifndef DISPLAYATTRIBUTE_H
#define DISPLAYATTRIBUTE_H

#include <QString>

/*!
 * \brief The DisplayAttribute class is a storage object for attribute names and display states.
 * It is used for communication between configuration class settings dialog and the proxy models
 * that wether display or not the specified attributes.
 */
class DisplayAttribute
{
public:
    DisplayAttribute();
    DisplayAttribute(QString name, bool state);

    QString attrName;
    bool displayState;
};

#endif // DISPLAYATTRIBUTE_H
