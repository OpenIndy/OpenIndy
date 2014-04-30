#ifndef DELETEFEATURESFUNCTOR_H
#define DELETEFEATURESFUNCTOR_H

#include "oifunctor.h"

class Controller;

class DeleteFeaturesFunctor : public OiFunctor
{
public:
    ~DeleteFeaturesFunctor();

    Controller *c;

    void operator()(QVariantList attributes);
};

#endif // DELETEFEATURESFUNCTOR_H
