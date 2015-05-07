#ifndef AVAILABLEELEMENTSTREEVIEWPROXYMODEL_H
#define AVAILABLEELEMENTSTREEVIEWPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QPointer>

#include "oijob.h"
#include "types.h"
#include "util.h"
#include "featuretreeitem.h"
#include "featuretreeviewmodel.h"
#include "function.h"

/*!
 * \brief The AvailableElementsTreeViewProxyModel class
 * Model that holds all available elements that fit the requirements of a function of the active feature
 */
class AvailableElementsTreeViewProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit AvailableElementsTreeViewProxyModel(QObject *parent = 0);

    //###############################
    //get or set current OpenIndy job
    //###############################

    const QPointer<OiJob> &getCurrentJob() const;
    void setCurrentJob(const QPointer<OiJob> &job);

    //##########
    //set filter
    //##########

    void setSelectedFunctionPosition(const int &functionPosition, const int &neededElementIndex);

    //###############################################
    //get or set information about available elements
    //###############################################

    bool validateSelection(const QModelIndexList &selection) const;

    void addInputElements(const QModelIndexList &selection);

    //###########################################
    //override methods of sort filter proxy model
    //###########################################

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

signals:

    //###########################
    //signals to change job state
    //###########################

    void addInputObservation(const QPointer<FeatureWrapper> &target, const int &functionPosition, const int &neededElementsIndex, const QPointer<Observation> &observation);
    void addInputReading(const QPointer<FeatureWrapper> &target, const int &functionPosition, const int &neededElementsIndex, const QPointer<Reading> &reading);
    void addInputFeature(const QPointer<FeatureWrapper> &target, const int &functionPosition, const int &neededElementsIndex, const QPointer<FeatureWrapper> &feature);

protected:

    //###########################################
    //override methods of sort filter proxy model
    //###########################################

    bool filterAcceptsRow (int source_row, const QModelIndex &source_parent) const;
    bool filterAcceptsColumn (int source_column, const QModelIndex &source_parent) const;

private:

    //##############
    //helper methods
    //##############

    void connectJob();
    void disconnectJob();

    void resetSelectedFunctionPosition();

    void addInputElement(const QPointer<FeatureTreeItem> &item, const ElementTypes &type);

    //###########
    //current job
    //###########

    QPointer<OiJob> currentJob;

    //#################
    //function position
    //#################

    int functionPosition; //specifies the index of the function list of the active feature
    int neededElementIndex; //specifies the needed element index of the function at functionPosition

    //#######################
    //save connected function
    //#######################

    QPointer<Function> connectedFunction;

};

#endif // AVAILABLEELEMENTSTREEVIEWPROXYMODEL_H
