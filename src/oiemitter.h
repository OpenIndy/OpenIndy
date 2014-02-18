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
    void sendDataMap(QVariantMap*);
    void sendString(QString);

public slots:
    void emitSendDataMap(QVariantMap*);
    void emitSendString(QString);


};

#endif // OIEMITTER_H
