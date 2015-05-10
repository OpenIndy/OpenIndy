#ifndef NURBS_H
#define NURBS_H

#include <QObject>
#include <QtXml>

#include "geometry.h"

/*!
 * \brief The Nurbs class
 */
class Nurbs : public Geometry
{
    Q_OBJECT

public:
    Nurbs(const bool &isNominal, QObject *parent = 0);

    Nurbs(const Nurbs &copy, QObject *parent = 0);

    Nurbs &operator=(const Nurbs &copy);

    ~Nurbs();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum NurbsUnknowns{

    };

    //###################################################################
    //check wether this geometry has a direction, a position and a radius
    //###################################################################

    //###########################
    //get or set nurbs parameters
    //###########################

    //###########################
    //reexecute the function list
    //###########################

    void recalc();

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

};

#endif // NURBS_H
