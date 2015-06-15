#ifndef FEATURETREEVIEWMODEL_H
#define FEATURETREEVIEWMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QPixmap>
#include <QPointer>

#include "featuretreeitem.h"
#include "oijob.h"
#include "types.h"
#include "util.h"

using namespace oi;

/*!
 * \brief The FeatureTreeViewModel class
 * Model that holds all features of an OpenIndy job in a tree structure
 */
class FeatureTreeViewModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit FeatureTreeViewModel(QPointer<OiJob> job, QObject *parent = 0);
    explicit FeatureTreeViewModel(QObject *parent = 0);

    //#######################################
    //override methods of abstract item model
    //#######################################

    int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QModelIndex	index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    QModelIndex	parent(const QModelIndex & index) const;

    //###############################
    //get or set current OpenIndy job
    //###############################

    const QPointer<OiJob> &getCurrentJob() const;
    void setCurrentJob(const QPointer<OiJob> &job);

    //#############
    //get root item
    //#############

    const QPointer<FeatureTreeItem> &getRootItem() const;

private slots:

    //###########################################
    //update the model when job state has changed
    //###########################################

    void updateModel();

private:

    //##############
    //helper methods
    //##############

    void connectJob();
    void disconnectJob();

    void addGeometries();

    //###########
    //current job
    //###########

    QPointer<OiJob> currentJob;

    //##############
    //root tree item
    //##############

    QPointer<FeatureTreeItem> rootItem;

};

#endif // FEATURETREEVIEWMODEL_H
