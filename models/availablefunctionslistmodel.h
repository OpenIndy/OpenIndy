#ifndef AVAILABLEFUNCTIONSLISTMODEL_H
#define AVAILABLEFUNCTIONSLISTMODEL_H

#include <QAbstractListModel>
#include <QPointer>
#include <QModelIndex>
#include <QPixmap>
#include <QList>
#include <QPair>

#include "types.h"
#include "util.h"
#include "systemdbmanager.h"
#include "function.h"

using namespace oi;

/*!
 * \brief The AvailableFunctionsListModel class
 * Model that holds all available create functions (possible first functions of a feature)
 */
class AvailableFunctionsListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit AvailableFunctionsListModel(QObject *parent = 0);

    //#######################################
    //override methods of abstract item model
    //#######################################

    int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    //#########################
    //get the function at index
    //#########################

    sdb::Function getFunctionAtIndex(const QModelIndex &index) const;

    //############################
    //get or set default functions
    //############################

    QPair<QString, QString> getDefaultFunction(const FeatureTypes &type) const;
    void setDefaultFunction(const FeatureTypes &type, const QPair<QString, QString> &function);

    //############################
    //get or set available plugins
    //############################

    const QList<sdb::Plugin> &getPlugins() const;
    void setPlugins(const QList<sdb::Plugin> &plugins);

signals:

    //###########################
    //signals to change job state
    //###########################

    void startRemoveUsedElements(const QPointer<FeatureWrapper> &target, const int &functionPosition, const int &neededElementsIndex, const int &elementId);

private slots:

    //###########################################
    //update the model when job state has changed
    //###########################################

    void updateModel();

private:

    //#################
    //available plugins
    //#################

    QList<sdb::Plugin> plugins;

};

#endif // AVAILABLEFUNCTIONSLISTMODEL_H
