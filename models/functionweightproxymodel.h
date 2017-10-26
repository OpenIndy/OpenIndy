#ifndef FUNCTIONWEIGHTPROXYMODEL_H
#define FUNCTIONWEIGHTPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QPointer>
#include "oijob.h"

#include "functionweightstablemodel.h"

using namespace oi;

/*!
 * \brief The FunctionWeightProxyModel class
 */
class FunctionWeightProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit FunctionWeightProxyModel(QObject *parent = 0);

    //###############################
    //get or set current OpenIndy job
    //###############################

    const QPointer<OiJob> &getCurrentJob() const;
    void setCurrentJob(const QPointer<OiJob> &job);

    //###########################################
    //override methods of sort filter proxy model
    //###########################################

    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const;

protected:

    bool filterAcceptsRow (int source_row, const QModelIndex &source_parent ) const;
    bool filterAcceptsColumn (int source_column, const QModelIndex &source_parent ) const;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;

private:
    QPointer<OiJob> job;
};

#endif // FUNCTIONWEIGHTPROXYMODEL_H
