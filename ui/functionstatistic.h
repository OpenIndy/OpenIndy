#ifndef FUNCTIONSTATISTIC_H
#define FUNCTIONSTATISTIC_H

#include <QAbstractTableModel>
#include "featurewrapper.h"
#include "function.h"
#include "oimetadata.h"
#include <QMutableMapIterator>

class FunctionStatistic : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit FunctionStatistic(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void setFunction(Function *f);
    void getKeys();
    void getIDs();

    Function *selectedFunction;

    QStringList residualName;
    QStringList originalResidualName;
    //QList<UnitConverter::dimensionType> residualUnit;
    QList<int> ElementIDs;

signals:

public slots:

    void updateModel();
};

#endif // FUNCTIONSTATISTIC_H
