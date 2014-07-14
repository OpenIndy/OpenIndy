#ifndef OINETWORKADJUSTMENTEMITTER_H
#define OINETWORKADJUSTMENTEMITTER_H

#include "oiemitter.h"

class OiNetworkAdjustmentEmitter : public OiEmitter
{

    Q_OBJECT

public:
    explicit OiNetworkAdjustmentEmitter(QObject *parent = 0);
};

#endif // OINETWORKADJUSTMENTEMITTER_H
