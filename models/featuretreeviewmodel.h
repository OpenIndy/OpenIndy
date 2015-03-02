#ifndef FEATURETREEVIEWMODEL_H
#define FEATURETREEVIEWMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QPixmap>
#include <QPointer>

#include "featuretreeitem.h"
#include "oijob.h"
#include "types.h"
#include "util.h"

class FeatureTreeViewModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit FeatureTreeViewModel(QPointer<OiJob> job, QObject *parent = 0);
    explicit FeatureTreeViewModel(QObject *parent = 0);

    QModelIndex	index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    QModelIndex	parent(const QModelIndex & index) const;
    int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    FeatureTreeItem* getRootItem() const;

public slots:
    void refreshModel();

private:
    QPointer<OiJob> currentJob;
    QPointer<FeatureTreeItem> rootItem;

    void appendGeometries(const QList<FeatureWrapper*> &geometries, const FeatureTypes &type);
    void appendCoordinateSystems(const QList<FeatureWrapper*> &coordinateSystems);
    void appendStations(const QList<FeatureWrapper*> &stations);
    void appendTrafoParams(const QList<FeatureWrapper*> &trafoParams);

    void appendObservation(const QPointer<FeatureTreeItem> &parent, Observation *obs);
    void appendReading(const QPointer<FeatureTreeItem> &parent, Observation *obs);

};

#endif // FEATURETREEVIEWMODEL_H
