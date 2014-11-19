#ifndef OIEXCHANGEOBJECT_H
#define OIEXCHANGEOBJECT_H

#include "featurewrapper.h"
#include <QIODevice>
#include <QObject>

/*!
 * \brief The oiExchangeObject class
 * exchange object which is used by each OiExchange plugin
 */
class OiExchangeObject : public QObject
{
    Q_OBJECT
public:
    explicit OiExchangeObject(QObject *parent = 0);

    OiExchangeObject(const OiExchangeObject &copy, QObject *parent = 0);
    OiExchangeObject &operator=(const OiExchangeObject &copy);

    QIODevice *device; //the device used for the im- or export (e.g. a file)
    QList<FeatureWrapper*> features; //the features that shall be exported or were imported

};

#endif // OIEXCHANGEOBJECT_H
