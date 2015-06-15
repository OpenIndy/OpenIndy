#ifndef PLUGINTREEVIEWMODEL_H
#define PLUGINTREEVIEWMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QPixmap>

#include "plugintreeitem.h"
#include "systemdbmanager.h"

using namespace oi;

/*!
 * \brief The PluginTreeViewModel class
 * Model that holds all plugins currently loaded into OpenIndy
 */
class PluginTreeViewModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit PluginTreeViewModel(QObject *parent = 0);

    //#######################################
    //override methods of abstract item model
    //#######################################

    QModelIndex	index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    QModelIndex	parent(const QModelIndex & index) const;

    int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    //##################
    //get or set plugins
    //##################

    const QList<sdb::Plugin> &getPlugins() const;
    void setPlugins(const QList<sdb::Plugin> &plugins);

private:

    //##############
    //loaded plugins
    //##############

    QList<sdb::Plugin> plugins;

    //##############
    //root tree item
    //##############

    PluginTreeItem *rootItem;

};

#endif // PLUGINTREEVIEWMODEL_H
