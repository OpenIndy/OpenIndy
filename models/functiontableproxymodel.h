#ifndef FUNCTIONTABLEPROXYMODEL_H
#define FUNCTIONTABLEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QPointer>

#include "oijob.h"
#include "types.h"
#include "util.h"
#include "functiontablemodel.h"
#include "oimetadata.h"
#include "pluginloader.h"

using namespace oi;

/*!
 * \brief The FunctionTableProxyModel class
 * Model that holds all available function plugins filtered by the function types that could be added to the active feature's function list
 */
class FunctionTableProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit FunctionTableProxyModel(QObject *parent = 0);

    //####################################
    //add a function to the active feature
    //####################################

    void addFunction(const QModelIndex &index);

    //###############################
    //get or set current OpenIndy job
    //###############################

    const QPointer<OiJob> &getCurrentJob() const;
    void setCurrentJob(const QPointer<OiJob> &job);

    QVariant data(const QModelIndex &index, int role) const;

signals:

    //###########################
    //signals to change job state
    //###########################

    void startAddFunction(const QPointer<Function> &function);

    //#########################
    //send messages to OpenIndy
    //#########################

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);
    
protected:

    //###########################################
    //override methods of sort filter proxy model
    //###########################################

    bool filterAcceptsRow (int source_row, const QModelIndex &source_parent ) const;
    bool filterAcceptsColumn (int source_column, const QModelIndex &source_parent ) const;

private:

    //##############
    //helper methods
    //##############

    void connectJob();
    void disconnectJob();

    //###########
    //current job
    //###########

    QPointer<OiJob> currentJob;
    
};

#endif // FUNCTIONTABLEPROXYMODEL_H
