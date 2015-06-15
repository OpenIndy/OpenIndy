#ifndef FUNCTIONTABLEMODEL_H
#define FUNCTIONTABLEMODEL_H

#include <QAbstractTableModel>
#include <QObject>
#include <QPointer>
#include <QList>

#include "oijob.h"
#include "types.h"
#include "util.h"
#include "systemdbmanager.h"

using namespace oi;

/*!
 * \brief The FunctionTableModel class
 * Model that holds all available function plugins
 */
class FunctionTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit FunctionTableModel(QObject *parent = 0);

    //########################################
    //override methods of abstract table model
    //########################################

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    //########################
    //get function information
    //########################

    QString getFunctionDescription(const QModelIndex &index) const;
    QString getFunctionName(const QModelIndex &index) const;
    QString getPluginFilePath(const QModelIndex &index) const;

    //######################################
    //get or set currently available plugins
    //######################################

    const QList<sdb::Plugin> &getPlugins() const;
    void setPlugins(const QList<sdb::Plugin> &plugins);

private slots:

    //##############################################
    //update the model when the plugins were changed
    //##############################################

    void updateModel();

private:

    //###########################
    //currently available plugins
    //###########################

    QList<sdb::Plugin> plugins;
    
};

#endif // FUNCTIONTABLEMODEL_H
