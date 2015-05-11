#ifndef MEASUREMENTCONFIGURATIONPROXYMODEL_H
#define MEASUREMENTCONFIGURATIONPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QPointer>

#include "types.h"
#include "util.h"
#include "measurementconfigurationmodel.h"

/*!
 * \brief The MeasurementConfigurationProxyModel class
 * Model that holds all available measurement configurations filtered by saved and project property
 */
class MeasurementConfigurationProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit MeasurementConfigurationProxyModel(QObject *parent = 0);

    //##########
    //set filter
    //##########

    void setFilter(const bool &showAll);

    //###########################################
    //override methods of sort filter proxy model
    //###########################################

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

protected:

    bool filterAcceptsRow (int source_row, const QModelIndex &source_parent) const;
    bool filterAcceptsColumn (int source_column, const QModelIndex &source_parent) const;

private:

    //################
    //filter attribute
    //################

    bool showAll;

};

#endif // MEASUREMENTCONFIGURATIONPROXYMODEL_H
