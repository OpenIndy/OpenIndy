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

using namespace oi;

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

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    //###############################
    //get or set function information
    //###############################

    //remove functions
    bool canRemoveFunction(const QModelIndex &index) const;
    void removeFunction(const QModelIndex &index);

    //get description of function and input elements
    QString getFunctionDescription(const QModelIndex &index) const;
    QString getInputElementDescription(const QModelIndex &index) const;

    //get user defined scalar input parameters
    QMap<QString, int> getIntegerParameter(const QModelIndex &index) const;
    QMap<QString, double> getDoubleParameter(const QModelIndex &index) const;
    QMap<QString, QString> getStringParameterSelection(const QModelIndex &index) const;

    //set scalar input parameters
    void setScalarInputParameters(const QModelIndex &index, const QMap<QString, int> &intParams, const QMap<QString, double> &doubleParams, const QMap<QString, QString> &stringParams);

    //get all available string parameters
    QMultiMap<QString, QString> getStringParameter(const QModelIndex &index) const;

    //###############################
    //get or set current OpenIndy job
    //###############################

    const QPointer<OiJob> &getCurrentJob() const;
    void setCurrentJob(const QPointer<OiJob> &job);

    //#############
    //get root item
    //#############

    const QPointer<FunctionTreeItem> &getRootItem() const;

signals:

    //###########################
    //signals to change job state
    //###########################

    void startRemoveFunction(const int &functionIndex);

    //#########################
    //send messages to OpenIndy
    //#########################

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);

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
