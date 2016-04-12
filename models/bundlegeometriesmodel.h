#ifndef BUNDLEGEOMETRIESMODEL_H
#define BUNDLEGEOMETRIESMODEL_H

#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

#include "oijob.h"

#include "bundlegeometryitem.h"

/*!
 * \brief The BundleGeometriesModel class
 * Model with all bundle geometries
 */
class BundleGeometriesModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit BundleGeometriesModel(QObject *parent = 0);
    ~BundleGeometriesModel();

    //########################################
    //override methods of abstract table model
    //########################################

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex & index) const;

    //###############
    //getter / setter
    //###############

    //job
    const QPointer<oi::OiJob> &getCurrentJob() const;
    void setCurrentJob(const QPointer<oi::OiJob> &job);

    //stations
    const QJsonArray &getStations();
    void setStations(const QJsonArray &stations);

private slots:

    //###########################################
    //update the model when settings have changed
    //###########################################

    void updateModel();

private:

    //##############
    //helper methods
    //##############

    void connectJob();
    void disconnectJob();

    //#################
    //helper attributes
    //#################

    //root item and job
    BundleGeometryItem *rootItem;
    QPointer<oi::OiJob> currentJob;

    //used stations
    QJsonArray usedStations;


};

#endif // BUNDLEGEOMETRIESMODEL_H
