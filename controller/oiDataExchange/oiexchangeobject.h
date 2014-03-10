#ifndef OIEXCHANGEOBJECT_H
#define OIEXCHANGEOBJECT_H

#include <QObject>
#include "featurewrapper.h"
#include <QIODevice>

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

    //the device for import or export. For example a file
    QIODevice *device;

    //features that will be imported or exported
    QList<FeatureWrapper*> features;

    // Reference coordinate system of the features
    CoordinateSystem* nominalCoordSys;

    //defines wich element type will be imported
    Configuration::ElementTypes typeOfElement;


signals:

public slots:

};

#endif // OIEXCHANGEOBJECT_H
