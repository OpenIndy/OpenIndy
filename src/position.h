#ifndef POSITION_H
#define POSITION_H

#include <QObject>

#include "element.h"
#include "oivec.h"

class Position : public Element
{
    Q_OBJECT

public:
    explicit Position(QObject *parent = 0);
    explicit Position(const OiVec &v, QObject *parent = 0);
    explicit Position(const double &x, const double &y, const double &z, const double &h = 1.0, QObject *parent = 0);

    Position(const Position &copy, QObject *parent = 0);

    Position &operator=(const Position &copy);

    ~Position();

    //##########################
    //get or set position vector
    //##########################

    void setVector(const OiVec &v);
    void setVector(const double &x, const double &y, const double &z, const double &h = 1.0);

    const OiVec &getVector() const;
    const OiVec &getVectorH() const;

protected:
    OiVec xyz; //vector of size 3 (x, y, z)
    OiVec xyzH; //homogeneous vector of size 4 (x, y, z, h)

};

#endif // POSITION_H
