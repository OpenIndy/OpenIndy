#ifndef POINTFEATUREMODEL_H
#define POINTFEATUREMODEL_H

#include <QAbstractListModel>

#include "featurewrapper.h"
#include "oifeaturestate.h"

class PointFeatureModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit PointFeatureModel(QObject *parent = 0);

    int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

};

#endif // POINTFEATUREMODEL_H
