#ifndef POINTFEATUREFILTERMODEL_H
#define POINTFEATUREFILTERMODEL_H

#include <QSortFilterProxyModel>
#include <QString>

#include "featurewrapper.h"
#include "oifeaturestate.h"

class PointFeatureFilterModel : public QSortFilterProxyModel
{
public:
    explicit PointFeatureFilterModel(QObject *parent = 0);

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

public slots:
    void setFilter(QString group);
    QList<FeatureWrapper *> getSelectedPoints(QModelIndexList selectedIndices);

private:
    QString filterGroup;

};

#endif // POINTFEATUREFILTERMODEL_H
