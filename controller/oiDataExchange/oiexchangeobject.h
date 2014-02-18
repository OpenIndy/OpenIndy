#ifndef OIEXCHANGEOBJECT_H
#define OIEXCHANGEOBJECT_H

#include <QObject>
#include "featurewrapper.h"
#include <QIODevice>

class oiExchangeObject : public QObject
{
    Q_OBJECT
public:
    explicit oiExchangeObject(QObject *parent = 0);

    QIODevice *device;
    QList<FeatureWrapper*> features;
    CoordinateSystem* nominalCoordSys;
    Configuration::ElementTypes typeOfElement;


signals:

public slots:

};

#endif // OIEXCHANGEOBJECT_H
