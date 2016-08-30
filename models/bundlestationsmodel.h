#ifndef BUNDLESTATIONSMODEL_H
#define BUNDLESTATIONSMODEL_H

#include <QAbstractTableModel>
#include <QAbstractItemModel>
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

#include "oijob.h"

#include "bundlestationitem.h"

/*!
 * \brief The BundleStationsModel class
 * Model with all bundle stations
 */
class BundleStationsModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit BundleStationsModel(QObject *parent = 0);
    ~BundleStationsModel();

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

    //template
    const QJsonObject &getBundleTemplate();
    void setBundleTemplate(const QJsonObject &bundleTemplate);

signals:

    //#####################################
    //signals to inform about model changes
    //#####################################

    void stationsChanged();

private slots:

    //################
    //update the model
    //################

    void updateModel();

private:

    //##############
    //helper methods
    //##############

    void connectJob();
    void disconnectJob();

    //update json from current selection
    void updateJson();

    //#################
    //helper attributes
    //#################

    //root item and job
    BundleStationItem *rootItem;
    QPointer<oi::OiJob> currentJob;

    //used stations and template
    QJsonObject bundleTemplate;
    QJsonArray usedStations;

};

#endif // BUNDLESTATIONSMODEL_H
