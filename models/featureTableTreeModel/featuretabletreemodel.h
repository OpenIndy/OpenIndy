#ifndef FEATURETABLETREEMODEL_H
#define FEATURETABLETREEMODEL_H

#include <QAbstractItemModel>
#include <QPointer>
#include <QColor>

#include "featureitem.h"
#include "oijob.h"
#include "types.h"
#include "util.h"
#include "parameterdisplayconfig.h"
#include "console.h"
#include "measurementconfigmanager.h"


using namespace oi;

class FeatureTableTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:

    explicit FeatureTableTreeModel(const QPointer<OiJob> &job, QObject *parent = 0);
    explicit FeatureTableTreeModel(QObject *parent = 0);
    ~FeatureTableTreeModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    const QPointer<OiJob> &getCurrentJob() const;
    void setCurrentJob(const QPointer<OiJob> &job);

    void deleteChildren();

    FeatureItem* getRootItem();

signals:

    //#########################
    //send messages to OpenIndy
    //#########################

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

private:

    void setupModelData();

    void updateModel();

    void connectJob();
    void disconnectJob();

    FeatureItem *rootItem;

    //###########
    //current job
    //###########

    QPointer<OiJob> currentJob;

    //##############
    //display config
    //##############

    ParameterDisplayConfig parameterDisplayConfig;

    //##############
    //config manager
    //##############

    QPointer<MeasurementConfigManager> measurementConfigManager;

    //#################
    //helper attributes
    //#################

    ActualNominalFilter actualNominalFilter;


};

#endif // FEATURETABLETREEMODEL_H
