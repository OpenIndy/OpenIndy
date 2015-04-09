#ifndef DIRECTION_H
#define DIRECTION_H

#include <QObject>

#include "element.h"
#include "oivec.h"

class Direction : public Element
{
    Q_OBJECT

public:
    explicit Direction(QObject *parent = 0);
    explicit Direction(const OiVec &v, QObject *parent = 0);
    explicit Direction(const double &x, const double &y, const double &z, const double &h = 1.0, QObject *parent = 0);

    Direction(const Direction &copy, QObject *parent = 0);

    Direction &operator=(const Direction &copy);

    ~Direction();

    //###########################
    //get or set direction vector
    //###########################

    void setVector(const double &x, const double &y, const double &z, const double &h = 1.0);
    void setVector(const OiVec &v);

    const OiVec &getVector() const;
    const OiVec &getVectorH() const;

private:
    OiVec ijk; //vector of size 3 (i, j, k)
    OiVec ijkH; //vector of size 4 (i, j, k, h)

};

#endif // DIRECTION_H
