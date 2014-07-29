#ifndef OIFUNCTIONEMITTER_H
#define OIFUNCTIONEMITTER_H

#include "oiemitter.h"
#include "featurewrapper.h"

class OiFunctionEmitter : public OiEmitter
{
    Q_OBJECT

public:
    explicit OiFunctionEmitter(QObject *parent = 0);

    void addFeature(FeatureWrapper *myFeature);

signals:
    void featureAdded(FeatureWrapper *myFeature);

};

#endif // OIFUNCTIONEMITTER_H
