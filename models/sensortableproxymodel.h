#ifndef SENSORTABLEPROXYMODEL_H
#define SENSORTABLEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QPointer>

#include "oijob.h"
#include "types.h"
#include "util.h"
#include "sensortablemodel.h"
#include "oimetadata.h"
#include "pluginloader.h"

/*!
 * \brief The SensorTableProxyModel class
 * Model that holds all available sensor plugins filtered by sensor type
 */
class SensorTableProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit SensorTableProxyModel(QObject *parent = 0);

    //######################
    //set sensor type filter
    //######################

    const SensorTypes &getSensorType() const;
    void setSensorType(const SensorTypes &type);
    
protected:

    //###########################################
    //override methods of sort filter proxy model
    //###########################################

    bool filterAcceptsRow (int source_row, const QModelIndex &source_parent ) const;
    bool filterAcceptsColumn (int source_column, const QModelIndex &source_parent ) const;

private:

    //###################
    //current sensor type
    //###################

    SensorTypes sensorType;

};

#endif // SENSORTABLEPROXYMODEL_H
