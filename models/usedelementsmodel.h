#ifndef USEDELEMENTSMODEL_H
#define USEDELEMENTSMODEL_H

#include <QAbstractListModel>
#include <QPointer>
#include <QModelIndex>
#include <QPixmap>
#include <QList>

#include "oijob.h"
#include "types.h"
#include "util.h"
#include "featuretreeitem.h"
#include "function.h"

/*!
 * \brief The UsedElementsModel class
 * Model that holds all used elements of a function of the active feature at a special position
 */
class UsedElementsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit UsedElementsModel(QObject *parent = 0);

    //#######################################
    //override methods of abstract item model
    //#######################################

    int	rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    //#####################################
    //access used element id by model index
    //#####################################

    int getElementIdAtIndex(const QModelIndex &index);

    //###############################
    //get or set current OpenIndy job
    //###############################

    const QPointer<OiJob> &getCurrentJob() const;
    void setCurrentJob(const QPointer<OiJob> &job);

    //##########
    //set filter
    //##########

    void setSelectedFunctionPosition(const int &functionIndex, const int &neededElementIndex);

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

    void resetSelectedFunctionPosition();

    //###########
    //current job
    //###########

    QPointer<OiJob> currentJob;

    //#################
    //function position
    //#################

    int functionPosition; //specifies the index of the function list of the active feature
    int neededElementIndex; //specifies the needed element index of the function at functionPosition

};

#endif // USEDELEMENTSMODEL_H
