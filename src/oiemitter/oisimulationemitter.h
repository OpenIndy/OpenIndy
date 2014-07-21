#ifndef OISIMULATIONEMITTER_H
#define OISIMULATIONEMITTER_H

#include "oiemitter.h"

class OiSimulationEmitter : public OiEmitter
{
    Q_OBJECT

public:
    explicit OiSimulationEmitter(QObject *parent = 0);
};

#endif // OISIMULATIONEMITTER_H
