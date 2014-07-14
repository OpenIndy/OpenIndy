#ifndef OIFUNCTIONEMITTER_H
#define OIFUNCTIONEMITTER_H

#include "oiemitter.h"

class OiFunctionEmitter : public OiEmitter
{

    Q_OBJECT

public:
    explicit OiFunctionEmitter(QObject *parent = 0);
};

#endif // OIFUNCTIONEMITTER_H
