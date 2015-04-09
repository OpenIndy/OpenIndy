#ifndef RADIUS_H
#define RADIUS_H

#include <QObject>

#include "element.h"

class Radius : public Element
{
    Q_OBJECT

public:
    explicit Radius(QObject *parent = 0);
    explicit Radius(const double &r, QObject *parent = 0);

    Radius(const Radius &copy, QObject *parent = 0);

    Radius &operator=(const Radius &copy);

    ~Radius();

    //#################
    //get or set radius
    //#################

    void setRadius(const double &r);

    const double &getRadius() const;

protected:
    double radius;

};

#endif // RADIUS_H
