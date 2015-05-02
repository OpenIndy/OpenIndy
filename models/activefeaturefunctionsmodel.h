#ifndef ACTIVEFEATUREFUNCTIONSMODEL_H
#define ACTIVEFEATUREFUNCTIONSMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QPixmap>
#include <QPointer>

#include "functiontreeitem.h"
#include "oijob.h"
#include "types.h"
#include "util.h"
#include "function.h"

/*!
 * \brief The ActiveFeatureFunctionsModel class
 * Model that holds all functions of the active feature in a tree hierarchy
 */
class ActiveFeatureFunctionsModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit ActiveFeatureFunctionsModel(QPointer<OiJob> job, QObject *parent = 0);
    explicit ActiveFeatureFunctionsModel(QObject *parent = 0);

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

    const QPointer<FunctionTreeItem> &getRootItem() const;

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

    QPointer<FunctionTreeItem> rootItem;

};

#endif // ACTIVEFEATUREFUNCTIONSMODEL_H
