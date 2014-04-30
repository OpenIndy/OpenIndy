#ifndef OIFUNCTOR_H
#define OIFUNCTOR_H

#include <QVariantList>

class OiFunctor
{
public:
    virtual ~OiFunctor(){}

    virtual void operator()(QVariantList attributes) = 0;
};

#endif // OIFUNCTOR_H
