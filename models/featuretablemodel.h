#ifndef FEATURETABLEMODEL_H
#define FEATURETABLEMODEL_H

#include <QAbstractTableModel>
#include <QObject>
#include <QPointer>
#include <QColor>

#include "oijob.h"
#include "types.h"
#include "util.h"
#include "parameterdisplayconfig.h"

/*!
 * \brief The FeatureTableModel class
 * Model with all features of an OpenIndy job
 */
class FeatureTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit FeatureTableModel(const QPointer<OiJob> &job, QObject *parent = 0);
    explicit FeatureTableModel(QObject *parent = 0);

    //########################################
    //override methods of abstract table model
    //########################################

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    //##############################
    //get id of the feature at index
    //##############################

    int getFeatureIdAtIndex(const QModelIndex &index);

    //#########################
    //get or set active feature
    //#########################

    QPointer<FeatureWrapper> getActiveFeature() const;
    void setActiveFeature(const QModelIndex &index);

    //###############################
    //get or set current OpenIndy job
    //###############################

    const QPointer<OiJob> &getCurrentJob() const;
    void setCurrentJob(const QPointer<OiJob> &job);

    //#########################
    //get or set display config
    //#########################

    const ParameterDisplayConfig &getParameterDisplayConfig() const;
    void setParameterDisplayConfig(const ParameterDisplayConfig &config);

private slots:

    //##############################################################
    //update the model when job state or display config have changed
    //##############################################################

    void updateModel();

private:

    //##############
    //helper methods
    //##############

    QString getDisplayValue(const QPointer<FeatureWrapper> &feature, const int &column) const;

    void connectJob();
    void disconnectJob();

private:

    //###########
    //current job
    //###########

    QPointer<OiJob> currentJob;

    //##############
    //display config
    //##############

    ParameterDisplayConfig parameterDisplayConfig;
    
};

#endif // FEATURETABLEMODEL_H
