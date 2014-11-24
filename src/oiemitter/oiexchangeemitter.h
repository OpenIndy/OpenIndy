#ifndef OIEXCHANGEEMITTER_H
#define OIEXCHANGEEMITTER_H

#include "oiemitter.h"

class OiExchangeEmitter : public OiEmitter
{
    Q_OBJECT

public:
    explicit OiExchangeEmitter(QObject *parent = 0);
};

#endif // OIEXCHANGEEMITTER_H
