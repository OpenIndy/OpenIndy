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
class oiExchangeObject : public QObject
{
    Q_OBJECT
public:
    explicit oiExchangeObject(QObject *parent = 0);
    oiExchangeObject(const oiExchangeObject &copy, QObject *parent = 0);
    oiExchangeObject &operator=(const oiExchangeObject &copy);

    //the device for import or export. For example a file
    QIODevice *device;

    //features that will be imported or exported
    QList<FeatureWrapper*> features;

    QMap<UnitConverter::dimensionType, UnitConverter::unitType> unit;

    //Reference coordinate system of the features
    CoordinateSystem* nominalCoordSys;

    //defines wich element type will be imported
    Configuration::ElementTypes typeOfElement;

};

#endif // OIEXCHANGEOBJECT_H
