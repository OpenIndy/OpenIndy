#ifndef OIEXCHANGEOBJECT_H
#define OIEXCHANGEOBJECT_H

#include "featurewrapper.h"
#include <QIODevice>
#include <QObject>

/*!
 * \brief The oiExchangeObject class
 * exchange object which will be used by every oiexchange class to
 * import or export feature
 */
class OiExchangeObject : public QObject
{
    Q_OBJECT
public:
    explicit OiExchangeObject(QObject *parent = 0);
    OiExchangeObject(const OiExchangeObject &copy, QObject *parent = 0);
    OiExchangeObject &operator=(const OiExchangeObject &copy);

    //the device for import or export. For example a file
    QIODevice *device;

    //features that will be imported or exported
    QList<FeatureWrapper*> features;

    //units that shall be used for im- or export
    QMap<UnitConverter::dimensionType, UnitConverter::unitType> unit;

    //reference coordinate system of the features
    CoordinateSystem* nominalCoordSys;

    //defines wich element type will be imported
    //Configuration::ElementTypes typeOfElement;

};

#endif // OIEXCHANGEOBJECT_H
