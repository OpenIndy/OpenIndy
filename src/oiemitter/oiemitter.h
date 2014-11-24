#ifndef OIEMITTER_H
#define OIEMITTER_H

#include <QObject>
#include <QVariant>
#include <QMap>
#include "reading.h"


class OiEmitter : public QObject
{
    Q_OBJECT
public:
    explicit OiEmitter(QObject *parent = 0);

signals:
    void sendString(QString) const;

public slots:
    void emitSendString(QString) const;

};

#endif // OIEMITTER_H
