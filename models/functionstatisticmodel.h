#ifndef FUNCTIONSTATISTICMODEL_H
#define FUNCTIONSTATISTICMODEL_H

#include <QAbstractTableModel>
#include <QPointer>

#include "feature.h"
#include "function.h"
#include "statistic.h"
#include "types.h"
#include "util.h"
#include "parameterdisplayconfig.h"

using namespace oi;

/*!
 * \brief The FunctionStatisticModel class
 * Model that holds the statistic of a specific function of a feature
 */
class FunctionStatisticModel : public QAbstractTableModel
{

public:
    explicit FunctionStatisticModel(const QPointer<Feature> &feature, QObject *parent = 0);

    //########################################
    //override methods of abstract table model
    //########################################

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    //##############################
    //get or set the current feature
    //##############################

    const QPointer<Feature> &getFeature() const;
    void setFeature(const QPointer<Feature> &feature);

    //#####################################
    //get or set the current function index
    //#####################################

    const int &getFunctionIndex() const;
    void setFunctionIndex(const int &index);

    //#########################
    //get or set display config
    //#########################

    const ParameterDisplayConfig &getParameterDisplayConfig() const;
    void setParameterDisplayConfig(const ParameterDisplayConfig &config);

private slots:

    //##############
    //helper methods
    //##############

    void updateModel();

private:

    //##################################
    //current feature and function index
    //##################################

    QPointer<Feature> feature;
    int functionIndex;

    //##############
    //display config
    //##############

    ParameterDisplayConfig parameterDisplayConfig;

};

#endif // FUNCTIONSTATISTICMODEL_H
